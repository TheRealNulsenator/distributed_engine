// engine attempt #1 brad nulsen 9/8/2022
#include "../lib/engine.h"

bool gameRunning = true;
ArrayList* Entities;

size_t newEntity(Vec2 pos, Vec2 rad, Vec2 vel, Vec2 acc, CollisionType type, SDL_Color color, char entityName[], int ID)
{	
	Entity newEntity;
	strcpy(newEntity.name, entityName);
	newEntity.ID = ID;
	newEntity.color = color;
	newEntity.body = physics_newBody(pos, rad, vel, acc, type);
	return listAppend(Entities, &newEntity);
}

void EngineInit(void (*callback)()) 
{
	srand((unsigned int)time(NULL));
	time_init(60);
	render_init();
	Entities = listCreate(sizeof(Entity));
	gameRunning = true;
	(*callback)();
	printf("%s\n", "Engine started.");
}

void EngineUpdate(void (*update)()) {
	Entity* player;
	Entity* mouse;
	while (gameRunning)
	{
		time_update();
		render_clear();
		player = listGet(Entities, 0);
		mouse = listGet(Entities, 1);
		for(int i = 0; i < Entities->length; i++)
		{
			Entity* ent = listGet(Entities, i);
			physics_updateBody(ent, Entities);	
			renderBox(ent->body.aabb.position, vec2_scale(ent->body.aabb.radius, 2), ent->color);		
		}
		(*update)();
		render_refresh();
		time_update_late();
	}
}

void EngineExit()
{
	gameRunning = 0;
	render_destroy();
	exit(0);
}


