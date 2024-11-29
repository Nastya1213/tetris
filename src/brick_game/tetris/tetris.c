#ifndef TETRIS_C
#define TETRIS_C
#include "tetris.h"
#include <stdlib.h>


TetFiguresT* createTetFiguresT(int count, int figures_size, TetBlock* figures_template){
    TetFiguresT* tetft = (TetFiguresT*)malloc(sizeof(TetFiguresT));
    tetft->count = count;
    tetft->size = figures_size;
    tetft->blocks =  figures_template;
    return tetft; 
};

void freeTetFiguresT(TetFiguresT* tetft){
     if(tetft){
        free(tetft);
     }
};

TetField* createTetField(int width, int height){
    TetField* tetf = (TetField*)malloc(sizeof(TetField));
    tetf->width = width;
    tetf->height = height;
    tetf->blocks = (TetBlock*)malloc(sizeof(TetBlock)*width*height);
    for (int i = 0; i < width * height; i++){
        tetf->blocks[i].b = 0;
    }
    return tetf;
};

void freeTetField(TetField* tetf){
    if (tetf){
        if(tetf->blocks){
            free(tetf->blocks);
        }
    free(tetf);
    } 
};


TetGame* createTetGame(int field_width, int field_height,  int figure_size,
int count, TetBlock* figures_template, int score){
    TetGame* tetg = (TetGame*)malloc(sizeof(TetGame)); 
    tetg->field = createTetField(field_width, field_height);
    tetg->figurest = createTetFiguresT(count, figure_size, figures_template);
    tetg->ticks = TET_TICKS_START;
    tetg->ticks_left = TET_TICKS_START;
    tetg->score = 0;
    tetg->high_score = score;
    tetg->playing = 1;
    tetg->level = 1;
    tetg->next_template = rand() % tetg->figurest->count;
    dropNewFigure(tetg); //выброс фигуры
    return tetg;
    
};

void freeTetGame(TetGame* tetg){
    if (tetg){
         freeTetField(tetg->field);
         freeTetFiguresT(tetg->figurest);
         free(tetg); 
    }
};


void moveFigureDown(TetGame* tetg){ //перемещение фигуры вниз
    if (tetg->playing == TET_GAMEOVER) return;
    tetg->figure->y++;
};
void moveFigureUp(TetGame* tetg){ //перемещение фигуры вверх
    if (tetg->playing == TET_GAMEOVER) return;
    tetg->figure->y--;
};
void moveFigureRight(TetGame* tetg){ //перемещение фигуры вправо
    if (tetg->playing == TET_GAMEOVER) return;
    tetg->figure->x++;
};
void moveFigureLeft(TetGame* tetg){ //перемещение фигуры влево
    if (tetg->playing == TET_GAMEOVER) return;
    tetg->figure->x--;
};
int collusionTet(TetGame* tetg){ //проверка на столкновения
    TetFigure* t = tetg->figure;
    TetField* tf = tetg->field;
    for (int i = 0; i < t->size; i++)
        for (int j = 0; j < t->size; j++){
            //y=i  x=j
            if (t->blocks[i*t->size+j].b != 0){     
                int fx = t->x + j;
                int fy = t->y + i;
                if (tf->blocks[fy*tf->width+fx].b != 0) //в том же самом месте есть ли блок
                    return 1;
                if (fx < 0 || fx >= tf->width || fy < 0 || fy >= tf->height) // за рамки выход
                    return 1;
            };
        };
    return 0; //если нет столеновений 0
};

void plantFigure(TetGame* tetg){ //фигура после падения
    TetFigure* t = tetg->figure;
    for (int i = 0; i < t->size; i++)
        for (int j = 0; j < t->size; j++){
            //y=i  x=j
            if (t->blocks[i*t->size+j].b != 0){
                int fx = t->x + j;
                int fy = t->y + i;
                tetg->field->blocks[fy*tetg->field->width+fx].b = t->blocks[i*t->size+j].b;
            };
        };
};

int lineFilledTet(int i, TetField* tfl){ //проверка на заполненность строки
    for (int j = 0; j < tfl->width; j++)
        if(tfl->blocks[i*tfl->width+j].b == 0){ //если в строке найден 0
            return 0;
        }
    return 1;
};

void dropLineTet(int i, TetField* tfl){ //сдвиг блоков поля
    if (i == 0){
        for (int j = 0; j < tfl->width; j++) //нулевую строку просто очищаем
            tfl->blocks[j].b = 0;
    } else {
        for (int k = i; k > 0; k--)
            for (int j = 0; j < tfl->width; j++)
            tfl->blocks[k*tfl->width+j].b = tfl->blocks[(k - 1)*tfl->width+j].b;
        
    };
};

