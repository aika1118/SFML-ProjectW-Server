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

// ���� Ŭ����
class Server
{
public:
	Server(io_context& io_context, short port, thread_pool& pool); // ������: ������ �ʱ�ȭ�ϰ� Ŭ���̾�Ʈ ������ ���

private:
	tcp::acceptor _acceptor; // Ŭ���̾�Ʈ ������ �޴� acceptor
	thread_pool& _pool; // ������ Ǯ ����
	mutex _mutex; // �÷��̾� ���¿� ���� ���ؽ�
	io_context& _io_context;

	void do_accept(); // Ŭ���̾�Ʈ ������ �񵿱������� �޴� �Լ�
};