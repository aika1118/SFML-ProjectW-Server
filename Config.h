#pragma once
#include <string>
using namespace std;

// 상수 설정
const float M_PI = 22.f / 7.f;

// 맵 관련 설정
const float MAP_CELL_SIZE = 1.f;
const string MAP_BASE_PATH = "./resources/images/";
const bool MAP_IS_DEBUG_DRAW = true;
enum MapType
{
    STAGE_1,
    STAGE_2,
    END_OF_MAP_INDEX
};


// 카메라 관련 설정
const float CAMERA_ZOOM_LEVEL = 20.f;

// 플레이어 관련 설정
const float PLAYER_SIZE_RATIO = 1.28f;
const float PLAYER_SIZE_WIDTH = 1.f;
const float PLAYER_SIZE_HEIGHT = (PLAYER_SIZE_WIDTH * PLAYER_SIZE_RATIO);
const float PLAYER_SENSOR_HX = (PLAYER_SIZE_WIDTH / 2.f) * 0.8f;
const float PLAYER_SENSOR_HY = 0.1f;
const float PLAYER_HEAD_SENSOR_HX = (PLAYER_SIZE_WIDTH / 2.f) * 0.9f;
const float PLAYER_HEAD_SENSOR_HY = 0.1f;
const int PLAYER_MAX_JUMP_COUNT = 1;
const float PLAYER_INVINCIBILITY_TIME = 1.f;
const int PLAYER_MAX_HP = 1;

// 판정 관련

// DJMAX 판정 (쉬움)
//const float JUDGEMENT_PERFECT = 41.67f;
//const float JUDGEMENT_GREAT = 60.f;

// EZ2ON 판정 (어려움)
const float JUDGEMENT_PERFECT = 22.f;
const float JUDGEMENT_GREAT = 60.f;


// 월드 관련 설정
const float WORLD_GRAVITY_X = 0.f;
const int WORLD_VELOCITY_ITERATION = 8;
const int WORLD_POSITION_ITERATION = 3;

// 플레이어 점프 관련 설정
const float PLAYER_JUMP_HEIGHT = 4.f;
const float PLAYER_JUMP_TIME = 0.75f;
// 고정값
const float WORLD_GRAVITY_Y = (8.0f * PLAYER_JUMP_HEIGHT) / (PLAYER_JUMP_TIME * PLAYER_JUMP_TIME);
// 고정값
const float PLAYER_JUMP_VELOCITY = -1.f * (WORLD_GRAVITY_Y * PLAYER_JUMP_TIME) / 2.0f;
// 플레이어 X축 속도
const float PLAYER_MOVE_DISTANCE = 3.5f;
// 고정값
const float PLAYER_MOVE_VELOCITY = (PLAYER_MOVE_DISTANCE / PLAYER_JUMP_TIME);

// 적 관련 설정
const float ENEMY_MOVE_VELOCITY = 5.f;
const float ENEMY_RADIUS = 0.5f;
const float ENEMY_DESTROY_TIME = 2.f;
const float ENEMY_STOP_TIME_AFTER_ATTACKED = 1.f;

// 오브젝트 관련 설정
const float OBJECT_COIN_SIZE = 1.f;
const float OBJECT_SPIKE_SIZE = 1.f;
const float OBJECT_BOX_FRAGILE_SIZE = 1.f;

// UI 관련 설정
const float UI_VIEW_SIZE = 100.f;
const float UI_CHARACTER_SCALE = 0.1f;

// 스킬 관련 설정
const float SKILL_MELEE_ATTACK_COOLDOWN = 0.f;
const float SKILL_MELEE_ATTACK_DAMAGE = 1.f;
const float SKILL_MELEE_ATTACK_SKILLTIME = 0.5f;

// 게임오버 관련 설정
const float GAME_OVER_LIMIT_TIME = (60.f) * (1000.f); // ms 단위 (해당 시간 잠수타면 게임 종료 처리)

// 메뉴 관련 설정
const int MENU_MAX_STAGE_NUM = 2;
const int MENU_MAX_TYPE_NUM = 2;
enum MenuIndex
{
    EXIT = -1,
    MAIN_MENU,
    STAGE_MENU,
    IN_PLAY,
    END_OF_MENU_INDEX
};

// DB 관련 설정
const string DB_HOST = "tcp://35.247.65.229:3306";
const string DB_USERNAME = "wskang";
const string DB_PASSWORD = "Angelic3545!@";
const string DB_SCHEMA = "trip_plan"; // DB 이름 