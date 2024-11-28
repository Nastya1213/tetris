#ifndef HELP_FUNC_H
#define HELP_FUNC_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "brick_game/tetris/tetris.h"

typedef struct {
    int **field;
    int **next;
    int score;
    int high_score;
    int level;
    int speed;
    int pause;
} GameInfo_t;

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

int load_high_score(const char *filename);
void save_high_score(const char *filename, int high_score);
GameInfo_t updateCurrentState(TetGame* tetGame, int pause);
int** copyField(TetField* tetField);
void freeGameInfo(GameInfo_t* gameInfo, TetField* field, TetFiguresT* figurest);
void userInput(UserAction_t action, bool hold, TetPlayer* player, int* pause);

#endif