#include <msp430.h> 
#include <stdio.h>
#include <inttypes.h>

#include "../../../../embedded_Syst/HW2/Lab2/peripherals.h"
#include "../../../../embedded_Syst/HW2/Lab2/songs/songs.h"
#include "../../../../embedded_Syst/HW2/Lab2/src/gui.h"
#include "../../../../embedded_Syst/HW2/Lab2/src/musicplayer.h"

uint8_t getButtonPressed(void);
void initButtons(void);
void initUserLeds(void);
void configureUserLeds(uint8_t);

void initUserLeds() {
    P1SEL &= ~(BIT0);
    P1DIR |=  (BIT0);

    P4SEL &= ~(BIT7);
    P4DIR |=  (BIT7);

    P1OUT &= ~(BIT0);
    P4OUT &= ~(BIT7);
}

/**
 * main.c
 */
void loop()
{
    char key = getKey();
    if(key != 0) {
        handleGuiKey(key);
    }

    handleMusicButtons(getButtonPressed());
}

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
    _BIS_SR(GIE); // Enable Interrupts

    initLeds();
    initButtons();
    initUserLeds();
    configKeypad();
    initMusicPlayer();
    configDisplay();

    setGuiState(WELCOME);

    while (1) {
        loop();
    }

    return 0;
}

void initButtons(void){
    // for button S1 & S4
    P7SEL &= ~(BIT0|BIT4); // XXX0 XXX0
    P7DIR &= ~(BIT0|BIT4); // XXX0 XXX0
    P7REN |=  (BIT0|BIT4); // XXX1 XXX1
    P7OUT |=  (BIT0|BIT4); // XXX1 XXX1

    // for button S2
    P3SEL &= ~(BIT6); // XXXX XXX0
    P3DIR &= ~(BIT6); // XXXX XXX0
    P3REN |=  (BIT6); // XXXX XXX1
    P3OUT |=  (BIT6); // XXXX XXX1

    // for button S3
    P2SEL &= ~(BIT2); // XXXX XXX0
    P2DIR &= ~(BIT2); // XXXX XXX0
    P2REN |=  (BIT2); // XXXX XXX1
    P2OUT |=  (BIT2); // XXXX XXX1
}

void configureUserLeds(uint8_t inBits) {
    P1OUT = (P1OUT & ~BIT0) | (inBits & BIT0);
    P4OUT = (P4OUT & ~BIT7) | ((inBits << 6) & BIT7);
}

uint8_t getButtonPressed(void)
{
    uint8_t buttons = ((P7IN & BIT0) << 3) | ((P3IN & BIT6) >> 4) | ((P2IN & BIT2) >> 1) | ((P7IN & BIT4) >> 4);
    return ~buttons;
}


