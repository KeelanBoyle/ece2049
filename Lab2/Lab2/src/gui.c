#include "gui.h"
#include "songs.h"
#include "grlib.h"
#include "peripherals.h"
#include "msp430.h"

uint8_t currentPage = 0;
GUISTATE state = WELCOME;

void formatNumToAscii(uint32_t number, char* buffer, uint8_t length) {
    int i = length;
    for(; i > 0; i--) {

    }
}

void drawSongList() {
    Graphics_clearDisplay(&g_sContext);

    uint8_t header[] = " :";

    int i;
    int numSongs = sizeof(songList)/sizeof(Song*);

    Graphics_drawString(&g_sContext, "Song Title", AUTO_STRING_LENGTH, 16, 8, TRANSPARENT_TEXT);

    for(i = 0; i < 10; i++) {
        header[0] = (uint8_t)(i + 48);
        Graphics_drawStringCentered(&g_sContext, header, AUTO_STRING_LENGTH, 8, 19 +  (8 * i), TRANSPARENT_TEXT);
        if((currentPage * 10) + i < numSongs) {
            Graphics_drawString(&g_sContext, songList[(currentPage * 10) + i]->name, AUTO_STRING_LENGTH, 16, 16 + (8 * i), TRANSPARENT_TEXT);
        }
        else {
            Graphics_drawString(&g_sContext, "No Song", AUTO_STRING_LENGTH, 16, 16 + (8 * i), TRANSPARENT_TEXT);
        }
    }

    Graphics_flushBuffer(&g_sContext);
}

void drawPauseScreen() {
    Graphics_clearDisplay(&g_sContext);
    Graphics_drawStringCentered(&g_sContext, "PAUSE", AUTO_STRING_LENGTH, 48, 15, TRANSPARENT_TEXT);
    Graphics_drawStringCentered(&g_sContext, "* To Exit", AUTO_STRING_LENGTH, 48, 30, TRANSPARENT_TEXT);
    Graphics_drawStringCentered(&g_sContext, "# To Resume", AUTO_STRING_LENGTH, 48, 45, TRANSPARENT_TEXT);
    Graphics_flushBuffer(&g_sContext);
}

void drawPlayingScreen() {
    Graphics_clearDisplay(&g_sContext);
    Graphics_drawStringCentered(&g_sContext, "Now Playing:", AUTO_STRING_LENGTH, 48, 15, TRANSPARENT_TEXT);
    Graphics_drawStringCentered(&g_sContext, currentlyPlaying()->name, AUTO_STRING_LENGTH, 48, 30, TRANSPARENT_TEXT);
    Graphics_flushBuffer(&g_sContext);
}

void drawWelcomeScreen() {
    Graphics_clearDisplay(&g_sContext);
    Graphics_drawStringCentered(&g_sContext, "MSP430", AUTO_STRING_LENGTH, 48, 15, TRANSPARENT_TEXT);
    Graphics_drawStringCentered(&g_sContext, "HERO!", AUTO_STRING_LENGTH, 48, 30, TRANSPARENT_TEXT);
    Graphics_drawStringCentered(&g_sContext, "# TO START", AUTO_STRING_LENGTH, 48, 45, TRANSPARENT_TEXT);
    Graphics_flushBuffer(&g_sContext);
}

void drawFinishedScreen() {
    Graphics_clearDisplay(&g_sContext);
    if(getScore() > (currentlyPlaying()->length / 2)) {
        Graphics_drawStringCentered(&g_sContext, "GOOD JOB!", AUTO_STRING_LENGTH, 48, 15, TRANSPARENT_TEXT);
        Graphics_drawStringCentered(&g_sContext, "YOU GOT", AUTO_STRING_LENGTH, 48, 30, TRANSPARENT_TEXT);
        Graphics_drawStringCentered(&g_sContext, "MORE THAN 50%", AUTO_STRING_LENGTH, 48, 45, TRANSPARENT_TEXT);
    }
    else {
        Graphics_drawStringCentered(&g_sContext, "YOU SUCK!", AUTO_STRING_LENGTH, 48, 15, TRANSPARENT_TEXT);
        Graphics_drawStringCentered(&g_sContext, "YOU GOT", AUTO_STRING_LENGTH, 48, 30, TRANSPARENT_TEXT);
        Graphics_drawStringCentered(&g_sContext, "LESS THAN 50%", AUTO_STRING_LENGTH, 48, 45, TRANSPARENT_TEXT);
    }

    Graphics_drawStringCentered(&g_sContext, "# To Return", AUTO_STRING_LENGTH, 48, 60, TRANSPARENT_TEXT);
    Graphics_flushBuffer(&g_sContext);
}

