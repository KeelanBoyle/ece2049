#ifndef TEMP_CONTROL_H
#define TEMP_CONTROL_H

#include<msp430.h>
#include "peripherals.h"
#include "stdio.h"
#include <inttypes.h>

void adcSetup();
//int* getADCValue();
void displayTemp(float inAvgTempC);
float averageTempC();
int tempReading[32];
uint32_t tempCount;
void adcSetup();
float getData();



#endif
