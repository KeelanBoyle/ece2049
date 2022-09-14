/*
 * gameloop.c
 *
 *  Created on: Sep 7, 2022
 *      Author: arthur
 */

#include <inttypes.h>
#include <msp430f5529.h>
#include "global.h"
#include "peripherals.h"
#include "heap.h"
#include "game.h"
#include "grlib.h"
#include "list.h"
#include "graphics.h"

#define TEXT_OFFSET_OF_COL(x) (x * 16)
#define IMAGE_OFFSET_OF_COL(x) (88 - (x * 16))

Scene_t* activeScene;

char lastKey = 0;

void getInputs() {
	char key = getKey();
	if(key) {
		lastKey = key;
	}
};

void drawUI() {
    Graphics_clearDisplay(&g_sContext);
    Graphics_drawStringCentered(&g_sContext, "1", 1, TEXT_OFFSET_OF_COL(1), 90 , TRANSPARENT_TEXT);
    Graphics_drawStringCentered(&g_sContext, "2", 1, TEXT_OFFSET_OF_COL(2), 90, TRANSPARENT_TEXT);
    Graphics_drawStringCentered(&g_sContext, "3", 1, TEXT_OFFSET_OF_COL(3), 90, TRANSPARENT_TEXT);
    Graphics_drawStringCentered(&g_sContext, "4", 1, TEXT_OFFSET_OF_COL(4), 90, TRANSPARENT_TEXT);
    Graphics_drawStringCentered(&g_sContext, "5", 1, TEXT_OFFSET_OF_COL(5), 90, TRANSPARENT_TEXT);
    Graphics_flushBuffer(&g_sContext);
}

void updateActiveScene() {
	/* Shoot bullet if button pressed*/
	if(lastKey >= '1' && lastKey <= '5' && activeScene->active_bullets < 5) {
		Bullet_t* bullet = (Bullet_t*)allocate(sizeof(Bullet_t));
		bullet->y = UNITS_Y - 16;
		bullet->col = lastKey - '0';
		addToList(&activeScene->bullets, (ListHandle_t*)bullet);
		activeScene->active_bullets++;

		lastKey = 0;
	}

	/* Generate new aliens */
	activeScene->generator(activeScene);
	Alien_t* aIter = (Alien_t*)activeScene->aliens.next;

	Bullet_t* bIter = (Bullet_t*)activeScene->bullets.next;
	while(bIter != NULL) {
		bIter->y -= 8;
		bIter = (Bullet_t*)bIter->lh.next;
	}

	/* Update positions */
	while(aIter != NULL) {
		aIter->y = aIter->y + (aIter->speed);

		if(aIter->y > UNITS_Y - 8) {
			g_State = GAME_OVER;
		}

		/* Check for bullet hits */
		bIter = (Bullet_t*)activeScene->bullets.next;
		while(bIter != NULL) {
			if(bIter->col == aIter->col && bIter->y < aIter->y) {
				removeFromList((ListHandle_t*)bIter);
				removeFromList((ListHandle_t*)aIter);
				free((void*)aIter);
				free((void*)bIter);

				activeScene->active_aliens--;
				activeScene->active_bullets--;
				activeScene->difficulty++;

				break;
			}

			bIter = (Bullet_t*)bIter->lh.next;
		}

		aIter = (Alien_t*)aIter->lh.next;
	}

	activeScene->time++;
};

void drawActiveScene() {
    // Draw UI At Bottom.
    drawUI();

	Alien_t* alienIter = (Alien_t*)activeScene->aliens.next;
	while(alienIter != NULL) {
		Graphics_drawImage(&g_sContext, &Alien_Sprite,  alienIter->y, IMAGE_OFFSET_OF_COL(alienIter->col));
		alienIter = (Alien_t*)alienIter->lh.next;
	}

	Bullet_t* bulletIter = (Bullet_t*)activeScene->bullets.next;
    while(bulletIter != NULL) {
        Graphics_drawImage(&g_sContext, &Bullet_Sprite, bulletIter->y, IMAGE_OFFSET_OF_COL(bulletIter->col));
        bulletIter = (Bullet_t*)bulletIter->lh.next;
    }

	Graphics_flushBuffer(&g_sContext);
};

void gameLoop() {
	volatile uint16_t timer = TBR;
	if(timer > FRQ_TO_TMR(UPDATE_FREQ)) {
		CLEAR_TMRB;
		updateActiveScene();
		drawActiveScene();
	} else {
		getInputs();
	}
};

void setActiveScene(Scene_t* scene) {
	activeScene = scene;
}


void initGameState() {

	Scene_t* scene = allocate(sizeof(Scene_t));
	scene->active_aliens = 0;
	scene->active_bullets = 0;
	scene->aliens.next = NULL;
	scene->bullets.next = NULL;
	scene->generator = generateRandomAlien;
	scene->difficulty = 1;

	setActiveScene(scene);
}
