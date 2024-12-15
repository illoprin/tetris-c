#ifndef TETRIS_H
#define TETRIS_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define TICKS_START 80.0

typedef struct Block
{
	unsigned int type;
} Block;

typedef struct Figure
{
	int x;
	int y;
	unsigned int size;
	Block* blocks;
} Figure;


typedef struct FiguresT
{
	unsigned int count;
	unsigned int size;
	Block* blocks;
} FiguresT;
FiguresT* createFiguresTemplate(
	unsigned int count, 
	unsigned int figureSize, 
	Block* blockTemplate
);
void releaseTemplates(FiguresT* obj);

typedef struct Field
{
	unsigned int width;
	unsigned int height;
	Block* blocks;
} Field;


/* Player actions */
enum PlayerAction
{
	PLAYER_NOP = 0,
	PLAYER_ROTATE,
	PLAYER_DOWN,
	PLAYER_LEFT,
	PLAYER_RIGHT
};

typedef struct Player
{
	enum PlayerAction action;
	unsigned level;
	unsigned long score;
	double tetrisRate;
	unsigned ticksLeft;
} Player;

/* Game states */
enum GameState
{
	TET_GAME_OVER = 0,
	TET_PLAYING, TET_MAIN_MENU, TET_PAUSED
};

typedef struct Game
{
	Field* field;

	Figure* figure;
	Figure* next;

	FiguresT* figuresTemplate;
	Player* player;
	int* scoreTable;
	/* TETRIS - Four lines deleted at the same time */
	unsigned int lines, tetris;
	enum GameState state;
} Game;

Game* createGame (
	unsigned fieldWidth,
	unsigned fieldHeight,
	unsigned figureSize,
	unsigned char count,
	Block* figuresTemplate,
	int* scoreTable
);
void releaseGame(Game* game);
void reloadGame(Game* game);

/* Update functions */
void handlePlayerEvents(Game* game);
void updateGame(Game* game);
void tick(Game* game);

/* Game field functions */
void placeFigure(Game* game);
void updateActive(Game* game);
bool hasCollision(Game* game, Figure* figure);

/* Figure */
Figure* createNewFigure(Game* game);
Figure* createFigureFromTemplate(Game* game);
void releaseFigure(Figure* figure);

/* Field */
Field* createField(unsigned int width, unsigned int height);
void releaseField(Field* field);

bool lineFilled(unsigned y, Field* field);
void clearLine(unsigned y, Field* field);
unsigned clearLines(Game* game);
void updateStatements(Game* game, int count);

/* Player */
Player* initPlayer(unsigned startLevel, unsigned ticksLeftStart);
void releasePlayer(Player* player);

#endif
