#ifndef GLOBAL_H
#define GLOBAL_H

#include <main.h>

#define HEAP_SIZE 1024

#define GAME_REGION_X 5
#define GAME_REGION_Y 5
#define DISPLAY_X 96
#define DISPLAY_Y 96

#define UPDATE_FREQ 2

#define NULL (void*)0

typedef enum State {
    MAIN_MENU,
    GAME_LOOP,
    GAME_OVER
} State;

extern State g_State;

#endif
