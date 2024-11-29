#ifndef TETRIS_H
#define TETRIS_H

#include <curses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#define WIDTH 10
#define HEIGHT 20
#define O_FIGURE_TYPE 0


typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
} UserAction_t;

typedef struct {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;  // 0 active 1 pause 2 exit 3 wait to start
} GameInfo_t;

typedef struct {
  int figureArr[4][4];
  int type;
  int nextType;
  int cordX, cordY;
} Figure_t;

void userInput(GameInfo_t *game, Figure_t *figure, UserAction_t action);
int initializeField(GameInfo_t *game);
int canMoveDown(Figure_t *figure, GameInfo_t *game);
void movePieceDown(Figure_t *figure, GameInfo_t *game);
void fixPiece(Figure_t *figure, GameInfo_t *game);
void spawnNewPiece(Figure_t *figure, GameInfo_t *game);
bool canRotate(Figure_t *figure, GameInfo_t *game, int tempFigure[4][4]);
int canMove(Figure_t *figure, GameInfo_t *game, int dx, int dy);
void rotateFigure(Figure_t *figure, GameInfo_t *game);
void checkRow(GameInfo_t *game);
void moveField(GameInfo_t *game, int row);
void spawnFirstFig(Figure_t *figure, GameInfo_t *game);
void updateScore(GameInfo_t *game, int streak);
void checkGameOver(GameInfo_t *game);
void writeHighScore(int hscore);
void readHighScore(int *hscore);
void freeMemory(GameInfo_t *game, int f, int n);
void fallTime(struct timeval *lastFallTime, struct timeval *currentTime,
              GameInfo_t *game, Figure_t *figure);

#endif  // TETRIS_H