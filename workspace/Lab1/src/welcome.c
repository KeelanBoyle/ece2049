/*
 * welcome.c
 *
 *  Created on: Sep 7, 2022
 *      Author: kboyl
 */
#include <msp430.h>
#include "peripherals.h"
#include "global.h"
#include "graphics.h"

void displayMessage(char message[]){
    Graphics_clearDisplay(&g_sContext);
    Graphics_drawStringCentered(&g_sContext, message, AUTO_STRING_LENGTH, 48, 15, TRANSPARENT_TEXT);
    Graphics_drawStringCentered(&g_sContext, playAgainText1, AUTO_STRING_LENGTH, 48, 45, TRANSPARENT_TEXT);
    Graphics_drawStringCentered(&g_sContext, playAgainText2, AUTO_STRING_LENGTH, 48, 60, TRANSPARENT_TEXT);
    Graphics_drawImage(&g_sContext, &Alien_Sprite, 30, 30);
    Graphics_drawImage(&g_sContext, &Alien_Sprite, 30, 46);
    Graphics_flushBuffer(&g_sContext);

}

void countDown(){
    Graphics_clearDisplay(&g_sContext);
    Graphics_drawStringCentered(&g_sContext, "3...", AUTO_STRING_LENGTH, 48, 15, TRANSPARENT_TEXT);
    Graphics_flushBuffer(&g_sContext);
    BuzzerOn(256);
    DELAY(1024);
    Graphics_clearDisplay(&g_sContext);
    Graphics_drawStringCentered(&g_sContext, "2...", AUTO_STRING_LENGTH, 48, 35, TRANSPARENT_TEXT);
    Graphics_flushBuffer(&g_sContext);
    BuzzerOn(128);
    DELAY(1024);
    Graphics_clearDisplay(&g_sContext);
    Graphics_drawStringCentered(&g_sContext, "1...", AUTO_STRING_LENGTH, 48, 55, TRANSPARENT_TEXT);
    Graphics_flushBuffer(&g_sContext);
    BuzzerOn(64);
    DELAY(1024);
    BuzzerOff();
}
