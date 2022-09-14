#ifndef GLOBAL_H
#define GLOBAL_H

#include <msp430.h>

#define CLEAR_TMRB (TBCTL |= TBCLR);
#define FRQ_TO_TMR(x) (0x7fff / x)

#define DELAY(x) CLEAR_TMRB; while(TBR < (0x20 * x)) {} // Wait in 1024th second increments.

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

static char playAgainText1[] = "Press * to";
static char playAgainText2[] = "play again";
static char introText[]= "SPACE INVADERS";
static char defeatText[]= "RESISTANCE IS FUTILE";

#endif
