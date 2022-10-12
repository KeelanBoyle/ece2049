/*
 * wavegenerator.c
 *
 *  Created on: Oct 12, 2022
 *      Author: arthur
 */

#include "wavegenerator.h"
#include "msp430.h"
#include "peripherals.h"

uint32_t freq = 0;
WaveSetting waveMode = DC;
float dcLevel = 0.0;

bool setFreq(uint32_t hz) {
    if(freq < 100 || freq > 100000) {
        return false;
    }

    freq = hz;
    return true;
}

void DACInit(void)
{
    // Configure LDAC and CS for digital IO outputs
    DAC_PORT_LDAC_SEL &= ~DAC_PIN_LDAC;
    DAC_PORT_LDAC_DIR |=  DAC_PIN_LDAC;
    DAC_PORT_LDAC_OUT |= DAC_PIN_LDAC; // Deassert LDAC

    DAC_PORT_CS_SEL   &= ~DAC_PIN_CS;
    DAC_PORT_CS_DIR   |=  DAC_PIN_CS;
    DAC_PORT_CS_OUT   |=  DAC_PIN_CS;  // Deassert CS
}

void setWaveMode(WaveSetting setting) {
    waveMode = setting;
    return;
}

void setDCLevel(float level) {
    dcLevel = level;
}

void configureTimer() {
    TA2CTL = TASSEL_2 + ID_0 + MC_1;
    TA2CCR0 = 2^16;
    TA2CCTL0 = CCIE;
}

#pragma vector = TIMER2_A0_VECTOR
__interrupt void timer_A2_ISR(void)
{
    switch(waveMode) {
    case DC:

        break;
    case SQUARE:
        break;
    case SAWTOOTH:
        break;
    case TRIANGLE:
        break;
    default:
        return;
    }
}
