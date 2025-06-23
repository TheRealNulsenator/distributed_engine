// engine attempt #1 brad nulsen 9/8/2022
#pragma once

#include "global.h"
#include "render.h"
#include "physics.h"
#include "input.h"
#include "gtime.h"

extern bool gameRunning;

void EngineInit();
void EngineUpdate();
void EngineExit();
void ReturnError(char * msg);

size_t newEntity(Vec2 pos, Vec2 rad, Vec2 vel, Vec2 acc, CollisionType type, SDL_Color color, char entityName[], int ID);