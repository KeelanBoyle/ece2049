/*
 * wavegenerator.h
 *
 *  Created on: Oct 12, 2022
 *      Author: arthur
 */

#ifndef WAVEGENERATOR_H_
#define WAVEGENERATOR_H_

#include <stdbool.h>
#include <inttypes.h>

typedef enum WaveSetting {
    DC,
    SQUARE,
    SAWTOOTH,
    TRIANGLE
} WaveSetting;

bool setFreq(uint32_t hz);
void setWaveMode(WaveSetting setting);
void configureTimer(void);

#endif /* WAVEGENERATOR_H_ */
