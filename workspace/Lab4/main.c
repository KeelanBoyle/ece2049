#include <msp430.h> 
#include "welcome.h"
#include "peripherals.h"

#include "stdio.h"
#include "string.h"

/**
 * main.c
 */
typedef enum STATE {
    WELCOME,
    SQUARE,
    DC,
    SAWTOOTH,
    TRIANGLE,
} SIGNALSTATE;

SIGNALSTATE state = WELCOME;
volatile uint16_t scrollValue;
void initButtons(){
    // for button S1 & S4
    P7SEL &= ~(BIT0|BIT4); // XXX0 XXX0
    P7DIR &= ~(BIT0|BIT4); // XXX0 XXX0
    P7REN |=  (BIT0|BIT4); // XXX1 XXX1
    P7OUT |=  (BIT0|BIT4); // XXX1 XXX1

    // for button S2
    P3SEL &= ~(BIT6); // XXXX XXX0
    P3DIR &= ~(BIT6); // XXXX XXX0
    P3REN |=  (BIT6); // XXXX XXX1
    P3OUT |=  (BIT6); // XXXX XXX1

    // for button S3
    P2SEL &= ~(BIT2); // XXXX XXX0
    P2DIR &= ~(BIT2); // XXXX XXX0
    P2REN |=  (BIT2); // XXXX XXX1
    P2OUT |=  (BIT2); // XXXX XXX1
}
uint8_t getButtonPressed(void)
{
    int buttons=0;
     buttons = ((P7IN & BIT0) << 3) | ((P3IN & BIT6) >> 4) | ((P2IN & BIT2) >> 1) | ((P7IN & BIT4) >> 4);

    return buttons;
}
void configScrollWheel() {
    P8SEL &= ~BIT0;
    P8DIR |= BIT0;
    P8OUT |= BIT0;

    P6SEL |= BIT0;

    REFCTL0 &= ~REFMSTR;

    ADC12CTL0 = ADC12SHT0_9 | ADC12ON;
    ADC12CTL1 = ADC12SHP;

    // Use ADC12ADC12MCTL10 register for conversion results
    ADC12MCTL0 = ADC12SREF_0 | ADC12INCH_0;   // ADC12INCH5 = Scroll wheel = A5

    __delay_cycles(100);                      // delay to allow Ref to settle

    ADC12CTL0 |= ADC12ENC;     // Enable conversion
}

uint16_t pollScrollWheel(){
    ADC12CTL0 &= ~ADC12SC;
    ADC12CTL0 |= ADC12SC;

    while (ADC12CTL1 & ADC12BUSY)
        __no_operation();

    scrollValue = ADC12MEM0;
}
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
    configScrollWheel();
    configDisplay();
    initButtons();
    uint8_t buttonPressed=getButtonPressed();
	while(1){
	    buttonPressed=~(getButtonPressed()&0xF)-0xF0;
	    pollScrollWheel();
        if(buttonPressed==8){
            stateMachine(DC);
        }
        else if(buttonPressed==4){
            stateMachine(SQUARE);
            pollScrollWheel();
        }
        else if(buttonPressed==2){
            stateMachine(SAWTOOTH);
        }
        else if(buttonPressed==1){
            stateMachine(TRIANGLE);
        }
        else{
            stateMachine(WELCOME);
            }
	}
}

void stateMachine(SIGNALSTATE newState){
    state=newState;
    switch(state){
    case WELCOME:
        welcome();
        Graphics_flushBuffer(&g_sContext);
        break;
    case DC:
        Graphics_clearDisplay(&g_sContext);
        char numBuf[16];
        snprintf(&numBuf, 15, "%d %s", scrollValue, "DC");
        Graphics_drawString(&g_sContext, numBuf, AUTO_STRING_LENGTH, 10, 20, TRANSPARENT_TEXT);
        break;

    case SQUARE:
        Graphics_clearDisplay(&g_sContext);
        snprintf(&numBuf, 15, "%d %s", scrollValue, "square");
        Graphics_drawString(&g_sContext, numBuf, AUTO_STRING_LENGTH, 10, 20, TRANSPARENT_TEXT);
        break;
    case SAWTOOTH:
       Graphics_clearDisplay(&g_sContext);
       Graphics_drawString(&g_sContext, "saw", AUTO_STRING_LENGTH, 30, 30, TRANSPARENT_TEXT);
       break;
    case TRIANGLE:
        Graphics_clearDisplay(&g_sContext);
        Graphics_drawString(&g_sContext, "trig", AUTO_STRING_LENGTH, 40, 40, TRANSPARENT_TEXT);
        break;
    default:
        Graphics_clearDisplay(&g_sContext);
    }
    Graphics_flushBuffer(&g_sContext);


}

