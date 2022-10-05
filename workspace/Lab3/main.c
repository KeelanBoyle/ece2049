#include <msp430.h>
#include "peripherals.h"

unsigned int in_value;

#include "stdio.h"
#include "string.h"

#define GET_TMR (TA1R)
#define CLEAR_TMR (TA1CTL |= TACLR);
#define DELAY(x) CLEAR_TMR; while(GET_TMR < (0x20 * x)) {} // Wait in 1024th second increments.

#define DAY_TO_SEC 86400U
#define HOUR_TO_SEC 3600U
#define MIN_TO_SEC 60U

#define MAX_FIELDS 5

uint8_t pressed;
volatile uint16_t scrollValue;
uint8_t selected = 0;

uint32_t seconds = 0;
uint16_t editing = 0;

char* Months[] = {
    "JAN",
    "FEB",
    "MAR",
    "APR",
    "MAY",
    "JUN",
    "JUL",
    "AUG",
    "SEP",
    "OCT",
    "NOV",
    "DEC"
};

uint32_t MonthToDay[] = {
    31,
    28,
    31,
    30,
    31,
    30,
    31,
    31,
    30,
    31,
    30,
    31
};

typedef struct Time_t {
    uint8_t sec;
    uint8_t min;
    uint8_t hour;
    uint8_t day;
    uint8_t month;
} Time_t;

void timerConfig() {
    TA2CTL= TASSEL_1 + ID_0 + MC_1;
    TA2CCR0 = 32767;
    TA2CCTL0=CCIE;
}

void secondsToTime(uint32_t sec, Time_t* time) {

    int i = 0;
    for(i = 0; (i < 12) && (sec > (MonthToDay[i] * DAY_TO_SEC)); i++) {
        sec = sec - MonthToDay[i] * DAY_TO_SEC;
    }
    time->month = i;

    time->day = (uint8_t)(sec / DAY_TO_SEC) + 1;
    sec %= (DAY_TO_SEC);

    time->hour = (uint8_t)(sec / (HOUR_TO_SEC));
    sec %= (HOUR_TO_SEC);

    time->min = (uint8_t)(sec / (MIN_TO_SEC));
    sec %= (MIN_TO_SEC);

    time->sec = sec;
}

uint32_t timeToSeconds(Time_t* time) {
    uint32_t ret = 0;

    int i = 0;
    for(i = 0; i < time->month; i++) {
        ret += MonthToDay[i] * DAY_TO_SEC;
    }

    ret += ((time->day) * DAY_TO_SEC) + (time->hour * HOUR_TO_SEC) + (time->min * MIN_TO_SEC) + (time->sec);

    return ret;
}

void lpad0(char* buf, uint16_t number, uint8_t padSize) {
    char numBuf[16];
    snprintf(&numBuf, 15, "%d", number);

    size_t len = strlen(numBuf);

    int i = 0;
    for(i = 0; i < (padSize - len); i++) {
        buf[i] = '0';
    }

    memcpy(&buf[i], &numBuf, len);
    buf[i+len] = '\x00';
}

void dispTime(uint32_t timeInSec, uint8_t selected) {
    Time_t time;
    secondsToTime(timeInSec, &time);

    char dayHourMinSecStrs[4][3];
    lpad0(&dayHourMinSecStrs[0], time.day,  2);
    lpad0(&dayHourMinSecStrs[1], time.hour, 2);
    lpad0(&dayHourMinSecStrs[2], time.min,  2);
    lpad0(&dayHourMinSecStrs[3], time.sec,  2);

    char dispTop[16];
    snprintf(&dispTop, 15, "%s %s", Months[time.month], dayHourMinSecStrs[0]);

    char* dispTopMask = "      ";

    if(selected == 1)
        dispTopMask = "___   ";
    if(selected == 2)
        dispTopMask = "    __";

    char dispBot[16];
    snprintf(&dispBot, 15, "%s:%s:%s", dayHourMinSecStrs[1], dayHourMinSecStrs[2], dayHourMinSecStrs[3]);

    char* dispBotMask = "        ";

    if(selected == 3)
        dispBotMask = "__      ";
    if(selected == 4)
        dispBotMask = "   __   ";
    if(selected == 5)
        dispBotMask = "      __";

    Graphics_drawString(&g_sContext, dispTop, AUTO_STRING_LENGTH, 32, 8, TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext, dispTopMask, AUTO_STRING_LENGTH, 32, 10, TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext, dispBot, AUTO_STRING_LENGTH, 24, 20, TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext, dispBotMask, AUTO_STRING_LENGTH, 24, 22, TRANSPARENT_TEXT);
}

void configScrollWheel() {
    P8SEL &= ~BIT0;
    P8DIR |= BIT0;
    P8OUT |= BIT0;

    P6SEL |= BIT0;

    REFCTL0 &= ~REFMSTR;

    ADC12CTL0 = ADC12SHT0_9 | ADC12ON;
    ADC12CTL1 = ADC12SHP;

    // Use ADC12MEM0 register for conversion results
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

void pollSelectButton() {
    if(~P2IN & BIT1 && !(pressed & BIT0)) {
        selected = (selected + 1) % (MAX_FIELDS + 1);
        editing = 0;
        pressed |= BIT0;
    }

    if(P2IN & BIT1 && (pressed & BIT0)) {
        pressed &= ~BIT0;
    }

    if(~P1IN & BIT1 && !(pressed & BIT1)) {
        editing ^= 1;
        pressed |= BIT1;
    }

    if(P1IN & BIT1 && (pressed & BIT1)) {
        pressed &= ~BIT1;
    }
}

uint32_t remapScroll(uint32_t min, uint32_t max) {
    volatile float scale = (scrollValue/4096.0);
    return min + (max-min)*scale;
}

uint32_t updateDateTime(uint32_t seconds) {
    Time_t time;
    secondsToTime(seconds, &time);

    switch(selected) {
    case 1: // MONTH
        time.month = remapScroll(0, 12);
        break;
    case 2: // DAY
        time.day = remapScroll(0, MonthToDay[time.month]);
        break;
    case 3: // HOUR
        time.hour = remapScroll(0, 24);
        break;
    case 4: // MIN
        time.min = remapScroll(0, 60);
        break;
    case 5: // SEC
        time.sec = remapScroll(0, 60);
        break;
    default:
        return seconds;
    }

    return timeToSeconds(&time);
}

void configSelectButtons() {
    P2SEL &= ~(BIT1);
    P2DIR &= ~(BIT1);
    P2REN |=  (BIT1);
    P2OUT |=  (BIT1);

    P1SEL &= ~(BIT1);
    P1DIR &= ~(BIT1);
    P1REN |=  (BIT1);
    P1OUT |=  (BIT1);
}

void drawAll(uint32_t timeInSeconds) {
    Graphics_clearDisplay(&g_sContext);
    dispTime(timeInSeconds, selected);
    Graphics_flushBuffer(&g_sContext);
}

int main(void)
{
      WDTCTL = WDTPW + WDTHOLD;
      _BIS_SR(GIE);

      configScrollWheel();
      configDisplay();
      configSelectButtons();
      timerConfig();

      while(1) {
          pollScrollWheel();
          pollSelectButton();

          if(editing) {
              seconds = updateDateTime(seconds);
          }
      }
}

#pragma vector = TIMER2_A0_VECTOR
__interrupt void timer_A2_ISR(void)
{
    drawAll(seconds);
    seconds++;
}

