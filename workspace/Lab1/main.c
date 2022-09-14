#include <msp430.h> 
#include <grlib.h>

#include <src/global.h>
#include <src/game.h>
#include <src/peripherals.h>
#include <src/heap.h>
#include <src/welcome.h>

State g_State = MAIN_MENU;

void loop(void) {

    switch(g_State) {
    case MAIN_MENU:
        displayMessage(introText);
        while(1) {
            unsigned char currKey = getKey();
            if(currKey== '*'){
                countDown();
                g_State = GAME_LOOP;
                break;
            }
        };
        break;

    case GAME_LOOP:
    	gameLoop();
        break;

    case GAME_OVER:
        displayMessage(defeatText);
        DELAY(2048);
        g_State = MAIN_MENU;
        break;

    default:
        g_State = MAIN_MENU;
        break;
    }
}

void cfg_timer() {
    /* 16 bits, Clock source ACLK, Continuous Mode */
    TBCTL  = CNTL_0 | TBSSEL_1 | MC_2;
    CLEAR_TMRB;
}


int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	configDisplay();
	configKeypad();
	initHeap();
	initGameState();
	cfg_timer();

	while(1) {
	    loop();
	}

	return 0;
}
