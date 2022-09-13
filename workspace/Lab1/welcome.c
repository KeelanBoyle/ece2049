/*
 * welcome.c
 *
 *  Created on: Sep 7, 2022
 *      Author: kboyl
 */
#include <msp430.h>
#include "src/peripherals.h"

void displayMessage(char message[]){
    Graphics_clearDisplay(&g_sContext);
    Graphics_drawStringCentered(&g_sContext, message, AUTO_STRING_LENGTH, 48, 15, TRANSPARENT_TEXT);

    char playAgian[21]= "Press * to play agian";
    Graphics_drawStringCentered(&g_sContext, playAgian, AUTO_STRING_LENGTH, 48, 45, TRANSPARENT_TEXT);

}

void countDown(){
    Graphics_clearDisplay(&g_sContext);
    Graphics_drawStringCentered(&g_sContext, "3...", AUTO_STRING_LENGTH, 48, 15, TRANSPARENT_TEXT);
    //wait
    Graphics_drawStringCentered(&g_sContext, "2...", AUTO_STRING_LENGTH, 48, 35, TRANSPARENT_TEXT);
    //wait
    Graphics_drawStringCentered(&g_sContext, "1...", AUTO_STRING_LENGTH, 48, 55, TRANSPARENT_TEXT);
    //wait
}
