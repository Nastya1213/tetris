#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <ncurses.h>
#include "brick_game/tetris/tetris.h"
#include "gui/cli/interface.h"
#include "help_func.h"

TetBlock tet_templates[] = {
    // 1
    {0}, {0}, {0}, {0}, {0},
    {0}, {0}, {1}, {0}, {0},
    {0}, {0}, {1}, {0}, {0},
    {0}, {0}, {1}, {0}, {0},
    {0}, {0}, {1}, {0}, {0},

    // 2
    {0}, {0}, {0}, {0}, {0},
    {0}, {0}, {0}, {0}, {0},
    {0}, {1}, {0}, {0}, {0},
    {0}, {1}, {1}, {1}, {0},
    {0}, {0}, {0}, {0}, {0},

    // 3
    {0}, {0}, {0}, {0}, {0},
    {0}, {0}, {0}, {0}, {0},
    {0}, {0}, {0}, {1}, {0},
    {0}, {1}, {1}, {1}, {0},
    {0}, {0}, {0}, {0}, {0},

    // 4
    {0}, {0}, {0}, {0}, {0},
    {0}, {0}, {0}, {0}, {0},
    {0}, {1}, {1}, {0}, {0},
    {0}, {1}, {1}, {0}, {0},
    {0}, {0}, {0}, {0}, {0},

    // 5
    {0}, {0}, {0}, {0}, {0},
    {0}, {0}, {1}, {1}, {0},
    {0}, {1}, {1}, {0}, {0},
    {0}, {0}, {0}, {0}, {0},
    {0}, {0}, {0}, {0}, {0},

    // 6
    {0}, {0}, {0}, {0}, {0},
    {0}, {1}, {1}, {0}, {0},
    {0}, {0}, {1}, {1}, {0},
    {0}, {0}, {0}, {0}, {0},
    {0}, {0}, {0}, {0}, {0},

    // 7
    {0}, {0}, {0}, {0}, {0},
    {0}, {0}, {1}, {0}, {0},
    {0}, {1}, {1}, {1}, {0},
    {0}, {0}, {0}, {0}, {0},
    {0}, {0}, {0}, {0}, {0}
};
void start_game(){
    int pause = 0;
    struct timespec sp_start, sp_end, ts1, ts2 = {0, 0};
    int high_score = load_high_score("record.txt");
    TetGame* tetg = createTetGame(10, 20,  5, 6, tet_templates, high_score);
    GameInfo_t gameInfo = updateCurrentState(tetg, pause);
    TetPlayer player;
    player.action = TET_PLAYER_NOP;
    tetg->player = &player;
    tetg->playing = 1;
    bool hold = 0;
    while(tetg->playing != TET_GAMEOVER){
        clock_gettime(CLOCK_MONOTONIC, &sp_start); //время начала
        int ch = getch();
        switch (ch){
            case ' ':
                userInput(Action, hold, &player, &pause);
                break;
            case KEY_DOWN :
                userInput(Down, hold, &player, &pause);
                break;
            case KEY_RIGHT :
                userInput(Right, hold, &player, &pause);
                break;
            case KEY_LEFT :
                userInput(Left, hold, &player, &pause);
                break;
            case 'q':
                tetg->playing = TET_GAMEOVER;
                break;
            case 'p':
                userInput(Pause, hold, &player, &pause);
            default:
                userInput(Start, hold, &player, &pause);
                break;
        };
        DrawLines();
        refresh();
        if (pause == 0){
            calculateTet(tetg);
        }
        gameInfo = updateCurrentState(tetg, pause); //обновляем информацию
        printTetField(&gameInfo);
        printTetFigure(tetg);
        printNextFigure(&gameInfo); //печаем следующую фигуру
        printPause(&gameInfo);
        clock_gettime(CLOCK_MONOTONIC, &sp_end); // время конца
        if (sp_end.tv_sec - sp_start.tv_sec <= 0 && 
        (ts2.tv_nsec = 33000000 - (sp_end.tv_nsec - sp_start.tv_nsec)) > 0)
        nanosleep(&ts2, &ts1);
    };
    clear();
    mvprintw(2, 5, "GAME OVER :(");
    move(21, 26);
    save_high_score("record.txt", gameInfo.high_score);
    freeGameInfo(&gameInfo, tetg->field, tetg->figurest);
    freeTetGame(tetg);
};


int main(){
    init();
    int flag = 1;
    while (flag){
        DrawMenu();
        move(21, 26);
        refresh();
        int ch = getch();
        switch (ch){
            case 's':
                clear();
                start_game();
                break;
            case 'q':
                flag = 0;
                break;
            default:
                break;
        };

    };
    endwin(); // вернуть исходный вид окна терминала
    return 0;
};