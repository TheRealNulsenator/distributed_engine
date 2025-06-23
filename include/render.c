#include "../lib/render.h"

static SDL_Window* rendererScreen = NULL;
static SDL_Renderer *renderer;

void render_init()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_CreateWindowAndRenderer(RENDERWIDTH, RENDERHEIGHT, SDL_WINDOW_SHOWN, &rendererScreen, &renderer);
	if (!rendererScreen) {
		ReturnError("InitSetup failed to create window");
	}
	render_setCaption("H.P.C.");
	SDL_SetRenderDrawColor(renderer, 0,0,0,255);
}

void render_clear()
{
	SDL_SetRenderDrawColor(renderer, 0,0,0,1);
	SDL_RenderClear(renderer);
}

void render_refresh()
{
	SDL_RenderPresent(renderer);
}

void render_destroy()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(rendererScreen);
	SDL_Quit();
}

void renderBox(Vec2 pos, Vec2 size, SDL_Color color) {
	SDL_Rect rect;
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	rect.w = (int)size.x;
	rect.h = (int)size.y;
	rect.x = (int)pos.x - rect.w/2;
	rect.y = (int)pos.y - rect.h/2;
	SDL_RenderFillRect(renderer,&rect);
}

void renderRay(Vec2 start, Vec2 direction, SDL_Color color)
{
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawLine(renderer, start.x, start.y, start.x + direction.x, start.y + direction.y);
}

void render_setCaption(char * msg) {
		SDL_SetWindowTitle(rendererScreen, msg);
}