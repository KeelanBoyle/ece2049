/*
 * gui.h
 *
 *  Created on: Sep 27, 2022
 *      Author: arthur
 */

#ifndef GUI_H
#define GUI_H

#include <inttypes.h>
#include "../../../../../embedded_Syst/HW2/Lab2/src/musicplayer.h"

typedef enum GUISTATE {
    WELCOME,
    SONGLIST,
    PLAYING,
    PAUSE,
    FINISHED,
    STARTING
} GUISTATE;

uint8_t getCurrentPage();
void setCurrentPage(uint8_t);

void setGuiState(GUISTATE state);
void handleGuiKey(char key);

void drawSongList(void);

#endif
