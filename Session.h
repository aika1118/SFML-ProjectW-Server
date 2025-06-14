#pragma once
#include "Packet.h"
#include "Config.h"
#include <boost/asio.hpp>
#include <sstream>
#ifdef _WIN32
	#include <mysql/jdbc.h>
#else
	#include "mysql_driver.h"
	#include "mysql_connection.h"
	#include <cppconn/resultset.h>
	#include <cppconn/prepared_statement.h>
	#include <cppconn/driver.h>
#endif

// 젠킨스 테스트
// 클라이언트와의 세션을 관리하는 클래스
// 상속을 통해 해당 클래스 객체가 자기 자신을 안전하게 shared_ptr로 만들 수 있는 기능을 갖게 됨
class Server;

using namespace sql;
using namespace sql::mysql;
using namespace boost::asio;
using namespace boost::asio::ip;

class Session : public enable_shared_from_this<Session>
{
public:
	/**
	 * 세션 생성자: 클라이언트와 통신할 소켓과 서버 참조를 초기화
	 *
	 * @param socket 클라이언트와 통신할 소켓
	 * @param server 서버 참조
	 * @param pool 스레드 풀 참조
	 * @param io_context I/O 컨텍스트 참조
	 */
	Session(tcp::socket socket, Server& server, thread_pool& pool, io_context& io_context);

	/**
	 * @brief 세션을 시작하고 패킷 헤더를 읽기 시작
	 */
	void start();

private:
	tcp::socket _socket; // 클라이언트와 통신할 소켓
	Server& _server; // 서버 참조
	thread_pool& _pool; // 스레드 풀 참조
	PacketHeader _header; // 패킷 헤더
	string _body;  // 패킷 바디

	/**
	 * @brief 패킷 헤더를 비동기적으로 읽기 시작
	 */
	void do_read_header(); 
	/**
	 * @brief 패킷 바디를 비동기적으로 읽음
	 */
	void do_read_body();
	/**
	 * @brief 패킷 종류에 따라 적절한 핸들러를 호출하는 함수
	 */
	void handle_packet(); 
	/**
	 * @brief DB에서 데이터를 읽어오는 함수
	 */
	void handle_read_packet(PacketType packetType);
	/**
	 * @brief DB에 데이터를 쓰는 함수
	 */
	void handle_write_packet(PacketType packetType);
	/**
	 * @brief 닉네임 생성 및 uid 발급
	 */
	void handle_create_packet(PacketType packetType); 
	/**
	 * @brief 랭킹 데이터를 전달하는 함수
	 */
	void handle_read_ranking_packet(PacketType packetType);
	/**
	 * @brief 유저 최신 clear stage 정보 받기
	 */
	void handle_read_max_clear_stage_packet(PacketType packetType);
	/**
	 * 클라이언트에게 응답을 전송하는 함수
	 *
	 * @param packetType 패킷 종류
	 * @param response 클라이언트에게 전송할 응답 데이터
	 * @param request_id 요청 ID
	 */
	void send_response(PacketType packetType, const string& response, uint32_t request_id);
	/**
	 * 클라이언트로부터 받은 바디 데이터를 파싱하여 벡터로 반환하는 함수
	 *
	 * @param _body 클라이언트로부터 받은 바디 데이터
	 * @return 파싱된 데이터를 담은 벡터
	 */
	vector<string> parseBody(const string& _body);

	strand<io_context::executor_type> _strand; // 소켓 접근 순차화
};