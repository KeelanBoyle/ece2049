/*
 * gameloop.c
 *
 *  Created on: Sep 7, 2022
 *      Author: arthur
 */

#include <inttypes.h>
#include "global.h"
#include "peripherals.h"
#include "grlib.h"

typedef struct Alien Alien;

struct Alien {
	Alien* next;
	uint8_t x, y, number;
	uint8_t* sprite;
};

Alien testAlien = {
		NULL,
		10,
		10,
		1,
		'1'
};

struct GameInfo {
	Alien* aliens;
	char key;
} gameInfo;

void pushAlien(Alien* alien) {
	alien->next = gameInfo.aliens;
	gameInfo.aliens = alien;
}

void getInputs() {
	// poll keys
	gameInfo.key = getKey();
};

void doUpdate() {
	Alien* iter = gameInfo.aliens;
	while(iter != NULL) {
		iter->y = iter->y + 1;
		iter = iter->next;
	}
};

void draw() {
	Graphics_clearDisplay(&g_sContext);

	Alien* iter = gameInfo.aliens;
	while(iter != NULL) {
		Graphics_drawStringCentered(&g_sContext, iter->sprite, 1, iter->x, iter->y, TRANSPARENT_TEXT);
		iter = iter->next;
	}

	Graphics_flushBuffer(&g_sContext);
};

void gameLoop() {
	getInputs();
	doUpdate();
	draw();
};

void initGameState() {
	gameInfo.aliens = NULL;

	pushAlien(&testAlien);
}
