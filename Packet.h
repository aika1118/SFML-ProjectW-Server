#pragma once

// client, server �������� ���� �����̶� ���� ���� ��ġ�� ���� ��� �ʿ�

// ��Ŷ ������ �����ϴ� ������
enum PacketType
{
	PACKET_READ, // DB���� �����͸� �о���� ��Ŷ
	PACKET_WRITE, // DB�� �����͸� ���� ��Ŷ
	PACKET_CREATE, // �г��� ���� �� uid �߱� 
	PACKET_SAVE  // ���� ���̺� ��û�� ���� ��Ŷ
};

// ��Ŷ ��� ����ü (��Ŷ ������ ũ�⸦ ����)
struct PacketHeader
{
	PacketType type; // ��Ŷ ����
	size_t size; // ��Ŷ �ٵ��� ũ��
};

