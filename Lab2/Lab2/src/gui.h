/*
 * gui.h
 *
 *  Created on: Sep 27, 2022
 *      Author: arthur
 */

#ifndef GUI_H
#define GUI_H

#include <musicplayer.h>
#include <inttypes.h>

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
