/**
   global
**/
#ifndef GLOBAL_H
#define GLOBAL_H

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define SCREEN_BITDEPTH 32

#define FOG_FADE_SPEED 3

#define MAX_TILES_WIDTH (SCREEN_WIDTH / 32)
#define MAX_TILES_HEIGHT (SCREEN_HEIGHT / 32)

#define MAX_TILES 32      // Note, these are tile grids!

#define MAP_WIDTH 128
#define MAP_HEIGHT 128
#define MAP_DEPTH 2

#define MAX_GRID_WIDTH 64
#define MAX_GRID_HEIGHT 64

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h> 
#include <math.h>

// Include SDL libraries
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL_rotozoom.h>

#include "SDL_collide.h"

// Include game header files
#include "player.h"
#include "game.h"
#include "engine_func.h"
#include "tile.h"
#include "map.h"
#include "camera.h"
#include "grid.h"
//#include "tinyxml.h"

#include "math.h"

// MATH
#define PI      3.1415926535897932384626433832795   // a definition of PI

#define MAX_DISTANCE    (SCREEN_WIDTH + SCREEN_HEIGHT)

#define PLAYER_MAX_MOVESPEED 1.0

// Screen to draw to
extern SDL_Surface *screen;
extern int mouse_x, mouse_y;
extern SDL_Surface *tiles[MAX_TILES];
extern Map map;
extern Camera cam;
extern Player player;
//extern GridMeta gridmeta;

#endif
