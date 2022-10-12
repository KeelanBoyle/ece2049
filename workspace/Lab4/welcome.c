
#include "peripherals.h"
#include "stdio.h"
#include "grlib.h"




void welcome(){
int center=0;
    Graphics_drawString(&g_sContext, "Welcome!", AUTO_STRING_LENGTH, center, 10, TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext, "button 1=DC", AUTO_STRING_LENGTH, center, 20, TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext, "button 2=Square ", AUTO_STRING_LENGTH, center, 30, TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext, "button 3=SawTooth ", AUTO_STRING_LENGTH, center, 40, TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext, "button 4=Triangle ", AUTO_STRING_LENGTH, center, 50, TRANSPARENT_TEXT);



}
