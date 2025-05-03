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
	/**
	 * ���� ������: ������ �ʱ�ȭ�ϰ� Ŭ���̾�Ʈ ������ ���� �غ� ��
	 *
	 * @param port ������ listen�� ��Ʈ ��ȣ
	 * @param thread_pool ������ Ǯ ����
	 * @param io_context I/O ���ؽ�Ʈ ����
	 *
	 * @throws boost::system::system_error ���� �ʱ�ȭ �� ������ �߻��� ���
	 */
	Server(io_context& io_context, short port, thread_pool& pool);

private:
	tcp::acceptor _acceptor; // Ŭ���̾�Ʈ ������ �޴� acceptor
	thread_pool& _pool; // ������ Ǯ ����
	mutex _mutex; // �÷��̾� ���¿� ���� ���ؽ�
	io_context& _io_context;

	/**
	 * Ŭ���̾�Ʈ ������ �񵿱������� �޴� �Լ�
	 *
	 * @note ���ο� Ŭ���̾�Ʈ ������ ���������� �ޱ� ���� ��������� ȣ���
	 *
	 * @throws boost::system::system_error Ŭ���̾�Ʈ ������ �޴� ���� ������ �߻��� ���
	 */
	void do_accept();
};