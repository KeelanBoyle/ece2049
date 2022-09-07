/*
 * main.h
 *
 *  Created on: Sep 7, 2022
 *      Author: arthur
 */

#ifndef MAIN_H_
#define MAIN_H_

typedef enum State {
    MAIN_MENU,
    GAME_LOOP,
    GAME_OVER
} State;

extern State g_State;

#endif /* MAIN_H_ */
