#include "lib/engine.h"


Vec2 mousePosition;
Entity* player;
int playerIndex;
Vec2 entitySize = (Vec2){20, 20};

void initialize_player()
{
	SDL_Color playerColor = (SDL_Color){randomInt(256),randomInt(256),randomInt(256),255};
	Vec2 centerOfScreen = (Vec2){RENDERWIDTH/4, RENDERHEIGHT/4};
	playerIndex = newEntity(centerOfScreen, entitySize, (Vec2){0,0}, (Vec2){0,0}, 2, playerColor, "player", 0);
	Entity* player = listGet(Entities, playerIndex);
	player->body.aabb.position = centerOfScreen;
	player->body.velocity = (Vec2){0};
	player->body.acceleration = (Vec2){0};
	player->body.mass = 3;
}

void initialize_walls()
{
	Vec2 position, size;
	SDL_Color color = (SDL_Color){255,255,255,255};
	float thickness = 50;

	//top
	position = (Vec2){RENDERWIDTH/2 - thickness/2, 0};
	size = (Vec2){RENDERWIDTH/2, thickness/2};
	newEntity(position, size, (Vec2){0,0}, (Vec2){0,0}, 0, color, "wall", Entities->length);

	//bottom
	position = (Vec2){RENDERWIDTH/2 + thickness/2, RENDERHEIGHT};
	size = (Vec2){RENDERWIDTH/2, thickness/2};
	newEntity(position, size, (Vec2){0,0}, (Vec2){0,0}, 0, color, "wall", Entities->length);

	//left
	position = (Vec2){0, RENDERHEIGHT/2 + thickness/2};
	size = (Vec2){thickness/2, RENDERHEIGHT/2};
	newEntity(position, size, (Vec2){0,0}, (Vec2){0,0}, 0, color, "wall", Entities->length);

	//right
	position = (Vec2){RENDERWIDTH, RENDERHEIGHT/2 - thickness/2};
	size = (Vec2){thickness/2, RENDERHEIGHT/2};
	newEntity(position, size, (Vec2){0,0}, (Vec2){0,0}, 0, color, "wall", Entities->length);

}

void initialize_randomShit()
{
	Vec2 velocity;
	Vec2 position;
	SDL_Color color;
	for(int i = 0; i < 25; i++)
	{
		color = (SDL_Color){randomInt(256),randomInt(256),randomInt(256),255};
		Entity* new = listGet(Entities, newEntity((Vec2){0}, vec2_scale(entitySize, 0.5), (Vec2){0}, (Vec2){0}, 2, color, "randomShit", Entities->length));
		new->body.aabb.position = (Vec2){randomInt(RENDERWIDTH/2), randomInt(RENDERHEIGHT/2)};
		new->body.velocity = (Vec2){1000,1000};
		new->body.acceleration = (Vec2){0,0};
		new->body.mass = 1;
	}
}

void input_mouse()
{
	int x,y;
	SDL_GetMouseState(&x, &y);
	mousePosition.x = (float)x;
	mousePosition.y = (float)y;
	SDL_ShowCursor(1);
}

void input_keyboard() 
{
	SDL_Event event;
	int keypressed;
	const Uint8 *kbState = SDL_GetKeyboardState(NULL);
	const int speed = 400;
	if (kbState[SDL_SCANCODE_SPACE])
	{
		Entity* player = listGet(Entities, playerIndex);
		player->body.aabb.position = (Vec2){RENDERWIDTH/2, RENDERHEIGHT/2};
	}
	if(kbState[SDL_SCANCODE_W])
	{
		//move up
		player->body.acceleration.y = speed * -1;
	}
	else if(kbState[SDL_SCANCODE_S])
	{
		//move down
		player->body.acceleration.y = speed;
	}
	else
	{
		player->body.acceleration.y = 0;
	}
	if(kbState[SDL_SCANCODE_A])
	{
		//move left
		player->body.acceleration.x = speed * -1;
	}
	else if(kbState[SDL_SCANCODE_D])
	{
		//move right
		player->body.acceleration.x = speed;
	}
	else
	{
		player->body.acceleration.x = 0;
	}
	while (SDL_PollEvent(&event)) 
	{
		switch (event.type) {
			case SDL_KEYDOWN:
				keypressed = event.key.keysym.sym;
				if(keypressed == SDLK_ESCAPE)
				{
					gameRunning = false;
				}				
				break;
			case SDL_QUIT:
			{
				gameRunning = false;
				break;
			}
			case SDL_KEYUP: {
				break;
			}
		}
	} 
}

void collision_check()
{
	bool targetInSight = false;
	Vec2 start = player->body.aabb.position;
	Vec2 end = mousePosition;
	Vec2 ray = vec2_sub(end, start);
	Hit hitTest;
	for(int i = 0; i < Entities->length; i++)
	{
		Entity* ent = listGet(Entities, i);
		if(ent->body.aabb.position.x > RENDERWIDTH || ent->body.aabb.position.x < 0 || ent->body.aabb.position.y > RENDERHEIGHT || ent->body.aabb.position.y < 0)
		{
			ent->body.aabb.position = (Vec2){RENDERWIDTH/2, RENDERHEIGHT/2};
			printf("%s %i out of bounds \n", ent->name, ent->ID);
		}
		if(strcmp(ent->name, "player") != 0)
		{
			hitTest = ray_intersect_aabb(start, ray, ent->body.aabb);
			if(hitTest.is_hit)
			{
				targetInSight = true;		
				Body collisionBody = 
				{
					.aabb = 
					{
						.position = hitTest.position,
						.radius = 3,
					},
					.acceleration = (Vec2){0},
					.velocity = (Vec2){0}
				};
				Vec2 fixed = vec2_add(hitTest.position, penetration_vector(minkowski_difference(ent->body, collisionBody)));
				renderBox(fixed, (Vec2){3,3}, player->color);
			}	
		}	
	}
	if(targetInSight)
	{
		renderRay(start, ray, (SDL_Color){255,0,0,255});
	}
	else
	{
		renderRay(start, ray, (SDL_Color){255,255,255,255});
	}
}

void start()
{
	initialize_player();
	initialize_walls();
	initialize_randomShit();
}

void update()
{
	player = listGet(Entities, playerIndex);
	input_mouse();
	input_keyboard();
	collision_check();
	printf("%f\n", gtime.frame_rate);
}

int main(int argc, char * args[])
{
	EngineInit(&start);
	EngineUpdate(&update);
	EngineExit();
    return 0;
}
