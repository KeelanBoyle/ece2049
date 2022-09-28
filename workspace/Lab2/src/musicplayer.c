/*
 * musicplayer.c
 *
 *  Created on: Sep 27, 2022
 *      Author: arthur
 */

#include <msp430.h>
#include <musicplayer.h>
#include <peripherals.h>
#include <gui.h>

#define CLR_TIMER (TA2CTL |= TACLR)
#define PAUSE_TIMER (TA2CTL &= ~MC_1)
#define RESUME_TIMER (TA2CTL |= MC_1)
#define MS_TO_T(x) (uint16_t)(x * (32768/1000))
#define T_TO_MS(x) (uint16_t)((x * 1000) / 32768)
#define SET_NOTE_DUR(x) (TA2CCR0 = MS_TO_T(x))
#define NOTE_TO_MS(x, bpm) (uint16_t)((4 * 60000)/(bpm * x))

#define LIGHT_GREEN P4OUT |= BIT7;
#define LIGHT_RED P1OUT |= BIT0;

#define STOP_GREEN P4OUT &= ~BIT7;
#define STOP_RED P1OUT &= ~BIT0;

const Song* active_song;
MODE active_mode;
uint16_t note_idx;
uint8_t onBeat;
uint8_t playing;

void handleMusicButtons(uint8_t buttons) {
    if (!playing || !active_song) {
        return;
    }

    buttons = (buttons & 0b1111);
    uint8_t leds = (active_song->notes[note_idx].leds & 0b1111);

    if(buttons && (buttons ^ leds)) {
        LIGHT_RED;
        STOP_GREEN;
        return;
    }

    if(buttons & leds) {
        LIGHT_GREEN;
        STOP_RED;
        return;
    }
}


void initMusicPlayer(void) {
    TA2CTL |= TASSEL_1 | ID_0; // Setup Timer
    TA2CCTL0 = CCIE;

    TA2CCR0 = 32768;
    CLR_TIMER;
    PAUSE_TIMER;

    setPlayerMode(BEAT);
};

void playSong(const Song* song) {
    active_song = song;
    note_idx = -1;
    onBeat = 1;
    playing = 1;
    CLR_TIMER;
    RESUME_TIMER;
}

void stopSong(void) {
    PAUSE_TIMER;
    CLR_TIMER;
    note_idx = 0;
    active_song = 0;
    playing = 0;
    setLeds(0);
}

void pauseSong(void) {
    PAUSE_TIMER;
    BuzzerOff();
    playing = 0;
}

void resumeSong(void) {
    playing = 1;
    RESUME_TIMER;
}

Song* currentlyPlaying(void) {
    return active_song;
}

void setPlayerMode(MODE mode) {
    active_mode = mode;
    onBeat = 1;
}

#pragma vector=TIMER2_A0_VECTOR
__interrupt void TimerA2_ISR (void)
{
    if(active_song != 0 && (note_idx < active_song->length - 1 || note_idx == -1)) {
        if(active_mode == HOLD) {
            note_idx++;
            SET_NOTE_DUR(NOTE_TO_MS(active_song->notes[note_idx].duration, active_song->bpm));
            BuzzerOn(active_song->notes[note_idx].freq);
            setLeds(active_song->notes[note_idx].leds);
            CLR_TIMER;
            return;
        }

        if(active_mode == BEAT) {
            if(onBeat) {
                note_idx++;
                volatile test = 3 * (NOTE_TO_MS(active_song->notes[note_idx].duration, active_song->bpm) / 4);
                SET_NOTE_DUR(test);
                BuzzerOn(active_song->notes[note_idx].freq);
                setLeds(active_song->notes[note_idx].leds);
                onBeat = !onBeat;
                CLR_TIMER;
                return;
            }
            else {
                SET_NOTE_DUR(NOTE_TO_MS(active_song->notes[note_idx].duration, active_song->bpm) / 4);
                BuzzerOff();
                onBeat = !onBeat;
                setLeds(0);

                STOP_GREEN;
                STOP_RED;

                CLR_TIMER;
                return;
            }
        }
    }
    else {
        BuzzerOff();
        setGuiState(SONGLIST);
        setLeds(0);
        STOP_GREEN;
        STOP_RED;
        PAUSE_TIMER;
    }
}
