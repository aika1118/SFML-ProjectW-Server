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
	/**
	 * 서버 생성자: 서버를 초기화하고 클라이언트 연결을 받을 준비를 함
	 *
	 * @param port 서버가 listen할 포트 번호
	 * @param thread_pool 스레드 풀 참조
	 * @param io_context I/O 컨텍스트 참조
	 *
	 * @throws boost::system::system_error 서버 초기화 중 오류가 발생할 경우
	 */
	Server(io_context& io_context, short port, thread_pool& pool);

private:
	tcp::acceptor _acceptor; // 클라이언트 연결을 받는 acceptor
	thread_pool& _pool; // 스레드 풀 참조
	mutex _mutex; // 플레이어 상태에 대한 뮤텍스
	io_context& _io_context;

	/**
	 * 클라이언트 연결을 비동기적으로 받는 함수
	 *
	 * @note 새로운 클라이언트 연결을 지속적으로 받기 위해 재귀적으로 호출됨
	 *
	 * @throws boost::system::system_error 클라이언트 연결을 받는 동안 오류가 발생할 경우
	 */
	void do_accept();
};