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

// ��Ų�� �׽�Ʈ
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
	/**
	 * ���� ������: Ŭ���̾�Ʈ�� ����� ���ϰ� ���� ������ �ʱ�ȭ
	 *
	 * @param socket Ŭ���̾�Ʈ�� ����� ����
	 * @param server ���� ����
	 * @param pool ������ Ǯ ����
	 * @param io_context I/O ���ؽ�Ʈ ����
	 */
	Session(tcp::socket socket, Server& server, thread_pool& pool, io_context& io_context);

	/**
	 * @brief ������ �����ϰ� ��Ŷ ����� �б� ����
	 */
	void start();

private:
	tcp::socket _socket; // Ŭ���̾�Ʈ�� ����� ����
	Server& _server; // ���� ����
	thread_pool& _pool; // ������ Ǯ ����
	PacketHeader _header; // ��Ŷ ���
	string _body;  // ��Ŷ �ٵ�

	/**
	 * @brief ��Ŷ ����� �񵿱������� �б� ����
	 */
	void do_read_header(); 
	/**
	 * @brief ��Ŷ �ٵ� �񵿱������� ����
	 */
	void do_read_body();
	/**
	 * @brief ��Ŷ ������ ���� ������ �ڵ鷯�� ȣ���ϴ� �Լ�
	 */
	void handle_packet(); 
	/**
	 * @brief DB���� �����͸� �о���� �Լ�
	 */
	void handle_read_packet(PacketType packetType);
	/**
	 * @brief DB�� �����͸� ���� �Լ�
	 */
	void handle_write_packet(PacketType packetType);
	/**
	 * @brief �г��� ���� �� uid �߱�
	 */
	void handle_create_packet(PacketType packetType); 
	/**
	 * @brief ��ŷ �����͸� �����ϴ� �Լ�
	 */
	void handle_read_ranking_packet(PacketType packetType);
	/**
	 * @brief ���� �ֽ� clear stage ���� �ޱ�
	 */
	void handle_read_max_clear_stage_packet(PacketType packetType);
	/**
	 * Ŭ���̾�Ʈ���� ������ �����ϴ� �Լ�
	 *
	 * @param packetType ��Ŷ ����
	 * @param response Ŭ���̾�Ʈ���� ������ ���� ������
	 * @param request_id ��û ID
	 */
	void send_response(PacketType packetType, const string& response, uint32_t request_id);
	/**
	 * Ŭ���̾�Ʈ�κ��� ���� �ٵ� �����͸� �Ľ��Ͽ� ���ͷ� ��ȯ�ϴ� �Լ�
	 *
	 * @param _body Ŭ���̾�Ʈ�κ��� ���� �ٵ� ������
	 * @return �Ľ̵� �����͸� ���� ����
	 */
	vector<string> parseBody(const string& _body);

	strand<io_context::executor_type> _strand; // ���� ���� ����ȭ
};