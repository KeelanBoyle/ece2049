/*
 * wavegenerator.c
 *
 *  Created on: Oct 12, 2022
 *      Author: arthur
 */

#include "wavegenerator.h"
#include "msp430.h"
#include "peripherals.h"

#define TMR_PAUSE (TA2CTL &= ~(BIT5|BIT4))
#define TMR_PLAY  (TA2CTL |= MC_1)
#define TMR_FREQ  (32768 * 32)
#define TMR_CLR   (TA2CTL |= TACLR)

#define RESOLUTION 8
#define DAC_MAX 4096

#define STEP (DAC_MAX / RESOLUTION)

uint32_t freq = 0;
WaveSetting waveMode = DC;
uint16_t dcLevel = 0;
uint16_t acLevel = 0;
uint16_t squareOn = 0;
uint8_t triangleStatus = 1;
uint16_t triangleCounter = 0;

bool setFreq(uint32_t hz) {
    if(hz < 100 || hz > 1000) {
        return false;
    }

    freq = hz;
    return true;
}


void setWaveMode(WaveSetting setting) {
    waveMode = setting;

    if(waveMode == DC) {
        TMR_PAUSE;
        DACSetValue(dcLevel);
    } else if(waveMode == TRIANGLE) {
        triangleStatus = 1;
        triangleCounter = 0;
        TMR_PLAY;
    }
    else {
        TMR_PLAY;
    }
}

void setDCLevel(float level) {
    dcLevel = (uint16_t)(level * 4096);

    if(waveMode == DC) {
        DACSetValue(dcLevel & 0xFFF);
    }
}

void configureTimer() {
    TA2CTL = TASSEL_2 + ID_0 + MC_1;
    TA2CCR0 = 1;
    TA2CCTL0 = CCIE;
    TMR_PAUSE;
    TMR_CLR;
}

#pragma vector = TIMER2_A0_VECTOR
__interrupt void timer_A2_ISR(void)
{
    switch(waveMode) {
    case SQUARE:
        TMR_PAUSE;
        TA2CCR0 = (uint16_t)(TMR_FREQ / 100);
        TMR_CLR;
        TMR_PLAY;

        squareOn = !squareOn;
        if(squareOn) {
            DACSetValue(dcLevel & 0xFFF);
        }
        else {
            DACSetValue(0);
        }
        return;
    case SAWTOOTH:
        TMR_PAUSE;
        TA2CCR0 = (uint16_t)(TMR_FREQ / (freq * RESOLUTION));
        TMR_CLR;
        TMR_PLAY;

        acLevel = (acLevel + STEP) % DAC_MAX;
        DACSetValue(acLevel & 0xFFF);
        return;
    case TRIANGLE:
        TMR_PAUSE;
        volatile uint16_t ccr0 = (TMR_FREQ / (freq * RESOLUTION * 2));
        TA2CCR0 = ccr0;
        TMR_CLR;
        TMR_PLAY;

        if(triangleStatus == 1) {
            triangleCounter++;
            DACSetValue((triangleCounter * STEP) & 0xFFF);
            if(triangleCounter == RESOLUTION) {
                triangleStatus = 0;
            }
        }

        if(triangleStatus == 0) {
            triangleCounter--;
            DACSetValue((triangleCounter * STEP) & 0xFFF);
            if(triangleCounter == 0) {
                triangleStatus = 1;
            }
        }
        return;
    default:
        return;
    }
}