void drawStartingScreen() {
    // Set up so we can use TimerA2 to delay here
    TA2CTL |= TASSEL_1 | ID_0; // Setup Timer
    TA2CCTL0 &= ~CCIE; // stop interrupts
    TA2CCR0 = -1;

    TA2CTL |= MC_1;
    Graphics_clearDisplay(&g_sContext);
    Graphics_drawStringCentered(&g_sContext, "3", AUTO_STRING_LENGTH, 48, 15, TRANSPARENT_TEXT);
    Graphics_flushBuffer(&g_sContext);
    (TA2CTL |= TACLR);
    while(TA2R < 32768) {}
    Graphics_clearDisplay(&g_sContext);
    Graphics_drawStringCentered(&g_sContext, "2", AUTO_STRING_LENGTH, 48, 15, TRANSPARENT_TEXT);
    Graphics_flushBuffer(&g_sContext);
    (TA2CTL |= TACLR);
    while(TA2R < 32768) {}
    Graphics_clearDisplay(&g_sContext);
    Graphics_drawStringCentered(&g_sContext, "1", AUTO_STRING_LENGTH, 48, 15, TRANSPARENT_TEXT);
    Graphics_flushBuffer(&g_sContext);
    (TA2CTL |= TACLR);
    while(TA2R < 32768) {}
    Graphics_clearDisplay(&g_sContext);
    Graphics_drawStringCentered(&g_sContext, "GO!", AUTO_STRING_LENGTH, 48, 15, TRANSPARENT_TEXT);
    Graphics_flushBuffer(&g_sContext);
    (TA2CTL |= TACLR);
    while(TA2R < 32768) {}

    TA2CTL &= ~MC_1;
    TA2CCTL0 = CCIE; // reset timer for song
    TA2CTL |= TASSEL_1 | ID_0; // reset timer for song

    restartSong();
    setGuiState(PLAYING);
}

void setCurrentPage(uint8_t page) {
    currentPage = page;
    setGuiState(state);
}

void handlePauseKey(char key) {
    if(key == '#') {
        resumeSong();
        setGuiState(PLAYING);
    }

    if(key == '*') {
        stopSong();
        setGuiState(SONGLIST);
    }
}

void handleSonglistKey(char key) {
    if(key >= '0' && key <= '9') {
        int numSongs = sizeof(songList)/sizeof(Song*);
        if((currentPage * 10) + (key - '0') < numSongs) {
            setSong(songList[(currentPage * 10) + (key - '0')]);
            setGuiState(STARTING);
        }
    }

    if(key == '*' && currentPage != 0) {
        setCurrentPage(currentPage - 1);
    }

    if(key == '#' && currentPage != 255) {
        setCurrentPage(currentPage + 1);
    }
}

void handlePlayingKey(char key) {
    if(key == '#') {
        pauseSong();
        setGuiState(PAUSE);
    }
}

void handleWelcomeKey(char key) {
    if(key == '#') {
        setGuiState(SONGLIST);
    }
}

void handleGuiKey(char key) {
    switch(state) {
    case PAUSE:
        handlePauseKey(key);
        break;
    case WELCOME:
        handleWelcomeKey(key);
        break;
    case SONGLIST:
        handleSonglistKey(key);
        break;
    case PLAYING:
        handlePlayingKey(key);
        break;
    case FINISHED:
        handleWelcomeKey(key);
        break;
    default:
        break;
    }
}

void setGuiState(GUISTATE newState) {
    state = newState;

    switch(state) {
    case PAUSE:
        drawPauseScreen();
        break;
    case PLAYING:
        drawPlayingScreen();
        break;
    case SONGLIST:
        drawSongList();
        break;
    case WELCOME:
        drawWelcomeScreen();
        break;
    case FINISHED:
        drawFinishedScreen();
        break;
    case STARTING:
        drawStartingScreen();
        break;
    default:
        drawSongList();
        break;
    }
}
