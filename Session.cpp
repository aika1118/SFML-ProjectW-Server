#include "Session.h"
#include "Server.h"

Session::Session(tcp::socket socket, Server& server, thread_pool& pool)
	: _socket(move(socket)), _server(server), _pool(pool), _header(), _body()
{
}

void Session::start()
{
	cout << "Session started" << endl;
	do_read_header(); // 패킷 헤더를 읽기 시작
}

void Session::do_read_header()
{
	shared_ptr<Session> self(shared_from_this()); // shared_ptr로 자기 자신을 유지
	// socket : 데이터를 읽을 소켓, 데이터는 buffer에 저장함
	// buffer : 데이터를 저장할 버퍼
	// handler : 읽기 작업이 완료되면 호출되는 콜백 함수
	cout << "do_read_header() start" << endl;
	async_read(_socket, buffer(&_header, sizeof(_header)),
		[this, self](boost::system::error_code ec, size_t length)
		{
			if (!ec)
			{
				// 패킷 크기 검사 (최대 1024바이트로 제한)
				if (_header.size > 1024)
				{
					cerr << "Invalid packet size !" << endl;
					cerr << "Sending packet with size: " << _header.size << endl;
					return;
				}
				do_read_body(); // 헤더를 읽은 후 바디를 읽기 시작
			}
			else
			{
				cerr << "Read header error !" << endl;
				return;
			}
		}
	);
}

void Session::do_read_body()
{
	shared_ptr<Session> self(shared_from_this()); // shared_ptr로 자기 자신을 유지
	_body.resize(_header.size); // 바디 크기만큼 버퍼를 확장
	cout << "do_read_body() start" << endl;
	async_read(_socket, buffer(_body),
		[this, self](boost::system::error_code ec, size_t length)
		{
			if (!ec)
			{
				// 바디를 읽은 후 패킷 처리
				post(_pool, [this, self] {handle_packet(); }); // 스레드 풀에 작업을 추가
			}
			else
			{
				cerr << "Read body error !" << endl;
				return;
			}
		}
	);
}

void Session::handle_packet()
{
	cout << "handle_packet() start" << endl;
	switch (_header.type)
	{
	case PACKET_READ:
		handle_read_packet(_header.type); // DB 읽기 요청 처리
		break;
	case PACKET_WRITE:
		handle_write_packet(_header.type); // DB 쓰기 요청 처리
		break;
	case PACKET_CREATE:
		handle_create_packet(_header.type); // 게임 생성 요청 처리
		break;
	case PACKET_SAVE:
		handle_save_packet(_header.type); // 게임 세이브 요청 처리
		break;
	default:
		cerr << "Unknown packet type !" << endl;
		break;
	}
}

void Session::handle_read_packet(PacketType packetType)
{
	cout << "handle_read_packet() start" << endl;
	MySQL_Driver* driver = get_mysql_driver_instance();
	unique_ptr<Connection> con(driver->connect(DB_HOST, DB_USERNAME, DB_PASSWORD));
	con->setSchema(DB_SCHEMA);

	cout << "handle_read_packet() DB connected" << endl;

	unique_ptr<ResultSet> res;
	try
	{
		string query = "SELECT * FROM player_stage_record WHERE id = ?";
		unique_ptr<PreparedStatement> pstmt(con->prepareStatement(query));
		pstmt->setInt(1, stoi(_body)); // 클라이언트로부터 받은 데이터를 바인딩
		res = unique_ptr<ResultSet>(pstmt->executeQuery()); // 쿼리 실행

		cout << "handle_read_packet() DB query executed" << endl;
	}
	catch (SQLException& e)
	{
		cout << "SQLException: " << e.what() << endl;
		send_response(packetType, "SQLException: " + string(e.what()));
		return;
	}

	string result;
	while (res->next())
	{
		try
		{
			// 여러 컬럼 값을 한 번에 가져오기
			result +=	res->getString("stage") + " " +
						res->getString("score") + " " +
						res->getString("time") + "\n";
		}
		catch (SQLException& e)
		{
			cout << "SQLException: " << e.what() << endl;
			result += "NULL\n";
		}
		
	}

	cout << "handle_read_packet() ready to send result" << endl;

	send_response(packetType, result); // 클라이언트에게 결과 전송
}

