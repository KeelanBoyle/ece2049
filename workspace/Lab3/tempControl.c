#include "tempControl.h"
#include "msp430.h"
#include "stdbool.h"
#include "grlib.h"
#include "peripherals.h"

void adcSetup(){
    REFCTL0 &= ~REFMSTR;
    ADC12CTL0 = ADC12SHT0_10 + ADC12SHT1_10 + ADC12REFON + ADC12ON + ADC12MSC;
    ADC12CTL1 = ADC12CSTARTADD_7 + ADC12SHP;
    P6SEL |= BIT6;
    ADC12MCTL7 = ADC12SREF_0 + ADC12INCH_6;  // this is for Full scale range
    ADC12MCTL8 = ADC12SREF_2 + ADC12INCH_13;
    ADC12MCTL9 = ADC12SREF_1 + ADC12INCH_10 + ADC12EOS; // input channel 10, the referenced used is Ref_1, V(R+) = VREF+ = 1.5 V, and V(R-) = Vss. The full-scale range is 1.5 V
    ADC12CTL0 |= (ADC12SC|ADC12ENC);
}


int* getADCValue(){
    int value1=0;
    int value2=0;
    int value3=0;

    while (ADC12CTL1 & ADC12BUSY){
     __no_operation();

    value1 = ADC12MEM7 & 0x0FFF; // get low 12 bits output memory register 7
    value2 = ADC12MEM8 & 0x0FFF; // get low 12 bits output memory register 8
    value3 = ADC12MEM9 & 0x0FFF; // get low 12 bits output memory register 9

    }
    int adcData[3]={value1,value2,value3};
    return 1000;
}

double averageTempC(){

    tempCount++;
    int currentPos=tempCount%31;
    double elements=0;
    if(tempCount<31){
        elements= tempCount;
    }
    else{

        elements=32.0;
}
    tempReading[currentPos]=getADCValue();
    int i=0;
    double totalTemp=0;
        for (i=0; i<31; i++){
            totalTemp+=tempReading[i];
        }
        double averageADC=totalTemp/elements;
        //convert from adc to temprature
       // double averageTemp= (averageADC-3.3)*(16.6666)+30;//???
        double VrefP=3.3;
        double VrefN=0;
        double bitCount=pow(2,10);
        double averageTemp= averageADC*(VrefP-VrefN)/bitCount+VrefP;
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
