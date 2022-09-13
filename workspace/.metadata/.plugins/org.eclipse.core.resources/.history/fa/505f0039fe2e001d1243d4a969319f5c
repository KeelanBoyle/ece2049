#include <msp430.h> 
#include <main.h>
#include <global.h>
#include <grlib.h>
#include <gameloop.h>
#include "peripherals.h"

State g_State = GAME_LOOP;

void loop(void) {
	long int i = 0;

    switch(g_State) {
    case MAIN_MENU:
        break;
    case GAME_LOOP:
    	for(i = 0; i < 40000; i++) {}
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
	configKeypad();
	
	initGameState();

	while(1) {
	    loop();
	}

	return 0;
}
