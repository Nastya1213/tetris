#include <check.h>
#include "../brick_game/tetris/tetris.h"
#include <stdlib.h>

 

// Функция для создания игрового состояния, которая используется в тестах
TetGame* create_default_game() {
    int field_width = 10;
    int field_height = 20;
    int figure_size = 4;
    int count = 5;
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
    {0}, {0}, {0}, {0}, {0}};
    int score = 0;
    return createTetGame(field_width, field_height, figure_size, count, tet_templates, score);
}

// Тестирование создания игрового состояния
START_TEST(test_createTetGame) {
    TetGame* game = create_default_game();
    ck_assert_ptr_nonnull(game);
    ck_assert_int_eq(game->field->width, 10);
    ck_assert_int_eq(game->field->height, 20);
    ck_assert_int_eq(game->figure->size, 4);
    ck_assert_int_eq(game->playing, TET_PLAYING); // Игра должна быть в процессе
    ck_assert_int_eq(game->score, 0);
    
    freeTetGame(game); // Освобождение ресурсов
}
END_TEST

// Тестирование создания фигуры
START_TEST(test_createTetFigure) {
    TetGame* game = create_default_game();
    TetFigure* figure = createTetFigure(game);
    ck_assert_ptr_nonnull(figure);
    ck_assert_int_eq(figure->x, 0);  // Фигура должна появляться в верхнем левом углу
    ck_assert_int_eq(figure->y, 0);
    ck_assert_int_eq(figure->size, 4);
    freeTetFigure(figure);
    freeTetGame(game);
}
END_TEST

// Тестирование движения фигуры вниз
START_TEST(test_moveFigureDown) {
    TetGame* game = create_default_game(); 
    TetFigure* t = game->figure;  
    int initial_y = t->y;  
    moveFigureDown(game);  
    ck_assert_int_eq(t->y, initial_y + 1);
}
END_TEST


// Тестирование столкновения фигуры
START_TEST(test_collusionTet) {
    TetGame* game = create_default_game();
    TetFigure* figure = game->figure;  
    figure->y = 13;
    int collision = collusionTet(game);
    ck_assert_int_eq(collision, 0);  // Столкновения нет
    figure->y = 20;  // Фигура выходит за пределы
    collision = collusionTet(game);
    ck_assert_int_eq(collision, 1);  // Ожидаем столкновение
    freeTetGame(game);
}
END_TEST

// Тестирование установки фигуры на поле
START_TEST(test_plantFigure) {
    TetGame* game = create_default_game();
    TetFigure* figure = createTetFigure(game);
    moveFigureDown(game); // Двигаем фигуру вниз
    plantFigure(game);    // Устанавливаем фигуру
    // Проверяем, что игра продолжает работу и фигура установлена
    ck_assert_int_eq(game->playing, TET_PLAYING);
    freeTetFigure(figure);
    freeTetGame(game);
}
END_TEST

// Тестирование удаления линии
START_TEST(test_eraseLinesTet) {
    TetGame* game = create_default_game();
    TetField* field = game->field;
    for (int x = 0; x < field->width; x++) {
        field->blocks[x] = (TetBlock) {1};  // Заполняем все блоки первой строки
    }
    int erased_lines = eraseLinesTet(game);
    ck_assert_int_eq(erased_lines, 1);
    freeTetGame(game);  // Освобождаем память
}
END_TEST

// Тестирование изменения счета
START_TEST(test_convert_count_to_score) {
    int count = 3;
    int score = convert_count_to_score(count);
    ck_assert_int_eq(score, 700);  // Ожидаем 700 очков
}
END_TEST

// Тестирование движения фигуры вправо
START_TEST(test_moveFigureRight) {
    TetGame* game = create_default_game();
    TetFigure* t = game->figure;
    int initial_x = t->x;
    moveFigureRight(game);
    ck_assert_int_eq(t->x, initial_x + 1);
    freeTetGame(game);
}
END_TEST

// Тестирование движения фигуры влево
START_TEST(test_moveFigureLeft) {
    TetGame* game = create_default_game();
    TetFigure* t = game->figure;
    int initial_x = t->x;
    moveFigureLeft(game);
    ck_assert_int_eq(t->x, initial_x - 1);
    freeTetGame(game);
}
END_TEST

// Тестирование поворота фигуры
START_TEST(test_rotateTetFigure) {
    TetGame* game = create_default_game();
    TetFigure* figure = game->figure;
    TetFigure* rotated = rotTetFigure(game);
    ck_assert_int_eq(figure->x, rotated->x);
    ck_assert_int_eq(figure->y, rotated->y);
    freeTetFigure(rotated);
    freeTetGame(game);
}
END_TEST




START_TEST(test_dropNewFigure) {
    TetGame* game = create_default_game();

    // Сохраняем старую фигуру
    TetFigure* old_figure = game->figure;

    // Запускаем функцию для создания новой фигуры
    dropNewFigure(game);

    // Проверяем, что новая фигура появилась в центре поля
    ck_assert_int_eq(game->figure->x, game->field->width / 2 - game->figure->size / 2);
    ck_assert_int_eq(game->figure->y, 0);

    // Проверяем, что фигура была заменена на новую
    ck_assert_ptr_ne(game->figure, old_figure);

    // Освобождаем память
    freeTetGame(game);
}
END_TEST


