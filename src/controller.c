#ifndef CONTROLLER_C
#define CONTROLLER_C

#include "controller.h"
#include <ncurses.h>

GameClock* initClock()
{
	GameClock* gameClock = (GameClock*)malloc(sizeof(GameClock));
	gameClock->fps = 0;
	gameClock->deltaTime = 0;
	return gameClock;
}

void startClock(GameClock* gclock)
{
	gclock->timeStart = clock();
}

void endClock(GameClock* gclock)
{
	gclock->timeEnd = clock();
	gclock->deltaTime = (((double)gclock->timeEnd - (double)gclock->timeStart) / (double)CLOCKS_PER_SEC) / 1000.0;
	gclock->fps = gclock->deltaTime == 0.0 ? 999.0 : 1.0 / gclock->deltaTime;

	/* Limit to 120 FPS */
	int targetDelta = (int)((0.0083 - gclock->deltaTime)*1000.0);
	if (gclock->deltaTime < 1)
	{
		#ifdef _WIN32
			/* Windows */
			Sleep(targetDelta);
		#else
			/* UNIX */
			usleep((__useconds_t)(targetDelta * 1000)); /* Sleep interval in MICROSECONDS*/
		#endif
	}
}

void handleInput(Game* game, GameClock* gclock, bool* started)
{
	/* 10 - Enter */
	/* 32 - Space */
	/* 114 - r */
	/* 27 - Escape */

	/* Get input key index */
	unsigned int in = getch();
    if (in != ERR)
    {
        switch (in)
        {
            case 27: /* Escape key */
                *started = false;
            break;
            case 32: /* Spacebar */
                if (game->state == TET_PLAYING)
                    game->state = TET_PAUSED;
                else if (game->state == TET_PAUSED)
                    game->state = TET_PLAYING;
            break;
            case 114: /* Reload game */
                if (game->state == TET_PLAYING || game->state == TET_GAME_OVER)
                    reloadGame(game);
            break;
            case 10: /* Enter key */
                if (game->state == TET_MAIN_MENU)
                    game->state = TET_PLAYING;
            break;

			/* Game events*/
			case 116: /* t key - Mirror figure */
				game->player->action = PLAYER_ROTATE;
			break;
			case 259:
				game->player->action = PLAYER_ROTATE;
			break;
			case 258:
				game->player->action = PLAYER_DOWN;
			break;
			case 260:
				game->player->action = PLAYER_LEFT;
			break;
			case 261:
				game->player->action = PLAYER_RIGHT;
			break;
			default:
				game->player->action = PLAYER_NOP;
			break;
        }
    }
}

void releaseController(GameClock* gclock)
{
	if (gclock) free(gclock);
}
#endif