#pragma once
#include "Packet.h"
#include "Config.h"
#include <mysql/jdbc.h>
#include <boost/asio.hpp>
#include <sstream>

// Ŭ���̾�Ʈ���� ������ �����ϴ� Ŭ����
// ����� ���� �ش� Ŭ���� ��ü�� �ڱ� �ڽ��� �����ϰ� shared_ptr�� ���� �� �ִ� ����� ���� ��
class Server;

using namespace sql;
using namespace sql::mysql;
using namespace boost::asio;
using namespace boost::asio::ip;

class Session : public enable_shared_from_this<Session>
{
public:
	Session(tcp::socket socket, Server& server, thread_pool& pool); // ������: ���ϰ� ���� ������ �ʱ�ȭ
	void start(); // ���� ����

private:
	tcp::socket _socket; // Ŭ���̾�Ʈ�� ����� ����
	Server& _server; // ���� ����
	thread_pool& _pool; // ������ Ǯ ����
	PacketHeader _header; // ��Ŷ ���
	string _body;  // ��Ŷ �ٵ�

	void do_read_header(); // ��Ŷ ����� �񵿱������� �д� �Լ�
	void do_read_body(); // ��Ŷ �ٵ� �񵿱������� �д� �Լ�
	void handle_packet(); // ��Ŷ ������ ���� ������ �ڵ鷯�� ȣ���ϴ� �Լ�
	void handle_read_packet(); // DB���� �����͸� �о���� �Լ�
	void handle_write_packet(); // DB�� �����͸� ���� �Լ�
	void handle_save_packet(); // ���� ���̺� ��û�� ó���ϴ� �Լ�

	void send_response(const string& response); // Ŭ���̾�Ʈ���� ������ �����ϴ� �Լ�

	vector<string> parseBody(const string& _body); // , �������� ���ڿ��� �ϳ��� �о�鿩 vector�� ����
};