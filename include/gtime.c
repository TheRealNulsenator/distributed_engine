#include "../lib/gtime.h"

Time_state gtime = {0};

void time_init(uint32_t frame_rate) {
	gtime.frame_rate = frame_rate;
	gtime.frame_delay = 1000.f / frame_rate;
}

void time_update(void) {
	gtime.now = (float)SDL_GetTicks64();
	gtime.delta = (gtime.now - gtime.last) / 1000.f;
	gtime.last = gtime.now;
	++gtime.frame_count;

	if (gtime.now - gtime.frame_last >= 1000.f) {
		gtime.frame_rate = gtime.frame_count;
		gtime.frame_count = 0;
		gtime.frame_last = gtime.now;
	}
}

void time_update_late(void) {
	gtime.frame_time = (float)SDL_GetTicks() - gtime.now;

	if (gtime.frame_delay > gtime.frame_time) {
		SDL_Delay(gtime.frame_delay - gtime.frame_time);
	}
}