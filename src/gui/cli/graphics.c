#include "graphics.h"

void displayBorder() { // Скрываем курсор
  init_pair(1, COLOR_BLUE, COLOR_BLACK);
  init_pair(2, COLOR_RED, COLOR_BLACK);
  init_pair(3, COLOR_GREEN, COLOR_BLACK);
  init_pair(4, COLOR_YELLOW, COLOR_BLACK);
  init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(6, COLOR_CYAN, COLOR_CYAN);
  init_pair(7, COLOR_CYAN, COLOR_WHITE);
  // горизонтальные линии границ
  for (int i = 0; i <= WIDTH; i++) {
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
  // горизонтальные линии между границами
  for (int i = 12; i < 25; i++) {
    mvaddch(0, i, ACS_HLINE);
    mvaddch(21, i, ACS_HLINE);
  }
  refresh();
}

void displayField(GameInfo_t *game, Figure_t *figure) {
  // Отображение зафиксированных блоков на поле
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      if (game->field[i][j] == 0) {
        mvaddch(i + 1, j + 1, ' '); // Пустая ячейка
      } else {
        attron(COLOR_PAIR(game->field[i][j]));  // Включаем цвет
        mvaddch(i + 1, j + 1, ACS_CKBOARD);     // Блок заполненной ячейки
        attroff(COLOR_PAIR(game->field[i][j])); // Выключаем цвет
      }
    }
  }
  // Отображение текущей падающей фигуры
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      if (figure->figureArr[y][x] != 0) {
        attron(COLOR_PAIR(figure->figureArr[y][x])); // Включаем цвет
        mvaddch(figure->cordY + y + 1, figure->cordX + x + 1, ACS_CKBOARD);
        attroff(COLOR_PAIR(figure->figureArr[y][x])); // Выключаем цвет
      }
    }
  }
  refresh();
}

void displayStat(GameInfo_t *game) {
  mvprintw(2, 13, "SCORE:");
  mvprintw(3, 13, "%d", game->score);
  mvprintw(5, 13, "HIGH SCORE:");
  mvprintw(6, 13, "%d", game->high_score);
  mvprintw(8, 13, "LEVEL:");
  mvprintw(9, 13, "%d", game->level);
  mvprintw(11, 13, "SPEED:");
  mvprintw(12, 13, "%d", game->speed);
  mvprintw(14, 13, "NEXT:");
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (game->next[i][j] == 0) {
        mvaddch(15 + i, 13 + j * 2, ' ');
        mvaddch(15 + i, 13 + j * 2 + 1, ' ');
      } else {
        mvaddch(15 + i, 13 + j * 2, ACS_CKBOARD);
        mvaddch(15 + i, 13 + j * 2 + 1, ACS_CKBOARD);
      }
    }
  }
  refresh();
}

void init() {
  initscr();            // Инициализация ncurses
  start_color();        // Инициализация цветов
  cbreak();             // Непрерывный ввод (без ожидания Enter)
  noecho();             // Не отображать вводимые символы
  keypad(stdscr, TRUE); // Включить специальные клавиши
  curs_set(0);          // Скрыть курсор
  GameInfo_t game = {0};
  game.level = 1;
  game.speed = 1;
  game.pause = 3;
  readHighScore(&game.high_score);
  Figure_t figure = {0};
  if (initializeField(&game))
    gameProccess(&figure, &game);
  else
    return;
}

void gameProccess(Figure_t *figure, GameInfo_t *game) {
  startMenu(game, figure);     // Отображение стартового меню
  nodelay(stdscr, TRUE);       // Непрерывное считывание ввода
  spawnFirstFig(figure, game); // Генерация первой фигуры
  struct timeval lastFallTime, currentTime;
  gettimeofday(&lastFallTime, NULL);

  while (game->pause != 2) {
    displayBorder();
    displayField(game, figure);
    displayStat(game);
    fallTime(&lastFallTime, &currentTime, game, figure);
    int ch = getch();
    if (ch != ERR) { // Если есть пользовательский ввод
      switch (ch) {
      case 'p':
        userInput(game, figure, Pause);
        break;
      case KEY_LEFT:
        userInput(game, figure, Left);
        break;
      case KEY_RIGHT:
        userInput(game, figure, Right);
        break;
      case KEY_DOWN:
        userInput(game, figure, Down);
        break;
      case 'z':
        userInput(game, figure, Action);
        break;
      case 'q':
        userInput(game, figure, Terminate);
        break;
      default:
        break;
      }
    }
    if (game->pause == 1) {
      while (game->pause == 1) {
        ch = getch();
        switch (ch) {
        case 'p':
          userInput(game, figure, Pause);
          break;
        case 'q':
          userInput(game, figure, Terminate);
          break;
        }
        mvprintw(10, 3, "Pause");
        mvprintw(11, 2, "Press 'p'");
        refresh();
      }
    }
  }
  writeHighScore(game->high_score);
  gameOver();
  freeMemory(game, 1, 1);
  endwin();
}

void gameOver() {
  mvprintw(19, 13, "GAME OVER!!!");
  refresh();
  sleep(3);
}

void startMenu(GameInfo_t *game, Figure_t *figure) {
  int ch;
  while (game->pause == 3) {
    displayBorder();
    displayField(game, figure);
    displayStat(game);
    mvprintw(4, 3, "TETRIS");
    mvprintw(8, 2, "Press 's'");
    mvprintw(9, 2, "to start");
    mvprintw(11, 2, "Press 'q'");
    mvprintw(12, 2, "to exit");
    ch = getch();
    switch (ch) {
    case 's':
      userInput(game, figure, Start);
      break;
    case 'q':
      userInput(game, figure, Terminate);
      break;
    default:
      break;
    }
  }
  refresh();
}