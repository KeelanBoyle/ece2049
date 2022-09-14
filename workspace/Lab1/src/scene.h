#ifndef SCENE_H
#define SCENE_H

#include "list.h"
#include "inttypes.h"
#include "grlib.h"

typedef struct Scene_t Scene_t;

struct Scene_t {
	List_t aliens;
	uint8_t active_aliens;
	List_t bullets;
	uint8_t active_bullets;
	uint8_t difficulty;
	uint32_t time;
	void (*generator)(Scene_t*);
};

typedef struct Alien_t {
	ListHandle_t lh;
	uint16_t col;
	uint16_t y, speed; // Speed measured in units/update
} Alien_t;

typedef struct Bullet_t {
	ListHandle_t lh;
	uint16_t col;
	uint16_t y;
} Bullet_t;

void generateRandomAlien(Scene_t* scene);

#endif