int eraseLinesTet(TetGame* tetg){ //очищаем заполненнные строки и добавляем очки
    TetField* tfl = tetg->field;
    int count = 0; //кол-во удаленных строк
    for (int i = tfl->height - 1; i >= 0; i--){ //начинаем с последней
        while (lineFilledTet(i, tfl)){ //пока текущая строка заполнена
            dropLineTet(i, tfl); // удаляем её со сдвигом вниз
            count++;
        };
    };
    return count;
};


TetFigure* createTetFigure(TetGame* tetg){
    TetFigure* t = (TetFigure*)malloc(sizeof(TetFigure));
    t->x = 0;
    t->y = 0;
    t->size = tetg->figurest->size;
    t->blocks = (TetBlock*)malloc(sizeof(TetBlock)*t->size*t->size);
    return t;
};

void freeTetFigure(TetFigure* tf){
    if(tf){
        if(tf->blocks)
            free(tf->blocks);
        free(tf);
    };
};

void dropNewFigure(TetGame* tetg){ //выброс новой фигуры
    tetg->number_template = tetg->next_template;
    TetFigure* t = createTetFigure(tetg); //создаем новую фигуру в памяти
    t->x = tetg->field->width/2 - t->size/2;
    t->y = 0;
    int fnum = tetg->number_template; //случайное число от 0 до кол-во шаблонов - 1
    for (int i = 0; i < t->size; i++)
        for(int j = 0; j < t->size; j++){
            t->blocks[i*t->size+j].b = tetg->figurest->blocks[fnum*t->size*t->size + 
            i*t->size + j].b; //скопируем блоки шалблона
        };
    freeTetFigure(tetg->figure);
    tetg->figure = t;
    tetg->next_template = rand() % tetg->figurest->count;
   
};

TetFigure* rotTetFigure(TetGame* tetg){
    TetFigure* t = createTetFigure(tetg);
    TetFigure* told = tetg->figure;
    t->x = told->x;
    t->y = told->y;
    for (int i = 0; i < t->size; i++)
        for(int j = 0; j < t->size; j++) //последний столбец старой фигурф = первая строка новой, 
        // предпоследний столбец старой = вторая строка новой и тд
            t->blocks[i*t->size+j].b = told->blocks[j*t->size+t->size-1-i].b;
    return t;
};

int convert_count_to_score(int count){ //преобразование кол-ва линий в очки
    switch(count){
        case 0: return 0;
        case 1: return 100;
        case 2: return 300;
        case 3: return 700;
        case 4: return 1500;
        default: return 1500;
    };
};



void calculateTet(TetGame* tetg){
    if (collusionTet(tetg)){
                tetg->playing = TET_GAMEOVER;
                return;
            };
    if(tetg->ticks_left <= 0){ //ticks_left - кол-во тактов до перемещения
        tetg->ticks_left = tetg->ticks; //ticks - кол-во тактов между перемещениями
        moveFigureDown(tetg);
        if (collusionTet(tetg)){
            moveFigureUp(tetg);  //передвигаем фигуру в исходное положение
            plantFigure(tetg); //переносим её блоки на поле
            tetg->score += convert_count_to_score(eraseLinesTet(tetg));
            if (tetg->score > tetg->high_score){
                tetg->high_score = tetg->score;
            }
            tetg->level = tetg->score/600 + 1; //вычисляем уровень
            tetg->ticks = TET_TICKS_START * (1 - (tetg->level - 1)*0.1); //уменьшаем кол-во тактов
            //tetg->score += eraseLinesTet(tetg); //добавляем к очкам кол-во строк заполненных блоками и удаляем их
            dropNewFigure(tetg); //выбрасываем новую фигуру
            
        };
    };
    switch(tetg->player->action){
        case TET_PLAYER_RIGHT:
            moveFigureRight(tetg);
            if (collusionTet(tetg)){
                moveFigureLeft(tetg);
            }
            break;
        case TET_PLAYER_LEFT:
            moveFigureLeft(tetg);
            if (collusionTet(tetg)){
                moveFigureRight(tetg);
            }
            break;
        case TET_PLAYER_DOWN:
            moveFigureDown(tetg);
            if (collusionTet(tetg)){
                moveFigureUp(tetg);
            }
            break;
        case TET_PLAYER_UP:{
            TetFigure* t = rotTetFigure(tetg); //фигура после поворота
            TetFigure* told = tetg->figure; // до поворота
            tetg->figure = t; //разворачиваем
            if (collusionTet(tetg)){ 
                tetg->figure = told; //возвращаем старую, если произошло столкновение
                freeTetFigure(t); //удаляем новую
            } else {
                freeTetFigure(told);}; //если все норм, удаляем старую
            };
            break;
        case TET_PLAYER_NOP:
        default: break;
    };
    tetg->ticks_left--;
};
#endif