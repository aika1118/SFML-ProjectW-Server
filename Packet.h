#pragma once
#include <cstdint>
// client, server 공통으로 쓰는 파일이라 파일 저장 위치에 대해 고민 필요

// 패킷 종류를 정의하는 열거형
enum PacketType
{
	PACKET_READ, // DB에서 데이터를 읽어오는 패킷
	PACKET_WRITE, // DB에 데이터를 쓰는 패킷
	PACKET_CREATE, // 닉네임 생성 및 uid 발급 
	PACKET_CREATE_ERROR, // PACKET_CREATE 서버 처리 중 예외사항 발생 시 반환 
	PACKET_SAVE,  // 게임 세이브 요청을 위한 패킷
	PACKET_READ_RANKING, //  랭킹 데이터를 얻기 위한 패킷
	PACKET_READ_MAX_CLEAR_STAGE, // 유저 최신 clear stage 정보를 얻기 위한 패킷
	PAKCET_TYPE_MAX_NUM // 패킷 최대개수 get 용
};

// 패킷 헤더 구조체 (패킷 종류와 크기를 저장)
struct PacketHeader
{
	PacketType type; // 패킷 종류
	size_t size; // 패킷 바디의 크기
	uint32_t request_id; // 요청 ID 추가
};

