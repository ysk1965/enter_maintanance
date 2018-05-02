#pragma once

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <glut.h>
#include <math.h>
#include <windows.h>
#include <string>
#include <random>
#include <mmsystem.h>
#include <time.h>
#include <fstream>
#include <list>
#include <vector>

#pragma comment(lib,"winmm") 
#pragma warning(disable:4996)
using namespace std;

///////////////////////////
GLubyte *LoadDIBitmap(const char *filename, BITMAPINFO **info);
GLubyte *pBytes; // 데이터를 가리킬 포인터
BITMAPINFO *info; // 비트맵 헤더 저장할 변수
GLuint textures[2];
void initTextures();
///////////////////////////

#define SOUND_FILE_NAME			"../sound/item.wav"
#define SOUND_FILE_NAME_ON		"../sound/on.wav"
#define SOUND_FILE_NAME_BGM_1	"../sound/bgm_1.wav"
#define SOUND_FILE_NAME_FINISH	"../sound/finish.wav"
#define SOUND_FILE_NAME_DARK	"../sound/dark.wav"
#define SOUND_FILE_NAME_FAIL	"../sound/IB.wav"
#define SOUND_FILE_NAME_CRASH	"../sound/crash.wav"
#define SOUND_FILE_NAME_MORNING "../sound/morning.wav"

#define MAP_1 "../MAP_Veryeasy.txt"
#define MAP_2 "../MAP_Easy.txt"
#define MAP_3 "../MAP_Normal.txt"
#define MAP_4 "../MAP_Hard.txt"
#define MAP_5 "../MAP_Veryhard.txt"
#define INIT "../init.txt"

#define SIZE 22 // 맵 사이즈

int mapcollect = 0;
int hint_sec = 0;

GLvoid DrawScene(GLvoid);
void Reshape(int w1, int h1);
void TimerFunction(int value);
void setProjection(int w1, int h1);

///////////////////////////////////////////////////////////
//                     Sub Menu                          //
///////////////////////////////////////////////////////////

//한바퀴 회전
GLfloat x_Rotate = 0.0;
GLfloat y_Rotate = 0.0;
GLfloat z_Rotate = 0.0;
int XR_P = 0;
int YR_P = 0;
int ZR_P = 0;

float speedup = 0;
int speed_cnt = 0;
int hp_max = 0;
bool pass = false;
int mini_pass = 0;
int cheat = 0;
int mini_item = 8;

clock_t start, finish;

//bool 값
bool greedon = true;
bool lightening = true;
bool bool_item = false;
bool bool_line = false;
bool shadingon = true;
bool depthon = true;
bool cullingon = true;
bool play_bool = false;

bool hint = false;

///////////////////////////////////////////////////////////
//                     Lookat
///////////////////////////////////////////////////////////

float look_x = 0;
float look_y = 0;
float look_z = -3;
float center_x = 0;
float center_y = 0;
float center_z = -100;
float up_x = 0;
float up_y = 1;
float up_z = 0;

///////////////////////////////////////////////////////////
//                   Light & Fog
///////////////////////////////////////////////////////////

float ambientL = 0.2;
float diffuseL = 0.7;
float specularL = 0.9;
int timef = 100;

bool light_1 = true;
float Light_rot = 0;

GLfloat fog_color[4] = { static_cast<float>(0.18), static_cast<float>(0.18), static_cast<float>(0.18), static_cast<float>(0.3) };
GLfloat density = 0.3;
GLfloat start_fog = -4.0;
GLfloat end_fog = 4.0;

int cutoff = 10;
int warning_sign = 0;
int passblock = 0;

///////////////////////////////////////////////////////////
//                    Board & Ghost                      //
///////////////////////////////////////////////////////////

int makeboard[SIZE][SIZE] = { 0 };
int map = 3;

float Big_x = 0;
float Big_z = 0;
float small_z = 0;
float small_x = 0;

int cnt = 0;
int change = 0;

bool choice1 = false;
bool choice2 = true;

int input = 0;

///////////////////////////////////////////////////////////
//                 Camera & Window                       //
///////////////////////////////////////////////////////////

// 각
float angle = 0.0f;

//벡터
float lx = 0.0f, lz = -1.0f, ly = 0.0f;

//카메라
float x = 0.0f, z = 5.0f, y = 1.75f;

// 이동 변수
float deltaAngle = 0.0f;
float deltaMove = 0;
int xOrigin = -1;

// 높낮이
int h, w;

//프레임
int frame;
long timing, timebase;
char s[50];
char t[50];

int mainscore = 0;

//윈도우
int mainWindow, subWindow1, subWindow2, subWindow3;
int border = 6;
float distribution = 0.5;

/////////////////////////////////////////////

float colorbuffer[4][3] = { 0 };

typedef struct ranking
{
	char* name;
	int map;
	int score;
}ranking;

ranking rankbuffer[100];

struct point
{
	float min_x = -15 - 0.5;
	float min_z = -15 + 0.5;
	float max_x = -15 + 0.5;
	float max_z = -15 - 0.5;
	bool warning = false;
};

point static_block[SIZE][SIZE];
point static_person;
point static_ghost[8];
point static_ghost_1[10];

//////////////////////////////////////////////////////////

int cageX = -55;
int cageY = -65;

float obj_rot = 0;
int replay_cnt = 0;

using Point2D = pair<float, float>;
using Angle = pair<float, float>;
vector<pair<Point2D, Angle>> cont;

enum class key {
	Up, Down, Left, Right
};

//void replaySave(key keynum);
void saveFPS(float x, float y, float deltaAngle, float angle);
void replayLoad();

bool replay_bool = true;

vector<pair<Point2D, Angle>>::iterator it;

bool state = true;
bool pause = false;