#ifndef INTERFACE_H
#define INTERFACE_H
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <ncurses.h>
#include "../../help_func.h"
#define WIDTH 10
#define HEIGHT 20
void init();
void DrawLines();
void DrawMenu();
void printNextFigure(GameInfo_t* gameInfo);
void printTetField(GameInfo_t* gameInfo);
void printTetFigure(TetGame* tetg);
void printPause(GameInfo_t* gameInfo);
#endif