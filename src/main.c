#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "controller.h"
#include "view.h"

#pragma GCC diagnostic ignored "-Wmissing-braces"

Block templates[] = {
	0, 0, 1, 0,
	0, 0, 1, 0,
	0, 0, 1, 0,
	0, 0, 1, 0,

	0, 0, 0, 0,
	0, 2, 2, 0,
	2, 2, 0, 0,
	0, 0, 0, 0,

	0, 0, 0, 0,
	0, 3, 3, 0,
	0, 0, 3, 3,
	0, 0, 0, 0,

	0, 0, 0, 0,
	0, 4, 0, 0,
	4, 4, 4, 0,
	0, 0, 0, 0,

	0, 5, 0, 0,
	0, 5, 0, 0,
	0, 5, 5, 0,	
	0, 0, 0, 0,

	0, 0, 6, 0,
	0, 0, 6, 0,
	0, 6, 6, 0,
	0, 0, 0, 0,

	0, 0, 0, 0,
	0, 7, 7, 0,
	0, 7, 7, 0,
	0, 0, 0, 0
};
int scoreTable[] = {40, 100, 300, 2450};

int main(int argc, char* argv[])
{
	srand(63748); /* Init random */
	/* Init player */
	Player* player = initPlayer(1, TICKS_START);
	Game* game = createGame(10U, 20U, 4U, 7U, templates, scoreTable);
	game->player = player;
	GameClock* clock = initClock();

	bool started = true;
	unsigned ticks = 0;
	initView();
	while (started)
	{
		ticks++;
		startClock(clock);
		handleInput(game, clock, &started);
		if (game->state == TET_PLAYING)
		{
			/* DEBUG: Select random player action */
			/* Select random action */
			// game->player->action = ticks % 5 == 0 ? rand() % 5 : game->player->action;
			handlePlayerEvents(game);
			if (ticks % player->ticksLeft == 0)
				updateGame(game);
		}
		render(game, clock);
		endClock(clock);
	}
	releaseView();
	releaseGame(game);
	releasePlayer(player);
	releaseController(clock);

	return 0;
}
