#include "gui.h"
#include "songs.h"
#include "grlib.h"
#include "peripherals.h"

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
    Graphics_flushBuffer(&g_sContext);

    setGuiState(SONGLIST);
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
            playSong(songList[(currentPage * 10) + (key - '0')]);
            setGuiState(PLAYING);
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

void handleGuiKey(char key) {
    switch(state) {
    case PAUSE:
        handlePauseKey(key);
        break;
    case WELCOME:
        break;
    case SONGLIST:
        handleSonglistKey(key);
        break;
    case PLAYING:
        handlePlayingKey(key);
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
    default:
        drawSongList();
        break;
    }
}
