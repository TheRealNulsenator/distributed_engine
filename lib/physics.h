//physics engine attempt #1 Brad Nulsen 9/10/2022

#pragma once

#include "global.h"



extern int physicsTicks; //number of physics ticks per rendered frame.

void aabb_MinMax(Vec2* resultMin, Vec2* resultMax, AABB box);
void physics_updateBody(Entity* entity, ArrayList* neighbors);
Body physics_newBody(Vec2 pos, Vec2 rad, Vec2 vel, Vec2 acc, CollisionType type);
bool physics_point_intersect_body(Vec2 point, Body body);
bool physics_body_intersect_body(Body a, Body b);
AABB minkowski_difference(Body a, Body b); 
Vec2 penetration_vector(AABB aabb);
Hit ray_intersect_aabb(Vec2 position, Vec2 magnitude, AABB aabb);
void physics_sweepResponse(Entity* entity, ArrayList* neighbors);
void physics_staticResponse(Entity* entity, ArrayList* neighbors);
void physics_inelasticCollision(Entity* a, Entity* b, Vec2 normal, float decay);