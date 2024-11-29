#ifndef TETRIS_H
#define TETRIS_H
#include <curses.h>
#define TET_TICKS_START 30
#define WIDTH 10
#define HEIGHT 20


typedef struct TetBlock {
    int b;
} TetBlock;

typedef struct TetFigure {
    int x;
    int y;
    int size;
    TetBlock* blocks;
} TetFigure;

typedef struct TetFiguresT {
    int count;
    int size;
    TetBlock* blocks;
}TetFiguresT;

typedef struct TetField {
    int width;
    int height;
    TetBlock* blocks;
} TetField;
 
enum { //константы для статуса игры
    TET_GAMEOVER = 0,
    TET_PLAYING
};

enum { //константы действий игрока
    TET_PLAYER_NOP = 0,
    TET_PLAYER_UP,
    TET_PLAYER_DOWN,
    TET_PLAYER_LEFT,
    TET_PLAYER_RIGHT
    };

typedef struct TetPlayer{
    int action;
} TetPlayer;


typedef struct TetGame {
    TetField* field; // игровое поле и блоки на на нем
    TetFigure* figure; // падающая в данный момент фигура
    TetFiguresT* figurest; //типовые фигуры 
    TetPlayer* player; //взаимодействие с игроком
    int ticks_left; // кол-во тактов до перемещения
    int ticks; // кол-во тактов между перемещениями
    int playing; //статус игры
    int score; // очки 
    int high_score; // рекорд
    int number_template; //номер текущего шаблона
    int next_template; //номер следующего шаблона
    int level;

} TetGame;

TetGame* createTetGame(int field_width, int field_height,  int figure_size,
int count, TetBlock* figures_template, int score);
void freeTetGame(TetGame* tetg);

TetFigure* createTetFigure(TetGame* tetg);
void freeTetFigure(TetFigure* tf);

void freeTetFiguresT(TetFiguresT* tetft);

TetField* createTetField(int width, int height);
void freeTetField(TetField* tetf);


void moveFigureDown(TetGame* tetg);
void moveFigureUp(TetGame* tetg);
void moveFigureRight(TetGame* tetg);
void moveFigureLeft(TetGame* tetg);
int collusionTet(TetGame* tetg);
void plantFigure(TetGame* tetg);
int lineFilledTet(int i, TetField* tfl);
void dropLineTet(int i, TetField* tfl);
int eraseLinesTet(TetGame* tetg);
void dropNewFigure(TetGame* tetg);
TetFigure* rotTetFigure(TetGame* tetg);

void calculateTet(TetGame* tetg);
int convert_count_to_score(int count);


#endif