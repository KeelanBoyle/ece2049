#include <msp430.h> 
#include <stdio.h>
//#include "peripherals.h"
int buttonPressed(void);
/**
 * main.c
 */
void loop(){

    int pog=0;
        pog=buttonPressed();
        printf(pog);
}
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	initButtons();
while (1){
    loop();
}
	return 0;
}

void initButtons(void){
   // for button S1 & S4
P7SEL&=~(BIT0|BIT4); // 0XXX XXX0
P7DIR&=~(BIT0|BIT4); // 0XXX XXX0
P7REN|=(BIT0|BIT4);  // 1XXX XXX1
P7OUT|=(BIT0|BIT4);  // 1XXX XXX1

// for button S2
P3SEL&=~(BIT6); // XXXX XXX0
P3DIR&=~(BIT6); // XXXX XXX0
P3REN|=(BIT6); // XXXX XXX1
P3OUT|=(BIT6);  // XXXX XXX1

// for button S3
P2SEL&=~(BIT2); // XXXX XXX0
P2DIR&=~(BIT2); // XXXX XXX0
P2REN|=(BIT2); // XXXX XXX1
P2OUT|=(BIT2);  // XXXX XXX1


}
// Turn on 4 colored LEDs on P6.1-6.4 to match the hex value
    // passed in on low nibble state. Unfortunately the LEDs are
    // out of order with 6.2 is the left most (i.e. what we think
    // of as MSB), then 6.1 followed by 6.3 and finally 6.4 is
    // the right most (i.e.  what we think of as LSB) so we have
    // to be a bit clever in implementing our LEDs
int buttonPressed(void){
    int val=0;

    int x=P2IN;
    int mask= BIT2;
   int  currentLED=x&mask;
    if(currentLED==0xFFFF){
        val=1;
        setLeds(0x0000);
        return val;

    }
    else
    {
        setLeds(0xFFFF);
        return val;
    }
    }


