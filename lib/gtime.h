#pragma once
#include "global.h"

typedef struct time_state
{
    float delta;
    float now;
    float last;

    float frame_last;
    float frame_delay;
    float frame_time;

    float frame_rate;
    float frame_count;
} Time_state;

extern Time_state gtime;

void time_init(u_int32_t frame_rate);
void time_update(void);
void time_update_late(void);