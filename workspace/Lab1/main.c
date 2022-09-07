#include <msp430.h> 
#include <main.h>
#include <global.h>
#include <grlib.h>
#include <gameloop.h>
#include "peripherals.h"

State g_State = GAME_LOOP;

void loop(void) {
    switch(g_State) {
    case MAIN_MENU:
        break;
    case GAME_LOOP:
    	gameLoop();
        break;
    case GAME_OVER:
        break;
    default:
        g_State = GAME_OVER;
        break;
    }
}


int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	configDisplay();
	
	initGameState();

	while(1) {
	    loop();
	}

	return 0;
}
