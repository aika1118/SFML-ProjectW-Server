#include "Session.h"
#include "Server.h"

Session::Session(tcp::socket socket, Server& server, thread_pool& pool)
	: _socket(move(socket)), _server(server), _pool(pool), _header(), _body()
{
}

void Session::start()
{
	cout << "Session started" << endl;
	do_read_header(); // ��Ŷ ����� �б� ����
}

void Session::do_read_header()
{
	shared_ptr<Session> self(shared_from_this()); // shared_ptr�� �ڱ� �ڽ��� ����
	// socket : �����͸� ���� ����, �����ʹ� buffer�� ������
	// buffer : �����͸� ������ ����
	// handler : �б� �۾��� �Ϸ�Ǹ� ȣ��Ǵ� �ݹ� �Լ�
	cout << "do_read_header() start" << endl;
	async_read(_socket, buffer(&_header, sizeof(_header)),
		[this, self](boost::system::error_code ec, size_t length)
		{
			if (!ec)
			{
				// ��Ŷ ũ�� �˻� (�ִ� 1024����Ʈ�� ����)
				if (_header.size > 1024)
				{
					cerr << "Invalid packet size !" << endl;
					cerr << "Sending packet with size: " << _header.size << endl;
					return;
				}
				do_read_body(); // ����� ���� �� �ٵ� �б� ����
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
	shared_ptr<Session> self(shared_from_this()); // shared_ptr�� �ڱ� �ڽ��� ����
	_body.resize(_header.size); // �ٵ� ũ�⸸ŭ ���۸� Ȯ��
	cout << "do_read_body() start" << endl;
	async_read(_socket, buffer(_body),
		[this, self](boost::system::error_code ec, size_t length)
		{
			if (!ec)
			{
				// �ٵ� ���� �� ��Ŷ ó��
				post(_pool, [this, self] {handle_packet(); }); // ������ Ǯ�� �۾��� �߰�
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
		handle_read_packet(_header.type); // DB �б� ��û ó��
		break;
	case PACKET_WRITE:
		handle_write_packet(_header.type); // DB ���� ��û ó��
		break;
	case PACKET_CREATE:
		handle_create_packet(_header.type); // ���� ���� ��û ó��
		break;
	case PACKET_SAVE:
		handle_save_packet(_header.type); // ���� ���̺� ��û ó��
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
		pstmt->setInt(1, stoi(_body)); // Ŭ���̾�Ʈ�κ��� ���� �����͸� ���ε�
		res = unique_ptr<ResultSet>(pstmt->executeQuery()); // ���� ����

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
			// ���� �÷� ���� �� ���� ��������
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

	send_response(packetType, result); // Ŭ���̾�Ʈ���� ��� ����
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
		pstmt->executeQuery(); // ���� ����

		cout << "handle_write_packet() DB query executed" << endl;
		send_response(packetType, "Write successful\n"); // Ŭ���̾�Ʈ���� ���� �޼��� ����	
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
		// Ʈ����� ���� (�г��� ���� + uid ��� ����)
		con->setAutoCommit(false);

		// 1. �г����� DB�� ����
		string query = "INSERT INTO player_info (username) VALUES (?)";
		unique_ptr<PreparedStatement> pstmt(con->prepareStatement(query));
		pstmt->setString(1, _body);
		pstmt->executeQuery(); // ���� ����

		// 2. uid�� DB���� ������
		query = "SELECT uid FROM player_info WHERE username = ?";
		unique_ptr<PreparedStatement> pstmt2(con->prepareStatement(query));
		pstmt2->setString(1, _body);
		unique_ptr<ResultSet> res(pstmt2->executeQuery()); // ���� ����
		string uid;
		if (res->next()) uid = res->getString("uid");

		send_response(packetType, uid); // Ŭ���̾�Ʈ���� ������ uid ����

		con->commit(); // Ʈ����� Ŀ��

		cout << "handle_create_packet() DB query executed" << endl;
	}
	catch (SQLException& e)
	{
		// ���� �߻� �� Ʈ����� �ѹ�
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
	// shared_ptr�� �ڱ� �ڽ��� ����
	shared_ptr<Session> self(shared_from_this()); 
	// ���� �����͸� shared_ptr�� ���μ� �����ϰ� ���� (response�� ���� �����ڶ� �񵿱� ó�� �� �ı��� ���� ����)
	shared_ptr<string> response_ptr = make_shared<string>(response);

	// ��Ŷ ��� ����
	PacketHeader header;
	header.type = packetType;
	header.size = static_cast<uint32_t>(response.size());

	vector<const_buffer> buffers;
	buffers.push_back(buffer(&header, sizeof(header))); // ��Ŷ ���
	buffers.push_back(buffer(*response_ptr)); // ���� ����

	async_write(_socket, buffers,
		[this, self, response_ptr](boost::system::error_code ec, size_t length)
		{
			if (!ec)
			{

				// ���� ��Ŷ ��û�� �ޱ� ���� �ٽ� do_read_header() ȣ��
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

	// shared_ptr self�� ���� ī��Ʈ�� 0�� �� �� Session ��ü�� �ڵ����� �Ҹ�
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
