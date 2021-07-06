#pragma once

// INCLUDES
//=============================
#include <limits.h>
#include <windows.h>
#include <winuser.h>
#include <cstdint>
#include <cstring>
#include <windef.h>
#include <wingdi.h>
#include <winnt.h>
#include <winuser.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include "../Math/Math.hpp"
#include "../Engine/Engine.hpp"
//=============================

#define DEBUG

// Errors handle
//=================================================
extern int errcode;
#define ERROR_MESSAGE_LEN 10000
extern char errorMessage[ERROR_MESSAGE_LEN];
#define NULL_ERR_CODE 1
#define BAD_ARGS_ERR_CODE 2
#ifdef DEBUG
	#define NOT_NULL(A) do {if ((A) == NULL) {memset(errorMessage, 0, ERROR_MESSAGE_LEN); sprintf(errorMessage, "NULL! In file %s on line %d\n", __FILE__, __LINE__); PrintErrorMessage(); exit(0); errcode = NULL_ERR_CODE;return -1;}} while (0)
	#define ENSURE_M(A, MES) do {if (!(A)) {memset(errorMessage, 0, ERROR_MESSAGE_LEN); sprintf(errorMessage, "%s In file %s on line %d\n", MES, __FILE__, __LINE__); PrintErrorMessage(); exit(0); errcode = BAD_ARGS_ERR_CODE; return -1;}} while (0)
	#define ENSURE(A) ENSURE_M(A, "Not ensured!")
#else
	#define NOT_NULL(A) do {if ((A) == NULL) {memset(errorMessage, 0, ERROR_MESSAGE_LEN); sprintf(errorMessage, "NULL! In file %s on line %d\n", __FILE__, __LINE__); errcode = NULL_ERR_CODE;return -1;}} while (0)
	#define ENSURE_M(A, MES) do {if (!(A)) {memset(errorMessage, 0, ERROR_MESSAGE_LEN); sprintf(errorMessage, "%s In file %s on line %d\n", MES, __FILE__, __LINE__); errcode = BAD_ARGS_ERR_CODE; return -1;}} while (0)
	#define ENSURE(A) ENSURE_M(A, "Not ensured!")
#endif
//=================================================


// CONSTANTS
//=============================
#define EPSILON (SCREEN_WIDTH / 300)
#define STATE_GAME 0
#define STATE_START_MENU 1
#define STATE_GAME_OVER_MENU 2

// PHYSICS
#define G 1000.0f

// BIRD
#define BIRD_JUMP_SPEED 300.0f
#define BIRD_JUMP_INTERVAL 0.2f
#define BIRD_WIDTH (SCREEN_WIDTH / 20)
#define BIRD_HEIGHT (BIRD_WIDTH)

// COLUMNS
#define COLUMNS_START_GENERATION_TIME_INTERVAL 3.0f
#define COLUMNS_START_SPEED 100.0f
#define COLUMNS_ACCELERATION 10.0f
#define MAX_COLUMNS 20
#define TOTAL_COLUMN_TEXTURE_SPLITS_COUNT 5

// MENU
#define MENU_MINX (SCREEN_WIDTH * 3 / 10)
#define MENU_MAXX (SCREEN_WIDTH * 7 / 10)
#define MENU_MINY (SCREEN_HEIGHT / 4)
#define MENU_MAXY (MENU_MINY + (MENU_MAXX-MENU_MINX))

#define MENU_BUTTON_MINY (MENU_MINY + 2 * (MENU_MAXY - MENU_MINY) / 3)
//=============================


// Typedefs
//=============================
typedef struct texture tex_t;
//=============================


// STRUCTS
//=============================
struct texture {
	char * buf;
	int width;
	int height;
};
struct bird {
	float jumping_time;
	vec2f_t pos;
	vec2f_t speed;
	tex_t bird_texture;
	bool killed;
};
struct column {
	vec2f_t pos;
	vec2f_t dim;
	int seed;
	int splits_y_count;
};
struct menu {
	tex_t button_not_pressed;
	tex_t button_pressed;
};
//=============================


// Global functions
//=============================
// Textures
int cleanupTexture(tex_t * t);
int loadTexture(const char *path, tex_t * result);
int drawTexture(tex_t * tex, uint32_t dest[SCREEN_HEIGHT][SCREEN_WIDTH],  vec2f_t pos, vec2f_t dim);

// Rectangles
int drawRect_char4(uint32_t dest[SCREEN_HEIGHT][SCREEN_WIDTH], vec2f_t pos, vec2f_t dim, char r, char g, char b, char a);
int drawRect_int(uint32_t dest[SCREEN_HEIGHT][SCREEN_WIDTH], vec2f_t pos, vec2f_t dim, uint32_t col);

// Utils
int cycle_bit_move_left(int x, int k);
int PrintErrorMessage();
bool aabb_intersects(vec2f_t a1, vec2f_t b1, vec2f_t a2, vec2f_t b2);
//=============================
