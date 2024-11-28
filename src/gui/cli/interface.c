#ifndef INTERFACE_C
#define INTERFACE_C
#include "interface.h"


void init(){
    initscr();
    keypad(stdscr, TRUE); //для работы стрелок
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_MAGENTA);
    init_pair(2, COLOR_GREEN, COLOR_GREEN);
    cbreak(); //немедленный вывод символа нажатой клавиши
    noecho(); // отключаем вывод нажатого символа
    nodelay(stdscr, TRUE); // отключаем задержку при вывозе getch 
    scrollok(stdscr, TRUE); //отключаем возможность скролинга
}

void DrawLines(){
    // горизонтальные линии границ
        for (int i = 0; i <= WIDTH + 15; i++) {
            mvaddch(0, i, ACS_HLINE);
            mvaddch(21, i, ACS_HLINE);
        }
        // вертикальные линии границ
        for (int i = 0; i <= HEIGHT; i++) {
            mvaddch(i, 0, ACS_VLINE);
            mvaddch(i, 11, ACS_VLINE);
            mvaddch(i, 25, ACS_VLINE);
        }
        // углы и соединения границ
        mvaddch(0, 0, ACS_ULCORNER);
        mvaddch(21, 0, ACS_LLCORNER);
        mvaddch(0, 25, ACS_URCORNER);
        mvaddch(21, 25, ACS_LRCORNER);
        mvaddch(21, 11, ACS_BTEE);
        mvaddch(0, 11, ACS_TTEE);
}

void DrawMenu(){
    // горизонтальные линии границ
        for (int i = 0; i <= WIDTH + 15; i++) {
            mvaddch(0, i, ACS_HLINE);
            mvaddch(21, i, ACS_HLINE);
        }
        // вертикальные линии границ
        for (int i = 0; i <= HEIGHT; i++) {
        mvaddch(i, 0, ACS_VLINE);
        // mvaddch(i, 11, ACS_VLINE);
        mvaddch(i, 25, ACS_VLINE);
        }
        // углы и соединения границ
        mvaddch(0, 0, ACS_ULCORNER);
        mvaddch(21, 0, ACS_LLCORNER);
        mvaddch(0, 25, ACS_URCORNER);
        mvaddch(21, 25, ACS_LRCORNER);
        mvprintw(5, 5, "Press S for start");
        mvprintw(8, 5, "Press Q for quit");
}

// Отображение следующей фигуры
void printNextFigure(GameInfo_t* gameInfo) {
    int start_x = 15; 
    int start_y = 15; 
    
    // Получаем данные для следующей фигуры
    int** next_blocks = gameInfo->next;
    
    mvprintw(start_y - 1, start_x - 3, "Next figure:");
    
    // Отображаем блоки следующей фигуры (матрица 5x5)
    for (int y = 0; y < 5; y++) {
        for (int x = 0; x < 5; x++) {
            int block = next_blocks[y][x];  // Берем блок из массива
            
            if (block != 0) {  // Отображаем только если блок не пустой
                attron(COLOR_PAIR(block));               // Устанавливаем цвет для этого блока
                mvaddch(start_y + y, start_x + x, ' ');  // Отображаем символ 'X' для блока
                attroff(COLOR_PAIR(block));              // Сбрасываем цвет
            } else {
                mvaddch(start_y + y, start_x + x, ' ');  // Печатаем пустое место, если блок пустой
            }
        }
    }

    refresh(); 
}

// Функция для печати игрового поля
void printTetField(GameInfo_t* gameInfo) {
    int height = 20; // Задаём высоту поля (пример)
    int width = 10;  // Задаём ширину поля (пример)

    // Печать игрового поля (состояние поля)
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int sym = 0;
            if (gameInfo->field[i][j] != 0) {
                sym = 1; // Есть блок на поле
            }

            // Рисуем блок с цветом
            attron(COLOR_PAIR(sym)); // Устанавливаем цвет
            mvaddch(i + 1, j + 1, ' '); // Печатаем блок
            attroff(COLOR_PAIR(sym)); // Сбрасываем цвет
        }
    }

    // Печать информации об игре (очистка экрана для статистики)
    attron(COLOR_WHITE);
    mvprintw(2, 12, "P->pause");
    mvprintw(3, 12, "Q->exit");
    mvprintw(5, 12, "Score: %d", gameInfo->score);
    mvprintw(7, 12, "High score: %d", gameInfo->high_score);
    mvprintw(9, 12, "Level: %d", gameInfo->level);

    // Печать статистики
    move(21, 26);
    refresh();
}

void printTetFigure(TetGame* tetg) {
    TetFigure* figure = tetg->figure; 
    int height = 20;  
    int width = 10; 
    for (int i = 0; i < figure->size; i++) {
        for (int j = 0; j < figure->size; j++) {
            int block = figure->blocks[i * figure->size + j].b; // Получаем блок
            if (block != 0) { // Если блок существует
                int x = figure->x + j; // Рассчитываем текущую позицию X фигуры
                int y = figure->y + i; // Рассчитываем текущую позицию Y фигуры
                if (x >= 0 && x < width && y >= 0 && y < height) { // Проверка на выход за границы
                    attron(COLOR_PAIR(block)); // Устанавливаем цвет
                    mvaddch(y + 1, x + 1, ' '); // Печатаем блок
                    attroff(COLOR_PAIR(block)); // Сбрасываем цвет
                }
            }
        }
    }

    refresh();
}

void printPause(GameInfo_t* gameInfo) {
    // Позиции на экране для вывода
    int start_x = 12;
    int start_y = 12;
    if (gameInfo->pause == 1) {
        attron(COLOR_PAIR(1));  // Устанавливаем цвет для паузы (например, красный)
        mvprintw(start_y, start_x, " PAUSED "); // Выводим текст на экране
        attroff(COLOR_PAIR(1));  // Сбрасываем цвет
    } else {
        attron(COLOR_PAIR(2));  // Если не на паузе, выводим сообщение об этом (например, зелёный)
        mvprintw(start_y, start_x, "RESUMING"); // Выводим текст на экране
        attroff(COLOR_PAIR(2));  // Сбрасываем цвет
    }
    refresh();
}



#endif
