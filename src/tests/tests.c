#include <check.h>

#include "../brick_game/tetris/tetris.h"

START_TEST(test_initializeField) {
  GameInfo_t game;
  int result = initializeField(&game);
  ck_assert_int_eq(result, 1);

  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      ck_assert_int_eq(game.field[i][j], 0);
    }
  }

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      ck_assert_int_eq(game.next[i][j], 0);
    }
  }

  freeMemory(&game, 1, 1);
}
END_TEST

START_TEST(test_canMoveDown) {
  GameInfo_t game;
  Figure_t figure;
  initializeField(&game);
  spawnFirstFig(&figure, &game);

  int result = canMoveDown(&figure, &game);
  ck_assert_int_eq(result,
                   1);  // Фигура может двигаться вниз, так как поле пустое

  figure.cordY = HEIGHT - 1;  // Устанавливаем фигуру в нижний край поля
  result = canMoveDown(&figure, &game);
  ck_assert_int_eq(result, 0);  // Фигура не может двигаться дальше вниз

  freeMemory(&game, 1, 1);
}
END_TEST

// Тест функции movePieceDown
START_TEST(test_movePieceDown) {
  GameInfo_t game;
  Figure_t figure;
  initializeField(&game);
  spawnFirstFig(&figure, &game);

  int initialY = figure.cordY;
  movePieceDown(&figure, &game);
  ck_assert_int_eq(figure.cordY,
                   initialY + 1);  // Фигура должна была опуститься на 1

  figure.cordY = HEIGHT - 2;  // Устанавливаем фигуру в нижний край поля
  movePieceDown(&figure, &game);  // Фигура должна зафиксироваться на месте
  ck_assert_int_eq(game.field[HEIGHT - 1][figure.cordX],
                   figure.figureArr[0][0]);

  freeMemory(&game, 1, 1);
}
END_TEST

// Тест функции spawnNewPiece
START_TEST(test_spawnNewPiece) {
  GameInfo_t game;
  Figure_t figure;
  initializeField(&game);
  spawnFirstFig(&figure, &game);

  spawnNewPiece(&figure, &game);
  ck_assert_int_ge(figure.cordY, 0);  // Новая фигура должна появиться сверху

  freeMemory(&game, 1, 1);
}
END_TEST

// Тест функции canMove
START_TEST(test_canMove) {
  GameInfo_t game;
  Figure_t figure;
  initializeField(&game);
  spawnFirstFig(&figure, &game);

  int result = canMove(&figure, &game, -1, 0);
  ck_assert_int_eq(result, 1);  // Фигура может двигаться влево

  figure.cordX = 0;  // Устанавливаем фигуру в край поля
  result = canMove(&figure, &game, -1, 0);
  ck_assert_int_eq(result, 0);  // Фигура не может выйти за границу

  freeMemory(&game, 1, 1);
}
END_TEST

// Тест функции rotateFigure
START_TEST(test_rotateFigure) {
    GameInfo_t game;
    Figure_t figure;
    initializeField(&game);
    spawnFirstFig(&figure, &game);

    int originalType = figure.type;
    rotateFigure(&figure, &game);

    ck_assert_int_ne(figure.type, originalType);  // Проверяем изменение типа
    ck_assert(canRotate(&figure, &game, figure.figureArr));  // Проверяем корректность вращения

    freeMemory(&game, 1, 1);
}
END_TEST


// Тест функции checkRow
START_TEST(test_checkRow) {
  GameInfo_t game = {0};
  game.level = 1;
  game.speed = 1;
  initializeField(&game);
  // Заполняем одну строку полностью
  for (int i = 0; i < WIDTH; i++) {
    game.field[HEIGHT - 1][i] = 1;
  }
  checkRow(&game);
  for (int i = 0; i < WIDTH; i++) {
    ck_assert_int_eq(game.field[HEIGHT - 1][i],
                     0);  // Строка должна быть очищена
  }
  freeMemory(&game, 1, 1);
}
END_TEST

// Тест функции updateScore
START_TEST(test_updateScore) {
  GameInfo_t game = {0};
  game.level = 1;
  game.speed = 1;
  initializeField(&game);
  updateScore(&game, 1);
  ck_assert_int_eq(game.score, 100);  // За одну линию добавляется 100 очков
  updateScore(&game, 4);
  ck_assert_int_eq(game.score, 1600);  // За четыре линии добавляется 1500 очков
  freeMemory(&game, 1, 1);
}
END_TEST

// Тест: игра должна закончиться, если вторая строка заполнена.
START_TEST(test_checkGameOver_game_over) {
  GameInfo_t game;
  game.field = (int **)malloc(HEIGHT * sizeof(int *));
  for (int i = 0; i < HEIGHT; i++) {
    game.field[i] = (int *)malloc(WIDTH * sizeof(int));
    for (int j = 0; j < WIDTH; j++) {
      game.field[i][j] = 0;  // Инициализируем пустое поле
    }
  }

  // Заполним вторую строку
  for (int i = 0; i < WIDTH; i++) {
    game.field[1][i] = 1;
  }

  game.pause = 0;  // Игра не приостановлена

  checkGameOver(&game);

  // Ожидаем, что игра приостановится
  ck_assert_int_eq(game.pause, 2);

  for (int i = 0; i < HEIGHT; i++) {
    free(game.field[i]);
  }
  free(game.field);
}
END_TEST

