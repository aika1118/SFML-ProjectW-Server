#include "Server.h"
#include <memory>

Server::Server(io_context& io_context, short port, thread_pool& pool)
	: _io_context(io_context), _acceptor(io_context, tcp::endpoint(tcp::v4(), port)), _pool(pool)
{
	do_accept();
}

void Server::do_accept()
{
	_acceptor.async_accept(
		[this](boost::system::error_code ec, tcp::socket socket) { // 비동기적 클라이언트 연결 완료 시 실행할 콜백 함수 정의
			if (!ec)
			{
				// 클라이언트가 연결되면 세션을 생성하고 시작
				// Session 클래스에서 shared_from_this()를 통해 참조 카운트 유지
				make_shared<Session>(move(socket), *this, _pool, _io_context)->start(); // 클라이언트 요청 별 독립 인스턴스로 작동
			}
			else
			{
				cerr << "do_accept() error !" << endl;
				return;
			}
			do_accept(); // 다음 클라이언트 연결을 대기
		});
}

