#pragma once

// 패킷 종류를 정의하는 열거형
enum PacketType
{
	PACKET_READ, // DB에서 데이터를 읽어오는 패킷
	PACKET_WRITE, // DB에 데이터를 쓰는 패킷
	PACKET_SAVE  // 게임 세이브 요청을 위한 패킷
};

// 패킷 헤더 구조체 (패킷 종류와 크기를 저장)
struct PacketHeader
{
	PacketType type; // 패킷 종류
	size_t size; // 패킷 바디의 크기
};

