#include "tetris.h"

const int TETROMINOS[7][4][4] = {
    // Квадрат
    {{0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
    // Линия
    {{0, 0, 0, 0}, {2, 2, 2, 2}, {0, 0, 0, 0}, {0, 0, 0, 0}},
    // Т-образная фигура
    {{0, 0, 0, 0}, {0, 3, 3, 3}, {0, 0, 3, 0}, {0, 0, 0, 0}},
    // L-образная фигура
    {{0, 0, 4, 0}, {0, 0, 4, 0}, {0, 0, 4, 4}, {0, 0, 0, 0}},
    // Обратная L-образная фигура
    {{0, 5, 0, 0}, {0, 5, 0, 0}, {0, 5, 5, 0}, {0, 0, 0, 0}},
    // Z-образная фигура
    {{0, 0, 0, 0}, {6, 6, 0, 0}, {0, 6, 6, 0}, {0, 0, 0, 0}},
    // Обратная Z-образная фигура
    {{0, 0, 0, 0}, {0, 7, 7, 0}, {7, 7, 0, 0}, {0, 0, 0, 0}}};

int initializeField(GameInfo_t *game) {
  // инициализируем поле для игры
  game->field = (int **)malloc(HEIGHT * sizeof(int *));
  if (game->field == NULL) {
    perror("Failed to allocate memory for game->field");
    return 0;
  }
  for (int i = 0; i < HEIGHT; i++) {
    game->field[i] = (int *)malloc(WIDTH * sizeof(int));
    if (game->field[i] == NULL) {
      perror("Failed to allocate memory for game->field[i]");
      for (int j = 0; j < i; j++) {
        free(game->field[j]);
      }
      free(game->field);
      return 0;
    }
  }
  // Выделяем память для следующей фигуры
  game->next = (int **)malloc(4 * sizeof(int *));
  if (game->next == NULL) {
    perror("Failed to allocate memory for game->next");
    freeMemory(game, 1, 0);
    return 0;
  }
  for (int i = 0; i < 4; i++) {
    game->next[i] = (int *)malloc(4 * sizeof(int));
    if (game->next[i] == NULL) {
      perror("Failed to allocate memory for game->next[i]");
      for (int j = 0; j < i; j++) {
        free(game->next[j]);
      }
      free(game->next);
      freeMemory(game, 1, 0);
      return 0;
    }
  }
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      game->field[i][j] = 0;
    }
  }
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      game->next[i][j] = 0;
    }
  }
  return 1;
}

int canMoveDown(Figure_t *figure, GameInfo_t *game) {
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      if (figure->figureArr[y][x] != 0) {
        int newY = figure->cordY + y + 1;
        if (newY >= HEIGHT || game->field[newY][figure->cordX + x] != 0) {
          return 0; // Если достигаем дна или сталкиваемся с другой фигурой
        }
      }
    }
  }
  return 1;
}

void movePieceDown(Figure_t *figure, GameInfo_t *game) {
    if (canMoveDown(figure, game)) {
        figure->cordY++;
    } else {
        fixPiece(figure, game);
        checkRow(game);
        checkGameOver(game);
        spawnNewPiece(figure, game);
    }
}


void fixPiece(Figure_t *figure, GameInfo_t *game) {
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (figure->figureArr[y][x] != 0) {
                int newY = figure->cordY + y;
                int newX = figure->cordX + x;
                // Проверяем, что индексы в пределах игрового поля
                if (newY >= 0 && newY < HEIGHT && newX >= 0 && newX < WIDTH) {
                    game->field[newY][newX] = figure->figureArr[y][x];
                } else {
                    printf("Error: Out of bounds! Y: %d, X: %d\n", newY, newX);
                }
            }
        }
    }
}


void spawnNewPiece(Figure_t *figure, GameInfo_t *game) {
  // Копируем новую фигуру в текущую
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      figure->figureArr[y][x] = game->next[y][x];
    }
  }
  int random = rand() % 7;
  figure->type = figure->nextType;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      game->next[i][j] = TETROMINOS[random][i][j];
    }
  }
  figure->nextType = random;
  figure->cordX = 3;
  figure->cordY = 0;
}

void spawnFirstFig(Figure_t *figure, GameInfo_t *game) {
  figure->cordX = 3;
  figure->cordY = 0;
  // начальная фигура и некст фигура
  int random = rand() % 8;
  figure->nextType = random;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      game->next[i][j] = TETROMINOS[random][i][j];
    }
  }
  random += (random == 0) ? 1 : -1;
  figure->type = random;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      figure->figureArr[i][j] = TETROMINOS[random][i][j];
    }
  }
}

bool canRotate(Figure_t *figure, GameInfo_t *game, int tempFigure[4][4]) {
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (tempFigure[y][x] != 0) {
                int newY = figure->cordY + y;
                int newX = figure->cordX + x;

                // Проверка на выход за границы поля
                if (newY < 0 || newY >= HEIGHT || newX < 0 || newX >= WIDTH) {
                    return false;
                }

                // Проверка на пересечение с другими блоками
                if (game->field[newY][newX] != 0) {
                    return false;
                }
            }
        }
    }
    return true;
}


