/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2012, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************
 *
 *                       MSP430 CODE EXAMPLE DISCLAIMER
 *
 * MSP430 code examples are self-contained low-level programs that typically
 * demonstrate a single peripheral function or device feature in a highly
 * concise manner. For this the code may rely on the device's power-on default
 * register values and settings such as the clock configuration and care must
 * be taken when combining code from several examples to avoid potential side
 * effects. Also see www.ti.com/grace for a GUI- and www.ti.com/msp430ware
 * for an API functional library-approach to peripheral configuration.
 *
 * --/COPYRIGHT--*/
#include <msp430.h>
#include "peripherals.h"
unsigned int in_value;
int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  // Configure P8.0 as digital IO output and set it to 1
  // This supplied 3.3 volts across scroll wheel potentiometer
  // See schematic at end or MSP-EXP430F5529 board users guide

}
void scrollWheel(){
    P8SEL &= ~BIT0;
      P8DIR |= BIT0;
      P8OUT |= BIT0;
      REFCTL0 &= ~REFMSTR;                      // Reset REFMSTR to hand over control of
                                                // internal reference voltages to
                       // ADC12_A control registers
      ADC12CTL0 = ADC12SHT0_9 | ADC12ON;

      ADC12CTL1 = ADC12SHP;                     // Enable sample timer

      // Use ADC12MEM0 register for conversion results
      ADC12MCTL0 = ADC12SREF_0 + ADC12INCH_5;   // ADC12INCH5 = Scroll wheel = A5
                       // ACD12SREF_0 = Vref+ = Vcc
      __delay_cycles(100);                      // delay to allow Ref to settle
      ADC12CTL0 |= ADC12ENC;     // Enable conversion
      while(1)
      {
        ADC12CTL0 &= ~ADC12SC;  // clear the start bit
        ADC12CTL0 |= ADC12SC;               // Sampling and conversion start
         // Single conversion (single channel)
        // Poll busy bit waiting for conversion to complete
        while (ADC12CTL1 & ADC12BUSY)
         __no_operation();
        in_value = ADC12MEM0;               // Read results if conversion done
        __no_operation();                       // SET BREAKPOINT HERE
      }
}
int* displayTime(long unsigned int inTime){
    double ticToSec=1;
    int sec2Min=60;
    int min2hour=60;
    int hour2Day=24;
    int day2Mounth=30;
    int inTimeSeconds=inTime*ticToSec;
    int sec2Mount=sec2Min*min2hour*hour2Day*day2Mounth;
    int mounth= floor( inTimeSeconds/sec2Mount);
    int sec2day= sec2Min*min2hour*hour2Day;
    int day= floor((inTimeSeconds-mounth*sec2Mount)/sec2day);
    int sec2hour=sec2Min*min2hour;
    int hour =floor( inTimeSeconds-    (mounth*sec2Mount+day*sec2day)/sec2hour);
    int min= floor(inTimeSeconds- (mounth*sec2Mount+day*sec2day+hour*sec2hour)/sec2Min);
    int sec=inTimeSeconds-(mounth*sec2Mount+day*sec2day+hour*sec2hour+min*sec2Min);
    int displayTime[5]={mounth+48,day+48,hour+48,min+48,sec+48};
    return displayTime;
}

double displayTemp(float inAvgTempC){
    double avgTempF=inAvgTempC*1.8+32;
    Graphics_clearDisplay(&g_sContext);
    Graphics_drawString(&g_sContext, "Temp (C) =>", AUTO_STRING_LENGTH, 16, 8, TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext, inAvgTempC, AUTO_STRING_LENGTH, 20, 8, TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext, "Temp (F) =>", AUTO_STRING_LENGTH, 16, 10, TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext, avgTempF, AUTO_STRING_LENGTH, 20, 10, TRANSPARENT_TEXT);

    return avgTempF;
}
