#ifndef TETRIS_C
#define TETRIS_C

#include "game.h"

/*
 * FiguresT
 * Constructor
 */
FiguresT* createFiguresTemplate(
	unsigned int count, 
	unsigned int figureSize, 
	Block* blockTemplate
)
{
	FiguresT* templates = (FiguresT*)malloc(sizeof(FiguresT));

	templates->count = count;
	templates->size = figureSize;
	templates->blocks = blockTemplate;
	printf("game.c: GameFiguresTemplate created!\n");

	return templates;
};
/*
 * FiguresT
 * Deconstructor
 */
void releaseTemplates(FiguresT* obj)
{
	if (obj)
	{
		free(obj);
	}
};

/*
 * Field
 * Constructor
 */
Field* createField(unsigned int width, unsigned int height)
{
	Field* field = (Field*)malloc(sizeof(Field));
	field->width = width;
	field->height = height;
	field->blocks = (Block*)malloc(sizeof(Block) * width * height);
	for (unsigned i = 0; i < width * height; i++)
		field->blocks[i].type = 0;
	if (field->blocks == NULL) {
		fprintf(stderr, "game.c: ERROR Cannot alloc memory for Block struct on GameField!\n");
		exit(EXIT_FAILURE);
	}
	printf("game.c: Field created!\n");
	

	return field;
};

/*
 * Field
 * Deconstructor
 */
void releaseField(Field* field)
{
	if (field)
	{
		if (field->blocks) free(field->blocks);
		free(field);
	}
};

/*
 * Player
 * Constructor
 */
Player* initPlayer(unsigned startLevel, unsigned ticksLeftStart)
{
	Player* player = (Player*)malloc(sizeof(Player));
	player->action = PLAYER_NOP;
	player->level = startLevel;
	player->score = 0;
	player->tetrisRate = 0.0;
	player->ticksLeft = ticksLeftStart;
	return player;
}


/*
 * Player
 * Deconstructor
 */
void releasePlayer(Player* player)
{
	if (player) free(player);
}


/*
 * Game
 * Constructor
 */
Game* createGame (
	unsigned fieldWidth,
	unsigned fieldHeight,
	unsigned figureSize,
	unsigned char count,
	Block* figuresTemplate,
	int* scoreTable
)
{
	Game* tetris = (Game*)malloc(sizeof(Game));
	if (tetris == NULL)
	{
		fprintf(stderr, "game.c: ERROR Cannot alloc memory for Game struct.\n");
		exit(EXIT_FAILURE);
	}
	tetris->field = createField(fieldWidth, fieldHeight);
	tetris->figuresTemplate = createFiguresTemplate((unsigned int)count, figureSize, figuresTemplate);
	tetris->scoreTable = scoreTable;
	
	tetris->lines = 0; tetris->tetris = 0;
	tetris->state = TET_MAIN_MENU;

	tetris->figure = createFigureFromTemplate(tetris);
	tetris->next = createFigureFromTemplate(tetris);
	printf("game.c: Game created!\n");

	return tetris;
};
void reloadGame(Game* game)
{
	/* Empty field */
	for (unsigned i = 0; i < game->field->width * game->field->height; i++)
		game->field->blocks[i].type = 0;
	
	releaseFigure(game->figure);
	releaseFigure(game->next);
	
	game->figure = createFigureFromTemplate(game);
	game->next = createFigureFromTemplate(game);

	game->player->score = 0;
	game->player->level = 1;
	game->player->ticksLeft = TICKS_START;
	game->player->tetrisRate = 0.0;
	
	game->state = TET_PLAYING;
}
/*
 * Game
 * Deconstructor
 */
void releaseGame(Game* game)
{
	if (game)
	{
		releaseFigure(game->figure);
		releaseFigure(game->next);
		releaseField(game->field);
		releaseTemplates(game->figuresTemplate);
		free(game);
	}	
};

/*
 * Figure
 * Constructor
*/
Figure* createNewFigure(Game* game)
{
	Figure* figure = (Figure*)malloc(sizeof(Figure));
	if (!figure)
	{
		fprintf(stderr, "game.c: ERROR Cannot alloc memory for Figure.\n");
		exit(EXIT_FAILURE);
	}
	figure->x = 0;
	figure->y = 0;
	figure->size = game->figuresTemplate->size;
	figure->blocks = (Block*)malloc(
		sizeof(Block) * figure->size * figure->size
	);
	return figure;
}
Figure* createFigureFromTemplate(Game* game)
{
	Figure* figure = createNewFigure(game);
	figure->x = game->field->width / 2 - figure->size / 2;
	int offset = rand() % game->figuresTemplate->count;
	int tplIndex;
	for (int i = 0; i < figure->size; i++)
		for (int j = 0; j < figure->size; j++)
		{
			tplIndex = offset * (figure->size * figure->size) + (j + i * figure->size);
			figure->blocks[j + i * figure->size].type = game->figuresTemplate->blocks[tplIndex].type;
		}

	return figure;
}

/*
 * Figure
 * Deconstructor
*/
void releaseFigure(Figure* figure)
{
	if (figure)
	{
		if (figure->blocks)
			free(figure->blocks);
		free(figure);
	}
}

/* ====== Movement functions ====== */
void moveActiveFigureDown(Game* game)
{
	game->figure->y += 1;
	if (hasCollision(game, game->figure))
	{
		game->figure->y -= 1;
		placeFigure(game);
		unsigned count = clearLines(game);
		updateStatements(game, count);
		updateActive(game);
		if (hasCollision(game, game->figure))
		{
			game->state = TET_GAME_OVER;
			return;
		}
	}
};

