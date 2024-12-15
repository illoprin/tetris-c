#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "game.h"
#include <stdlib.h>
#include <string.h>

#include <time.h>
#ifdef _WIN32
	#include <windows.h>	
#else
	#include <unistd.h>
#endif

typedef struct GameClock {
	clock_t timeStart;
	clock_t timeEnd;
	double deltaTime;
	double fps;
} GameClock;

GameClock* initClock();
void handleInput(Game* game, GameClock* gclock, bool* started);
void startClock(GameClock* gclock);
void endClock(GameClock* gclock);
void releaseController(GameClock* gclock);
#endif