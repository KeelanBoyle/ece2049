#include <msp430.h> 
#include "welcome.h"
#include "peripherals.h"

#include "stdio.h"
#include "string.h"
#include "wavegenerator.h"

/**
 * main.c
 */
void initButtons(){
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

int main(void)
{
    WDTCTL = WDTPW + WDTHOLD;
    _BIS_SR(GIE);

    setupSPI_DAC();
    DACInit();

    configureTimer();

    setWaveMode(SAWTOOTH);
    setFreq(100);

    while(1) {

    }
}