void Session::handle_write_packet(PacketType packetType)
{
	cout << "handle_write_packet() start" << endl;
	MySQL_Driver* driver = get_mysql_driver_instance();
	unique_ptr<Connection> con(driver->connect(DB_HOST, DB_USERNAME, DB_PASSWORD));
	con->setSchema(DB_SCHEMA);

	cout << "handle_write_packet() DB connected" << endl;

	vector<string> body = parseBody(_body);
	if (body.size() != 4)
	{
		cerr << "Invalid body !" << endl;
		return;
	}

	try
	{
		string query = "INSERT INTO player_stage_record (id, stage, score, time) VALUES (?, ?, ?, ?)";
		unique_ptr<PreparedStatement> pstmt(con->prepareStatement(query));

		pstmt->setInt(1, stoi(body[0]));
		pstmt->setInt(2, stoi(body[1]));
		pstmt->setDouble(3, stod(body[2]));
		pstmt->setDouble(4, stod(body[3]));
		pstmt->executeQuery(); // 쿼리 실행

		cout << "handle_write_packet() DB query executed" << endl;
		send_response(packetType, "Write successful\n"); // 클라이언트에게 성공 메세지 전송	
	}
	catch (SQLException& e)
	{
		cout << "SQLException: " << e.what() << endl;
		send_response(packetType, "SQLException: " + string(e.what()));
	}
}

void Session::handle_create_packet(PacketType packetType)
{
	cout << "handle_create_packet() start" << endl;
	MySQL_Driver* driver = get_mysql_driver_instance();
	unique_ptr<Connection> con(driver->connect(DB_HOST, DB_USERNAME, DB_PASSWORD));
	con->setSchema(DB_SCHEMA);

	cout << "handle_create_packet() DB connected" << endl;

	try
	{
		// 트랜잭션 시작 (닉네임 저장 + uid 얻는 과정)
		con->setAutoCommit(false);

		// 1. 닉네임을 DB에 저장
		string query = "INSERT INTO player_info (username) VALUES (?)";
		unique_ptr<PreparedStatement> pstmt(con->prepareStatement(query));
		pstmt->setString(1, _body);
		pstmt->executeQuery(); // 쿼리 실행

		// 2. uid를 DB에서 얻어오기
		query = "SELECT uid FROM player_info WHERE username = ?";
		unique_ptr<PreparedStatement> pstmt2(con->prepareStatement(query));
		pstmt2->setString(1, _body);
		unique_ptr<ResultSet> res(pstmt2->executeQuery()); // 쿼리 실행
		string uid;
		if (res->next()) uid = res->getString("uid");

		send_response(packetType, uid); // 클라이언트에게 생성된 uid 전송

		con->commit(); // 트랜잭션 커밋

		cout << "handle_create_packet() DB query executed" << endl;
	}
	catch (SQLException& e)
	{
		// 에러 발생 시 트랜잭션 롤백
		con->rollback();
		cout << "SQLException: " << e.what() << endl;
		send_response(PACKET_CREATE_ERROR, "SQLException: " + string(e.what()));
	}

	return;
}

void Session::handle_save_packet(PacketType packetType)
{
	return;
}

void Session::send_response(PacketType packetType, const string& response)
{
	// shared_ptr로 자기 자신을 유지
	shared_ptr<Session> self(shared_from_this()); 
	// 응답 데이터를 shared_ptr로 감싸서 안전하게 관리 (response가 현재 참조자라 비동기 처리 중 파괴될 위험 있음)
	shared_ptr<string> response_ptr = make_shared<string>(response);

	// 패킷 헤더 설정
	PacketHeader header;
	header.type = packetType;
	header.size = static_cast<uint32_t>(response.size());

	vector<const_buffer> buffers;
	buffers.push_back(buffer(&header, sizeof(header))); // 패킷 헤더
	buffers.push_back(buffer(*response_ptr)); // 응답 본문

	async_write(_socket, buffers,
		[this, self, response_ptr](boost::system::error_code ec, size_t length)
		{
			if (!ec)
			{

				// 다음 패킷 요청도 받기 위해 다시 do_read_header() 호출
				do_read_header(); 
				cout << "Server sending packet to Client !" << endl;
				cout << "do_read_header() again" << endl;
			}
			else
			{
				cerr << "Error Sending response: " << ec.message() << endl;
			}
		}
	);

	// shared_ptr self의 참조 카운트가 0이 될 때 Session 객체는 자동으로 소멸
}

vector<string> Session::parseBody(const string& _body)
{
	vector<string> result;
	istringstream ss(_body);
	string token;

	while (getline(ss, token, ','))
	{
		result.push_back(token);
	}

	return result;
}
