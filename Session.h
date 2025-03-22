#pragma once
#include "Packet.h"
#include "Config.h"
#include <mysql/jdbc.h>
#include <boost/asio.hpp>
#include <sstream>

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
	Session(tcp::socket socket, Server& server, thread_pool& pool, io_context& io_context); // 생성자: 소켓과 서버 참조를 초기화
	void start(); // 세션 시작

private:
	tcp::socket _socket; // 클라이언트와 통신할 소켓
	Server& _server; // 서버 참조
	thread_pool& _pool; // 스레드 풀 참조
	PacketHeader _header; // 패킷 헤더
	string _body;  // 패킷 바디

	void do_read_header(); // 패킷 헤더를 비동기적으로 읽는 함수
	void do_read_body(); // 패킷 바디를 비동기적으로 읽는 함수
	void handle_packet(); // 패킷 종류에 따라 적절한 핸들러를 호출하는 함수
	void handle_read_packet(PacketType packetType); // DB에서 데이터를 읽어오는 함수
	void handle_write_packet(PacketType packetType); // DB에 데이터를 쓰는 함수
	void handle_create_packet(PacketType packetType); // // 닉네임 생성 및 uid 발급 
	void handle_save_packet(PacketType packetType); // 게임 세이브 요청을 처리하는 함수
	void handle_read_ranking_packet(PacketType packetType); // 랭킹 데이터를 전달하는 함수
	void handle_read_max_clear_stage_packet(PacketType packetType); // 유저 최신 clear stage 정보 받기

	void send_response(PacketType packetType, const string& response, uint32_t request_id); // 클라이언트에게 응답을 전송하는 함수

	vector<string> parseBody(const string& _body); // , 기준으로 문자열을 하나씩 읽어들여 vector에 저장

	strand<io_context::executor_type> _strand; // 소켓 접근 순차화
};