void moveActiveFigureUp(Game* game)
{
	game->figure->y += 1;
	if (hasCollision(game, game->figure))
		game->figure->y -= 1;
};

void moveActiveFigureH(Game* game, char delta)
{
	game->figure->x += delta;
	if (hasCollision(game, game->figure)) game->figure->x -= delta;
};

void rotateActiveFigure(Game* game, char clockwise)
{
	Figure* tmp = createNewFigure(game);
	/* Create copy of active figure */
	for (int i = 0; i < tmp->size * tmp->size; i++)
	{
		tmp->blocks[i].type = game->figure->blocks[i].type;
	}
	tmp->x = game->figure->x;
	tmp->y = game->figure->y;

	/* Rotate active game figure */
	int size = game->figure->size;
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
		{
			/* i is y | j is x */
			if (!clockwise) /* Counterclockwise */
				game->figure->blocks[(size - j - 1) + i * size].type =
					tmp->blocks[(size - i - 1) + j * size].type;
			else /* Clockwise */
				game->figure->blocks[j + i * size].type =
					tmp->blocks[(size - i - 1) + j * size].type;

		}
	/*
		Logic:
			if not collision:
				release memory for tmp figure -> active figure rotated
			if collision:
				delete rotated, assign temp figure to active figure
	*/
	if (!hasCollision(game, game->figure))
		releaseFigure(tmp);
	else {
		releaseFigure(game->figure);
		game->figure = tmp;
	}
};
/* ================================ */

bool hasCollision(Game* game, Figure* figure)
{
	Field* field = game->field;

	for (int i = 0; i < figure->size; i++)
		for (int j = 0; j < figure->size; j++)
		{
			int block = figure->blocks[j + i * figure->size].type;
			if (block != 0)
			{
				int bx = figure->x + j;
				int by = figure->y + i;
				if (bx < 0 || bx >= field->width || by < 0 || by >= field->height)
					return true;	
				if (field->blocks[bx + by * field->width].type != 0)
					return true;
			}
		}

	return false;
}

/* Update score, level and tetris rate */
void updateStatements(Game* game, int count)
{
	if (count > 0)
	{
		Player* player = game->player;
		/* Add cleared lines to total */
		game->lines += count;
		/* Update tetris count */
		game->tetris += count >= 4 ? 1 : 0;
		
		/* Update score */
		player->score += game->scoreTable[count - 1];

		/* Compute tetris-rate */
		player->tetrisRate = (double)(game->tetris * 4) / (double)fmax(game->lines, 1);

		/* Goto new level logic */
		const unsigned linesPerLevel = 12;
		int newLevel = floor((double)game->lines / (double)linesPerLevel);
		if (newLevel != player->level) {
			player->level = newLevel != 0 ? newLevel : player->level;
			/* Update delay (ticksLeft value) */
			double speedFactor = pow(0.99, player->level);
			player->ticksLeft = (unsigned)(TICKS_START * speedFactor);
		}
	}
}

void placeFigure(Game* game)
{
	Field* field = game->field;
	Figure* figure = game->figure;
	int size = figure->size;
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
		{
			if (figure->blocks[j + i * size].type != 0)
			{
				int bx = j + figure->x; /* Block x */
				int by = i + figure->y; /* Block y */
				int index = bx + by * field->width;
				if (index < field->width * field->height)
					field->blocks[index].type = figure->blocks[j + i * size].type;
			}	
		}
}

void updateActive(Game* game)
{
	Figure* next = createFigureFromTemplate(game);
	releaseFigure(game->figure);
	game->figure = game->next;
	game->next = next;
}

/* ======== ClearLine Logic ======== */
bool lineFilled(unsigned y, Field* field)
{
	for (int j = 0; j < field->width; j++)
	{
		if (field->blocks[j + y * field->width].type == 0)
			return false;
	}
	return true;
}

void clearLine(unsigned y, Field* field)
{
	if (y == 0)
	{
		for (unsigned j = 0; j < field->width; j++)
			field->blocks[j + y * field->width].type = 0;
	}
	else
	{
		for (unsigned i = y; i > 0; i--)
		{
			for (unsigned j = 0; j < field->width; j++)
				field->blocks[j + i * field->width].type = field->blocks[j + (i - 1) * field->width].type;
		}
	}
}

unsigned clearLines(Game* game)
{
	Field* field = game->field;
	int count = 0;
	for (int i = field->height - 1; i >= 0; i--)
	{
		while (lineFilled(i, field))
		{
			clearLine(i, field);
			count++;
		}
	}
	return count;
}
/* ================================ */



/* ========= OnUpdate Functions ======= */
void tick(Game* game)
{
	handlePlayerEvents(game);
	updateGame(game);
};

void handlePlayerEvents(Game* game)
{
	Player* player = game->player;
	switch(player->action)
	{
		/* All move functions includes collision detection */
		case PLAYER_ROTATE:
			rotateActiveFigure(game, 1);
		break;
		case PLAYER_LEFT:
			moveActiveFigureH(game, -1);
		break;
		case PLAYER_RIGHT:
			moveActiveFigureH(game, 1);
		break;
		case PLAYER_DOWN:
			moveActiveFigureDown(game);
		break;
		default: break;
	}
	game->player->action = PLAYER_NOP;
};

void updateGame(Game* game)
{
	/* Move active figure vertically */
	moveActiveFigureDown(game);
};
/* ==================================== */

#endif