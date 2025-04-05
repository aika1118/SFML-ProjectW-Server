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
		[this](boost::system::error_code ec, tcp::socket socket) { // �񵿱��� Ŭ���̾�Ʈ ���� �Ϸ� �� ������ �ݹ� �Լ� ����
			if (!ec)
			{
				// Ŭ���̾�Ʈ�� ����Ǹ� ������ �����ϰ� ����
				// Session Ŭ�������� shared_from_this()�� ���� ���� ī��Ʈ ����
				make_shared<Session>(move(socket), *this, _pool, _io_context)->start(); // Ŭ���̾�Ʈ ��û �� ���� �ν��Ͻ��� �۵�
			}
			else
			{
				cerr << "do_accept() error !" << endl;
				return;
			}
			do_accept(); // ���� Ŭ���̾�Ʈ ������ ���
		});
}

