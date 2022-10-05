
#include "tempControl.h"


void adcSetup(){
    REFCTL0 &= ~REFMSTR;
    ADC12REFON=1;
    ADC12CTL0 = ADC12SHT0_10 + ADC12SHT1_10 + ADC12REFON + ADC12ON + ADC12MSC;
    ADC12CTL1 = ADC12CSTARTADD_7 + ADC12SHP;
    P6SEL |= BIT6;
    ADC12MCTL7 = ADC12SREF_0 + ADC12INCH_6;  // this is for Full scale range
    ADC12MCTL8 = ADC12SREF_2 + ADC12INCH_13;
    ADC12MCTL9 = ADC12SREF_1 + ADC12INCH_10 + ADC12EOS; // input channel 8, the referenced used is Ref_1, V(R+) = VREF+ = 1.5 V, and V(R-) = Vss. The full-scale range is 1.5 V
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
    return adcData;
}
int* floatToInt(float num){
    int intNum=num;
    double tempFDec=(num-intNum)*10.0;
    int tenthF=tempFDec;
    int hehe[2]={intNum, tenthF};
    return hehe;
}
void arrToPrint(int* arr, bool celcius ){
    int yPos=10;
if(celcius ){
    yPos=20;
}
    char buffer[32];

    snprintf(&buffer, 31,"%d.%d",arr[0],arr[1]);

    Graphics_drawString(&g_sContext, buffer, AUTO_STRING_LENGTH, 20, yPos, TRANSPARENT_TEXT);
}
void displayTemp(float inAvgTempC){
    int intC[2]=floatToInt(inAvgTempC);
    arrToPrint(intC,true);
    double avgTempF=inAvgTempC*1.8+32;
    int intF[2]=floatToInt(avgTempF);
    arrToPrint(intC,false);



    Graphics_drawString(&g_sContext, "Temp (C) =>", AUTO_STRING_LENGTH, 16, 8, TRANSPARENT_TEXT);


    Graphics_drawString(&g_sContext, "Temp (F) =>", AUTO_STRING_LENGTH, 16, 10, TRANSPARENT_TEXT);

    return avgTempF;
}
