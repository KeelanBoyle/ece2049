#include <msp430.h> 
#include <main.h>
#include <grlib.h>

#include <src/global.h>
#include <src/game.h>
#include <src/peripherals.h>
#include <src/heap.h>

State g_State = MAIN_MENU;

void loop(void) {



    char     intro[14]="SPACE INVADERS";
    char     defeat[20]="RESISTANCE IS FUTILE";
    long int i = 0;

    switch(g_State) {
    case MAIN_MENU:

        displayMessage(intro);
        unsigned char  currKey = getKey();
        if(currKey== '*'){
            countDown();
            g_State = GAME_LOOP;
        }
        break;
    case GAME_LOOP:
    	gameLoop();
        break;
    case GAME_OVER:

      displayMessage(defeat);
      //wait some time
      g_State = MAIN_MENU;
        break;
    default:

        g_State = GAME_OVER;
        break;
    }
    Graphics_flushBuffer(&g_sContext);
}


int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	configDisplay();
	configKeypad();

	initHeap();
	
	initGameState();

	while(1) {
	    loop();
	}

	return 0;
}
