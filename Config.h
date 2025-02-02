#pragma once
#include <string>
using namespace std;

// ��� ����
const float M_PI = 22.f / 7.f;

// �� ���� ����
const float MAP_CELL_SIZE = 1.f;
const string MAP_BASE_PATH = "./resources/images/";
const bool MAP_IS_DEBUG_DRAW = true;
enum MapType
{
    STAGE_1,
    STAGE_2,
    END_OF_MAP_INDEX
};


// ī�޶� ���� ����
const float CAMERA_ZOOM_LEVEL = 20.f;

// �÷��̾� ���� ����
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

// ���� ����

// DJMAX ���� (����)
//const float JUDGEMENT_PERFECT = 41.67f;
//const float JUDGEMENT_GREAT = 60.f;

// EZ2ON ���� (�����)
const float JUDGEMENT_PERFECT = 22.f;
const float JUDGEMENT_GREAT = 60.f;


// ���� ���� ����
const float WORLD_GRAVITY_X = 0.f;
const int WORLD_VELOCITY_ITERATION = 8;
const int WORLD_POSITION_ITERATION = 3;

// �÷��̾� ���� ���� ����
const float PLAYER_JUMP_HEIGHT = 4.f;
const float PLAYER_JUMP_TIME = 0.75f;
// ������
const float WORLD_GRAVITY_Y = (8.0f * PLAYER_JUMP_HEIGHT) / (PLAYER_JUMP_TIME * PLAYER_JUMP_TIME);
// ������
const float PLAYER_JUMP_VELOCITY = -1.f * (WORLD_GRAVITY_Y * PLAYER_JUMP_TIME) / 2.0f;
// �÷��̾� X�� �ӵ�
const float PLAYER_MOVE_DISTANCE = 3.5f;
// ������
const float PLAYER_MOVE_VELOCITY = (PLAYER_MOVE_DISTANCE / PLAYER_JUMP_TIME);

// �� ���� ����
const float ENEMY_MOVE_VELOCITY = 5.f;
const float ENEMY_RADIUS = 0.5f;
const float ENEMY_DESTROY_TIME = 2.f;
const float ENEMY_STOP_TIME_AFTER_ATTACKED = 1.f;

// ������Ʈ ���� ����
const float OBJECT_COIN_SIZE = 1.f;
const float OBJECT_SPIKE_SIZE = 1.f;
const float OBJECT_BOX_FRAGILE_SIZE = 1.f;

// UI ���� ����
const float UI_VIEW_SIZE = 100.f;
const float UI_CHARACTER_SCALE = 0.1f;

// ��ų ���� ����
const float SKILL_MELEE_ATTACK_COOLDOWN = 0.f;
const float SKILL_MELEE_ATTACK_DAMAGE = 1.f;
const float SKILL_MELEE_ATTACK_SKILLTIME = 0.5f;

// ���ӿ��� ���� ����
const float GAME_OVER_LIMIT_TIME = (60.f) * (1000.f); // ms ���� (�ش� �ð� ���Ÿ�� ���� ���� ó��)

// �޴� ���� ����
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

// DB ���� ����
const string DB_HOST = "tcp://35.247.65.229:3306";
const string DB_USERNAME = "wskang";
const string DB_PASSWORD = "Angelic3545!@";
const string DB_SCHEMA = "trip_plan"; // DB �̸� 