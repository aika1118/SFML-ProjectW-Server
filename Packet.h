#pragma once
#include <cstdint>
// client, server �������� ���� �����̶� ���� ���� ��ġ�� ���� ��� �ʿ�

// ��Ŷ ������ �����ϴ� ������
enum PacketType
{
	PACKET_READ, // DB���� �����͸� �о���� ��Ŷ
	PACKET_WRITE, // DB�� �����͸� ���� ��Ŷ
	PACKET_CREATE, // �г��� ���� �� uid �߱� 
	PACKET_CREATE_ERROR, // PACKET_CREATE ���� ó�� �� ���ܻ��� �߻� �� ��ȯ 
	PACKET_SAVE,  // ���� ���̺� ��û�� ���� ��Ŷ
	PACKET_READ_RANKING, //  ��ŷ �����͸� ��� ���� ��Ŷ
	PACKET_READ_MAX_CLEAR_STAGE, // ���� �ֽ� clear stage ������ ��� ���� ��Ŷ
	PAKCET_TYPE_MAX_NUM // ��Ŷ �ִ밳�� get ��
};

// ��Ŷ ��� ����ü (��Ŷ ������ ũ�⸦ ����)
struct PacketHeader
{
	PacketType type; // ��Ŷ ����
	size_t size; // ��Ŷ �ٵ��� ũ��
	uint32_t request_id; // ��û ID �߰�
};

