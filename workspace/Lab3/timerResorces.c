#include "timerResorces.h"


void timerConfig(){


    TA2CTL=TASSEL_1+ID_0+MC_1;
    TA2CCR0=32767;
    //1
    TA2CCTL0=CCIE;

}

#pragma vector = TIMER2_A0_VECOTOR
__interrupt void timer_A2_ISR(void)
{
    int help[3]=getADCValue();

    seconds++;

}
