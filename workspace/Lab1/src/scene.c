#include <scene.h>
#include <stdlib.h>
#include <heap.h>
#include "graphics.h"
#include "global.h"

void generateRandomAlien(Scene_t* scene) {
	if(scene->time % UPDATE_FREQ != 0 || scene->active_aliens > 4) {
		return;
	}

	Alien_t* alien = (Alien_t*)allocate(sizeof(Alien_t));

	alien->y = 0;
	alien->col = ((rand() % 5) + 1);
	alien->speed = scene->difficulty;

	addToList(&scene->aliens, alien);

	scene->active_aliens++;

	return scene;
}
