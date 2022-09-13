#include <msp430.h> 
#include <main.h>
#include <grlib.h>

#include <src/global.h>
#include <src/game.h>
#include <src/peripherals.h>
#include <src/heap.h>

State g_State = MAIN_MENU;
#define FRQ_TO_TMR(x) (0x7fff / x)

void loop(void) {
    volatile uint16_t timer = TBR;


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
        Graphics_clearDisplay(&g_sContext);
    	gameLoop();
        break;
    case GAME_OVER:

        if(timer > FRQ_TO_TMR(0x1)) {
            CLEAR_TMRB;
            g_State = MAIN_MENU;
        }
        else{

      displayMessage(defeat);
        }


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

	initHeap();
	
	initGameState();

	while(1) {
	    loop();
	}

	return 0;
}