// Тест: игра продолжается, если вторая строка пуста.
START_TEST(test_checkGameOver_no_game_over) {
  GameInfo_t game;
  game.field = (int **)malloc(HEIGHT * sizeof(int *));
  for (int i = 0; i < HEIGHT; i++) {
    game.field[i] = (int *)malloc(WIDTH * sizeof(int));
    for (int j = 0; j < WIDTH; j++) {
      game.field[i][j] = 0;  // Инициализируем пустое поле
    }
  }

  game.pause = 0;  // Игра не приостановлена

  checkGameOver(&game);

  // Ожидаем, что игра не приостановлена
  ck_assert_int_eq(game.pause, 0);

  for (int i = 0; i < HEIGHT; i++) {
    free(game.field[i]);
  }
  free(game.field);
}
END_TEST

START_TEST(test_write_and_read_high_score) {
  int hscore_write = 5000;
  int hscore_read;

  // Сохраняем исходное имя файла, чтобы использовать временный файл для тестов
  char *test_file = "test_hscore.txt";

  // Переопределяем файл для хранения рекордов
  FILE *fp = freopen(test_file, "w", stdout);
  ck_assert_ptr_nonnull(fp);

  // Тестируем запись рекорда
  writeHighScore(hscore_write);
  fclose(fp);

  // Чтение рекорда
  fp = freopen(test_file, "r", stdin);
  readHighScore(&hscore_read);
  fclose(fp);

  // Проверяем, что значение корректно записано и прочитано
  ck_assert_int_eq(hscore_read, hscore_write);

  // Удаляем тестовый файл
  remove(test_file);
}
END_TEST

START_TEST(test_user_input_move) {
  GameInfo_t game;
  Figure_t figure;
  initializeField(&game);
  spawnFirstFig(&figure, &game);
  figure.cordX = 3;

  userInput(&game, &figure, Left);
  ck_assert_int_eq(figure.cordX, 2);  // Должна сдвинуться влево

  userInput(&game, &figure, Right);
  ck_assert_int_eq(figure.cordX, 3);  // Должна сдвинуться влево

  freeMemory(&game, 1, 1);
}
END_TEST

START_TEST(test_user_input_pause) {
  GameInfo_t game;
  game.pause = 0;

  userInput(&game, NULL, Pause);
  ck_assert_int_eq(game.pause, 1);  // Пауза активирована

  userInput(&game, NULL, Pause);
  ck_assert_int_eq(game.pause, 0);  // Пауза деактивирована
}
END_TEST

START_TEST(test_user_input_down) {
  GameInfo_t game;
  Figure_t figure;

  initializeField(&game);
  spawnFirstFig(&figure, &game);

  figure.cordY = 5;  // Устанавливаем начальную позицию фигуры

  userInput(&game, &figure, Down);

  // Проверяем, что фигура сместилась вниз на одну клетку
  ck_assert_int_eq(figure.cordY, 6);

  freeMemory(&game, 1, 1);
}
END_TEST

START_TEST(test_user_input_action_rotate) {
  GameInfo_t game;
  Figure_t figure;

  initializeField(&game);
  spawnFirstFig(&figure, &game);

  int type = figure.type;
  userInput(&game, &figure, Action);

  ck_assert_int_ne(figure.type, type);

  freeMemory(&game, 1, 1);
}
END_TEST

START_TEST(test_user_input_terminate) {
  GameInfo_t game;

  initializeField(&game);
  game.pause = 0;  // Изначально игра не на паузе

  userInput(&game, NULL, Terminate);

  // Проверяем, что игра перешла в режим завершения (pause == 2)
  ck_assert_int_eq(game.pause, 2);

  freeMemory(&game, 1, 1);
}
END_TEST

START_TEST(test_user_input_start) {
  GameInfo_t game;

  initializeField(&game);
  game.pause = 1;

  userInput(&game, NULL, Start);

  ck_assert_int_eq(game.pause, 0);

  freeMemory(&game, 1, 1);
}
END_TEST

START_TEST(test_user_input_default) {
  GameInfo_t game = {0};
  game.level = 1;
  game.speed = 1;
  Figure_t figure = {0};

  initializeField(&game);
  spawnFirstFig(&figure, &game);

  int originalPause = game.pause;
  int originalCordX = figure.cordX;
  int originalCordY = figure.cordY;

  userInput(&game, &figure, -1);

  ck_assert_int_eq(game.pause, originalPause);
  ck_assert_int_eq(figure.cordX, originalCordX);
  ck_assert_int_eq(figure.cordY, originalCordY);

  freeMemory(&game, 1, 1);
}
END_TEST

Suite *tetris_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("Tetris");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_initializeField);
  tcase_add_test(tc_core, test_canMoveDown);
  tcase_add_test(tc_core, test_movePieceDown);
  tcase_add_test(tc_core, test_spawnNewPiece);
  tcase_add_test(tc_core, test_canMove);
  tcase_add_test(tc_core, test_rotateFigure);
  tcase_add_test(tc_core, test_checkRow);
  tcase_add_test(tc_core, test_updateScore);
  tcase_add_test(tc_core, test_user_input_default);
  tcase_add_test(tc_core, test_checkGameOver_game_over);
  tcase_add_test(tc_core, test_checkGameOver_no_game_over);
  tcase_add_test(tc_core, test_write_and_read_high_score);
  tcase_add_test(tc_core, test_user_input_move);
  tcase_add_test(tc_core, test_user_input_pause);
  tcase_add_test(tc_core, test_user_input_down);
  tcase_add_test(tc_core, test_user_input_action_rotate);
  tcase_add_test(tc_core, test_user_input_terminate);
  tcase_add_test(tc_core, test_user_input_start);

  suite_add_tcase(s, tc_core);
  return s;
}

int main(void) {
  int number_failed;
  Suite *s;
  SRunner *sr;

  s = tetris_suite();
  sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
