#ifndef TEMP_CONTROL_H
#define TEMP_CONTROL_H
#include<msp430.h>
#include "peripherals.h"
#include "stdio.h"
void adcSetup();
int* getADCValue();
void displayTemp(float inAvgTempC);



#endif
