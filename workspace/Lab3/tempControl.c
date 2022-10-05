
#include "tempControl.h"


void adcSetup(){
    REFCTL0 &= ~REFMSTR;
    ADC12CTL0 = ADC12SHT0_10 + ADC12SHT1_10 + ADC12REFON + ADC12ON + ADC12MSC;

}
