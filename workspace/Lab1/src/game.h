/*
 * gameloop.h
 *
 *  Created on: Sep 7, 2022
 *      Author: arthur
 */

#ifndef GAME_H_
#define GAME_H_

#include "scene.h"

#define UNITS_X 96
#define UNITS_Y 96

void setActiveScene(Scene_t* scene);
void initGameState();
void gameLoop();

#endif /* GAMELOOP_H_ */
