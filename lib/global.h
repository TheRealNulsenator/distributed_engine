#pragma once 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <inttypes.h>
#include <SDL2/SDL.h> 
#include <stddef.h>
#include  "gtime.h"
#include <math.h>

typedef enum {false = 0, true = 1} bool;

typedef struct vector2 {
    float x;
    float y;
} Vec2;

typedef struct list {
    size_t length;
    size_t capacity;
    size_t item_size;
    void *items;
} ArrayList;

typedef enum {off=0, kinematic=1, dynamic=2} CollisionType;

typedef struct hit{
    bool is_hit;
    float time;
    Vec2 position;
    Vec2 normal;
} Hit;

typedef struct aabb 
{
    Vec2 position;
    Vec2 radius;
} AABB;

typedef struct body {
    AABB aabb;
    Vec2 velocity;
    Vec2 acceleration;
    float mass;
    CollisionType collisionType;
} Body;

typedef struct entity{
    char name[255];
    int ID;
    Body body;
    SDL_Color color;
} Entity;
extern ArrayList* Entities;

ArrayList *listCreate(size_t item_size);
size_t listAppend(ArrayList *list, void *item);
void *listGet(ArrayList *list, size_t index);
uint8_t listRemove(ArrayList *list, size_t index);

int randomInt(int max);
void ReturnError(char * msg);

Vec2 vec2_sub(Vec2 a, Vec2 b);
Vec2 vec2_add(Vec2 a, Vec2 b);
Vec2 vec2_scale(Vec2 vector, float scalar);
float vec2_mag(Vec2 vector);