// Тестирование коллизий с блоками на поле
START_TEST(test_collusionTet_with_existing_blocks) {
    TetGame* game = create_default_game();

    TetField* field = game->field;
    field->blocks[10] = (TetBlock){1}; 
    field->blocks[11] = (TetBlock){1}; 
    TetFigure* figure = game->figure;
    figure->x = 0;  
    figure->y = 0;  
    int collision = collusionTet(game);
    ck_assert_int_eq(collision, 1);  
    freeTetGame(game);
}
END_TEST


START_TEST(test_calculateTet) {
    TetGame* game = create_default_game();
    TetPlayer player;
    player.action = TET_PLAYER_NOP;
    game->player = &player;
    game->playing = 1;
    calculateTet(game);
    ck_assert_int_eq(game->playing, 1);  
    freeTetGame(game);
}
END_TEST

START_TEST(test_createTetField) {
    int width = 10;
    int height = 20;
    TetField* field = createTetField(width, height);
    ck_assert_ptr_nonnull(field);
    ck_assert_int_eq(field->width, width);
    ck_assert_int_eq(field->height, height);
    for (int i = 0; i < width * height; i++) {
        ck_assert_int_eq(field->blocks[i].b, 0);
    }
    free(field->blocks);
    free(field);
}
END_TEST

START_TEST(test_lineFilledTet) {
    int width = 10;
    int height = 20;
    TetField* field = createTetField(width, height);
    int filled_row = 5;
    for (int j = 0; j < width; j++) {
        field->blocks[filled_row * width + j].b = 1; 
    }
    ck_assert_int_eq(lineFilledTet(filled_row, field), 1);
    int partially_filled_row = 6;
    for (int j = 0; j < width; j++) {
        field->blocks[partially_filled_row * width + j].b = 1;
    }
    field->blocks[partially_filled_row * width + 3].b = 0; 
    ck_assert_int_eq(lineFilledTet(partially_filled_row, field), 0);
    int empty_row = 7;
    for (int j = 0; j < width; j++) {
        field->blocks[empty_row * width + j].b = 0;
    }
    ck_assert_int_eq(lineFilledTet(empty_row, field), 0);
    free(field->blocks);
    free(field);
}
END_TEST


START_TEST(test_dropLineTet) {
    int width = 10;
    int height = 20;
    TetField* field = createTetField(width, height);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            field->blocks[i * width + j].b = i + 1; 
        }
    }
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            ck_assert_int_eq(field->blocks[i * width + j].b, i + 1);
        }
    }
    dropLineTet(5, field);
    for (int j = 0; j < width; j++) {
        ck_assert_int_eq(field->blocks[5 * width + j].b, 5);
    }
    for (int j = 0; j < width; j++) {
        ck_assert_int_eq(field->blocks[4 * width + j].b, 4); 
    }
    for (int j = 0; j < width; j++) {
        ck_assert_int_eq(field->blocks[0 * width + j].b, 1);
    }
    dropLineTet(0, field);
    for (int j = 0; j < width; j++) {
        ck_assert_int_eq(field->blocks[0 * width + j].b, 0);  
    }
    free(field->blocks);
    free(field);
}
END_TEST


START_TEST(test_calculateTet_gameover) {
    TetGame* game = create_default_game();
    TetPlayer player;
    player.action = TET_PLAYER_DOWN;
    game->player = &player;
    game->playing = 1;
    int y = game->figure->y;
    int x = game->figure->x;
    calculateTet(game);
    // Проверим, что игра перешла в состояние GAMEOVER
    ck_assert_int_eq(game->figure->y, y + 1);
    player.action = TET_PLAYER_RIGHT;
    calculateTet(game);
    ck_assert_int_eq(game->figure->x, x + 1);
    player.action = TET_PLAYER_LEFT;
    x = game->figure->x;
    calculateTet(game);
    ck_assert_int_eq(game->figure->x, x - 1);


    freeTetGame(game);
}
END_TEST




// Функция для добавления всех тестов
Suite* tetris_suite(void) {
    Suite* s = suite_create("Tetris");
    TCase* tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_createTetGame);
    tcase_add_test(tc_core, test_createTetFigure);
    tcase_add_test(tc_core, test_moveFigureDown);
    tcase_add_test(tc_core, test_collusionTet);
    tcase_add_test(tc_core, test_plantFigure);
    tcase_add_test(tc_core, test_eraseLinesTet);
    tcase_add_test(tc_core, test_convert_count_to_score);
    tcase_add_test(tc_core, test_moveFigureRight);
    tcase_add_test(tc_core, test_moveFigureLeft);
    tcase_add_test(tc_core, test_rotateTetFigure);
    //tcase_add_test(tc_core, test_moveAfterGameOver);
    tcase_add_test(tc_core, test_dropNewFigure);
    tcase_add_test(tc_core, test_collusionTet_with_existing_blocks);
    tcase_add_test(tc_core, test_calculateTet);
    tcase_add_test(tc_core, test_createTetField);
    tcase_add_test(tc_core, test_lineFilledTet);
    tcase_add_test(tc_core, test_dropLineTet);
    tcase_add_test(tc_core, test_calculateTet_gameover);
    suite_add_tcase(s, tc_core);

    return s;
}


int main(void) {
    int number_failed;
    Suite* s = tetris_suite();
    SRunner* sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? 0 : 1;
}
