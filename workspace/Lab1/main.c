#include <msp430.h> 
#include <main.h>
#include <grlib.h>

State g_State = MAIN_MENU;

void loop(void) {
    switch(g_State) {
    case MAIN_MENU:
        break;
    case GAME_LOOP:
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
	
	while(1) {
	    loop();
	}

	return 0;
}
