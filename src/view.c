#ifndef VIEW_C
#define VIEW_C

#include "controller.h"
#include "view.h"
#include <ncurses.h>

void initView()
{
	/* Init ncurses screen */
	initscr();
	start_color();
	init_pair(1, COLOR_RED, COLOR_BLACK); /* Game over sheme */
	init_pair(2, COLOR_BLACK, COLOR_WHITE); /* Block Field sheme */
	init_pair(3, COLOR_WHITE, COLOR_BLACK); /* Empty field sheme */
	init_pair(4, COLOR_BLUE, COLOR_WHITE); /* Score sheme */

	/* Raw input - read */
	raw();
	keypad(stdscr, TRUE);
	nodelay(stdscr, TRUE);
	scrollok(stdscr, TRUE);
	curs_set(0);
	noecho();
}

void releaseView()
{
	endwin();
}

void renderMenuScreen()
{
	attron(A_BOLD);
	mvprintw(stdscr->_maxy / 2, stdscr->_maxx / 2 - 10, "Press Enter to start");
	attroff(A_BOLD);
}

void renderGameField(Game* game)
{
	Figure* fig = game->figure;
	Field* fld = game->field;

	/* Render field and active figure */
	unsigned mx = stdscr->_maxx / 2 - (fld->width * 2) / 2, my = stdscr->_maxy / 2 - fld->height / 2; /* Margin xy */
	unsigned char type, sum;
	unsigned fbx, fby;
	for (unsigned i = 0; i < fld->height; i++)
		for (unsigned j = 0; j < fld->width; j++)
		{
			type = (unsigned char)fld->blocks[j + i * fld->width].type;
			sum = 0;
			if (type != 0)
				sum++;
			else {
				fbx = j - fig->x;
				fby = i - fig->y;
				if (fbx >= 0 && fbx < fig->size && fby >= 0 && fby < fig->size)
					if (fig->blocks[fbx + fby * fig->size].type != 0)
						sum++;
			}
			if (sum > 0)
			{
				attron(COLOR_PAIR(2));
				mvaddch(i + my, j * 2 + mx, ' ');
				attroff(COLOR_PAIR(2));
			}
			else
			{
				attron(COLOR_PAIR(3));
				mvaddch(i + my, j * 2 + mx, '.');
				attroff(COLOR_PAIR(3));
			}
		}
	/* Render score */
	attron(COLOR_PAIR(4));
	attron(A_BOLD);
		Player* plr = game->player;
		unsigned px = mx + fld->width*2, py = my; /* Panel xy */
		mvprintw(py, px, "Score: %ld", plr->score);
		mvprintw(py + 1, px, "Lines: %d", game->lines);
		mvprintw(py + 2, px, "Tetris rate: %.2f", plr->tetrisRate);
		mvprintw(py + 3, px, "Level: %d", plr->level);
	attroff(A_BOLD);
	attroff(COLOR_PAIR(4));
}

void renderPauseScreen()
{
	attron(A_BOLD);
	mvprintw(stdscr->_maxy / 2, stdscr->_maxx / 2 - 10, "Press SPACE to resume");
	attroff(A_BOLD);
}

void renderEndScreen(Game* game)
{
	Player* player = game->player;
	attron(A_BOLD);
	attron(COLOR_PAIR(1));
	mvprintw(stdscr->_maxy / 2, stdscr->_maxx / 2 - 5, "Game over!");
	/* TODO: print score */
	attroff(COLOR_PAIR(1));
	mvprintw(stdscr->_maxy / 2 + 1, stdscr->_maxx / 2 - 3, "Score: %ld", player->score);
	attroff(A_BOLD);
}

void render(Game* game, GameClock* gclock)
{
	clear();
	switch (game->state)
	{
		case TET_PLAYING:
			renderGameField(game);
		break;
		case TET_MAIN_MENU:
			renderMenuScreen();
		break;
		case TET_GAME_OVER:
			renderEndScreen(game);
		break;
		case TET_PAUSED:
			renderPauseScreen();
		break;
	}
	/* 	Print current FPS
		attron(A_BOLD);
		mvprintw(0, 0, "FPS: %.0f", gclock->fps);
		mvprintw(1, 0, "GAME_STATE: %d", game->state);
		attroff(A_BOLD);
	*/

	/* Swap buffers */
	refresh();
}


#endif