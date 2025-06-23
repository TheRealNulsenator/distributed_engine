#pragma once 
#include "global.h"

#define RENDERWIDTH 1600
#define RENDERHEIGHT 900


//SDL_Rect source, destination, dst;

void render_init();
void render_clear();
void render_refresh();
void render_destroy();
void render_setCaption(char * msg);
void renderBox(Vec2 pos, Vec2 size, SDL_Color color);
void renderRay(Vec2 start, Vec2 direction, SDL_Color color);