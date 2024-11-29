#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "../../brick_game/tetris/tetris.h"

void displayField(GameInfo_t *game, Figure_t *figure);
void displayBorder();
void displayStat(GameInfo_t *game);
void init();
void gameProccess(Figure_t *figure, GameInfo_t *game);
void gameOver();
void startMenu(GameInfo_t *game, Figure_t *figure);

#endif  // GRAPHICS_H