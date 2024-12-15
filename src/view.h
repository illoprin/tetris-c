#ifndef VIEW_H
#define VIEW_H
#include "game.h"
#include <stdlib.h>

void initView();
void render(Game* game, GameClock* gclock);
void releaseView();

#endif
