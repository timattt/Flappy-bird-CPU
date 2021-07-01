#pragma once

// INCLUDES
//=============================
#include "../Engine/Engine.hpp"
#include "../Math/Math.hpp"
#include <stdlib.h>
#include <memory.h>
#include <windows.h>
#include <unistd.h>
#include <wingdi.h>
#include <stdio.h>
//=============================


// CONSTANTS
//=============================
// PHYSICS
#define FLOOR_Y (HEIGHT / 10)
#define G 1
#define BIRD_SPEED (WIDTH / 100)

// SIZES
#define BIRD_WIDTH WIDTH / 10
#define BIRD_HEIGH HEIGHT / 10
//=============================


// Typedefs
//=============================
typedef struct texture tex_t;
//=============================


// STRUCTS
//=============================
struct bird {
	vec2f_t pos;
	vec2f_t speed;
	tex_t * bird_texture;
};
struct texture {
	char * buf;
	int width;
	int height;
};
//=============================


// Global functions
//=============================
int loadTexture(char *path, tex_t * result);
//=============================