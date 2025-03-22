#pragma once

#include "Packet.h"
#include "Session.h"
#include <boost/asio.hpp>
#include <mutex>
#include <memory>
#include <iostream>


using namespace std;
using namespace boost::asio;
using namespace boost::asio::ip;

// 서버 클래스
class Server
{
public:
	Server(io_context& io_context, short port, thread_pool& pool); // 생성자: 서버를 초기화하고 클라이언트 연결을 대기

private:
	tcp::acceptor _acceptor; // 클라이언트 연결을 받는 acceptor
	thread_pool& _pool; // 스레드 풀 참조
	mutex _mutex; // 플레이어 상태에 대한 뮤텍스
	io_context& _io_context;

	void do_accept(); // 클라이언트 연결을 비동기적으로 받는 함수
};