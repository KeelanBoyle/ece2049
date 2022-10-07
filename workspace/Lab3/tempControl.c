#include "tempControl.h"
#include "msp430.h"
#include "stdbool.h"
#include "grlib.h"
#include "peripherals.h"

#define CALADC12_15V_30C  *((unsigned int *)0x1A1A)
// Temperature Sensor Calibration = Reading at 85 degrees C is stored at addr 1A1Ch                                            //See device datasheet for TLV table memory mapping
#define CALADC12_15V_85C  *((unsigned int *)0x1A1C)

unsigned int in_temp;

void adcSetup(void)
{
  volatile float temperatureDegC;
  volatile float temperatureDegF;
  volatile float degC_per_bit;
  volatile unsigned int bits30, bits85;

  WDTCTL = WDTPW + WDTHOLD;      // Stop WDT

  REFCTL0 &= ~REFMSTR;    // Reset REFMSTR to hand over control of
                          // internal reference voltages to
              // ADC12_A control registers

  ADC12CTL0 = ADC12SHT0_9 | ADC12REFON | ADC12ON;     // Internal ref = 1.5V

  ADC12CTL1 = ADC12SHP;                     // Enable sample timer

  // Using ADC12MEM0 to store reading
  ADC12MCTL0 = ADC12SREF_1 + ADC12INCH_10;  // ADC i/p ch A10 = temp sense
                                        // ACD12SREF_1 = internal ref = 1.5v

  __delay_cycles(100);                    // delay to allow Ref to settle
  ADC12CTL0 |= ADC12ENC;              // Enable conversion

  // Use calibration data stored in info memory
  //bits30 = CALADC12_15V_30C;
  //bits85 = CALADC12_15V_85C;
  //degC_per_bit = ((float)(85.0 - 30.0))/((float)(bits85-bits30));



    ADC12CTL0 &= ~ADC12SC;  // clear the start bit
    ADC12CTL0 |= ADC12SC;       // Sampling and conversion start
                        // Single conversion (single channel)
/*
    // Poll busy bit waiting for conversion to complete
    while (ADC12CTL1 & ADC12BUSY)
        __no_operation();
    in_temp = ADC12MEM0;      // Read in results if conversion

    // Temperature in Celsius. See the Device Descriptor Table section in the
    // System Resets, Interrupts, and Operating Modes, System Control Module
    // chapter in the device user's guide for background information on the
    // formula.
    temperatureDegC = (float)((long)in_temp - CALADC12_15V_30C) * degC_per_bit +30.0;

    // Temperature in Fahrenheit
    temperatureDegF =

    __no_operation();    // SET BREAKPOINT HERE
    */
  }

float getData(){
    volatile float temperatureDegC;
    volatile float temperatureDegF;
    volatile float degC_per_bit;
    volatile unsigned int bits30, bits85;
    bits30 = CALADC12_15V_30C;
    bits85 = CALADC12_15V_85C;
    degC_per_bit = ((float)(85.0 - 30.0))/((float)(bits85-bits30));
    while (ADC12CTL1 & ADC12BUSY){
        __no_operation();
    }

        in_temp = ADC12MEM0;      // Read in results if conversion

        // Temperature in Celsius. See the Device Descriptor Table section in the
        // System Resets, Interrupts, and Operating Modes, System Control Module
        // chapter in the device user's guide for background information on the
        // formula.
        temperatureDegC = (float)((long)in_temp - CALADC12_15V_30C) * degC_per_bit +30.0;

return temperatureDegC;

}

//int* getADCValue(){
//    int value1=0;
//    int value2=0;
//
//    while (ADC12CTL1 & ADC12BUSY){
//     __no_operation();
//
//    value1 = ADC12MEM9 & 0x0FFF; // get low 12 bits output memory register 7
//    value2=ADC12MEM9;
//
//    }
//
//    return value1;
//}

float averageTempC(){

    tempCount++;
    int currentPos=tempCount%31;
    double elements=0;
    if(tempCount<31){
        elements= tempCount;
    }
    else{

        elements=32.0;
}
    tempReading[currentPos]=getData();
    int i=0;
    float totalTemp=0;
        for (i=0; i<31; i++){
            totalTemp+=tempReading[i];
        }
        float averageTemp=totalTemp/(float)elements;
        //convert from adc to temprature
       // double averageTemp= (averageADC-3.3)*(16.6666)+30;//???

        return  averageTemp;
}

int* floatToInt(float num){// looks good

    int intNum=(int)num;
    float tempVar=(float)num-(float)intNum;
    float tempFDec=tempVar*0xA;
    int tenthF=(int)tempFDec;
    int hehe[2]={intNum, tenthF};
    return hehe;
}

void arrToPrint(int* arr, bool fahrenheit  ){
    int yPos=40;
    if(fahrenheit  ){
        yPos=70;
    }
    char buffer[32];

    snprintf(&buffer, 31,"%d.%d",arr[0],arr[1]);

    Graphics_drawString(&g_sContext, buffer, AUTO_STRING_LENGTH, 40, yPos, TRANSPARENT_TEXT);

}

void displayTemp(float inAvgTempC){

    int *intC;
    intC=floatToInt(inAvgTempC);

    arrToPrint(intC,false);
    float avgTempF=inAvgTempC*1.8+32;
    int *intF=floatToInt(avgTempF);
    arrToPrint(intF,true);



    Graphics_drawString(&g_sContext, "Temp (C) =>", AUTO_STRING_LENGTH, 10, 30, TRANSPARENT_TEXT);


    Graphics_drawString(&g_sContext, "Temp (F) =>", AUTO_STRING_LENGTH, 10, 60, TRANSPARENT_TEXT);

    return avgTempF;
}
void temp(){

    displayTemp(  averageTempC());
}