int canMove(Figure_t *figure, GameInfo_t *game, int dx, int dy) {
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      if (figure->figureArr[y][x]) {
        int newX = figure->cordX + x + dx;
        int newY = figure->cordY + y + dy;
        // Проверка на границы поля
        if (newX < 0 || newX >= WIDTH || newY >= HEIGHT) {
          return 0; // Выход за границы
        }
        // Проверка на столкновение с существующими фигурами на поле
        if (game->field[newY][newX]) {
          return 0;
        }
      }
    }
  }
  return 1;
}

void rotateFigure(Figure_t *figure, GameInfo_t *game) {
    int tempFigure[4][4] = {0};

    // Квадратная фигура (O) не вращается
    if (figure->type == O_FIGURE_TYPE) {
        return;
    }

    // Поворот на 90 градусов по часовой стрелке
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            tempFigure[x][3 - y] = figure->figureArr[y][x];
        }
    }

    // Проверяем, возможно ли вращение в текущей позиции
    if (canRotate(figure, game, tempFigure)) {
        // Копируем временную фигуру в основную
        for (int y = 0; y < 4; y++) {
            for (int x = 0; x < 4; x++) {
                figure->figureArr[y][x] = tempFigure[y][x];
            }
        }
    } else {
        printf("Rotation blocked at Y: %d, X: %d\n", figure->cordY, figure->cordX);
    }
}


void checkRow(GameInfo_t *game) {
  int streak = 0;
  for (int i = HEIGHT - 1; i >= 0; i--) {
    bool isFullRow = true;
    for (int j = 0; j < WIDTH; j++) {
      if (!game->field[i][j]) {
        isFullRow = false;
        break;
      }
    }
    if (isFullRow) {
      moveField(game, i);
      streak++;
      i++;
    }
  }
  if (streak)
    updateScore(game, streak);
}

void moveField(GameInfo_t *game, int row) {
  for (int i = row; i > 0; i--)
    for (int j = 0; j < WIDTH; j++) {
      game->field[i][j] = game->field[i - 1][j];
    }
  for (int i = 0; i < WIDTH; i++) {
    game->field[0][i] = 0;
  }
}

void updateScore(GameInfo_t *game, int streak) {
  int score_increments[4] = {100, 300, 700, 1500};
  game->score += score_increments[streak - 1];
  if (game->score > 99999)
    game->score = 99999;
  if (game->score > game->high_score)
    game->high_score = game->score;
  game->level = (game->score / 600) + 1;
  if (game->level > 10)
    game->level = 10;
  game->speed = game->level;
}

void checkGameOver(GameInfo_t *game) {
  for (int i = 0; i < WIDTH; i++)
    if (game->field[1][i] != 0)
      game->pause = 2;
}

void writeHighScore(int hscore) {
  FILE *fp = fopen("hscore.txt", "w");
  if (fp == NULL) {
    printf("Error opening file!\n");
    return;
  }
  fprintf(fp, "%d", hscore);
  fclose(fp);
}

void readHighScore(int *hscore) {
  FILE *fp = fopen("hscore.txt", "r");
  if (fp == NULL) {
    *hscore = 0;
  } else {
    fscanf(fp, "%d", hscore);
    fclose(fp);
  }
}

void userInput(GameInfo_t *game, Figure_t *figure, UserAction_t action) {
  switch (action) { // 0 active 1 pause 2 exit 3 wait to start
  case Pause:
    game->pause = !game->pause;
    break;
  case Left:
    if (canMove(figure, game, -1, 0)) {
      figure->cordX--;
    }
    break;
  case Right:
    if (canMove(figure, game, 1, 0)) {
      figure->cordX++;
    }
    break;
  case Down:
    movePieceDown(figure, game);
    break;
  case Action:
    rotateFigure(figure, game);
    break;
  case Terminate:
    game->pause = 2;
    break;
  case Start:
    game->pause = 0;
    break;
  default:
    break;
  }
}

void freeMemory(GameInfo_t *game, int f, int n) {
  if (f) {
    for (int i = 0; i < HEIGHT; i++) {
      free(game->field[i]);
    }
    free(game->field);
  }
  if (n) {
    for (int i = 0; i < 4; i++) {
      free(game->next[i]);
    }
    free(game->next);
  }
}

void fallTime(struct timeval *lastFallTime, struct timeval *currentTime,
              GameInfo_t *game, Figure_t *figure) {
  int speedState[10] = {1000, 900, 800, 700, 600, 500, 400, 300, 200, 100};
  gettimeofday(currentTime, NULL);
  if ((currentTime->tv_sec - lastFallTime->tv_sec) * 1000 +
          (currentTime->tv_usec - lastFallTime->tv_usec) / 1000 >
      speedState[game->speed - 1]) {
    movePieceDown(figure, game);
    gettimeofday(lastFallTime, NULL); // Обновляем время последнего падения
  }
}