#ifndef HELP_FUNC_C
#define HELP_FUNC_C
#include "help_func.h"


int load_high_score(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        return 0; // Если файл не найден, начальный рекорд — 0
    }

    int high_score = 0;
    if (fscanf(file, "%d", &high_score) != 1) {
        high_score = 0; // Если данные некорректны, устанавливаем 0
    }

    fclose(file);
    return high_score;
}

void save_high_score(const char *filename, int high_score) {
    FILE *file = fopen(filename, "w"); //запись рекорда
    if (file) {
        fprintf(file, "%d", high_score);
        fclose(file);
    } 
}

// Функция для копирования игрового поля из TetField в поле GameInfo_t
int** copyField(TetField* tetField) {
    int** newField = (int**)malloc(tetField->height * sizeof(int*));
    for (int i = 0; i < tetField->height; i++) {
        newField[i] = (int*)malloc(tetField->width * sizeof(int));
        memcpy(newField[i], &tetField->blocks[i * tetField->width], tetField->width * sizeof(int));
    }
    return newField;
}


GameInfo_t updateCurrentState(TetGame* tetGame, int pause) {
    GameInfo_t gameInfo;
    
    // Копируем игровое поле
    gameInfo.field = copyField(tetGame->field);

    // Получаем блоки следующего шаблона
    TetBlock* nextTemplateBlocks = &tetGame->figurest->blocks[tetGame->next_template * 25];  // 25 блоков в шаблоне 5x5
    int nextTemplateSize = 5;  // Шаблон имеет размер 5x5
    
    // Выделяем память для gameInfo.next (5x5 двумерный массив)
    gameInfo.next = (int**)malloc(nextTemplateSize * sizeof(int*));
    if (gameInfo.next == NULL) {
        return gameInfo;  // Если не удалось выделить память, возвращаем структуру с ошибкой
    }
    
    // Выделяем память для каждого ряда в массиве next (размер 5)
    for (int i = 0; i < nextTemplateSize; i++) {
        gameInfo.next[i] = (int*)malloc(nextTemplateSize * sizeof(int));  // 5 элементов в каждой строке
        if (gameInfo.next[i] == NULL) {
            // Освобождаем уже выделенную память в случае ошибки
            for (int j = 0; j < i; j++) {
                free(gameInfo.next[j]);
            }
            free(gameInfo.next);
            gameInfo.next = NULL;
            return gameInfo;
        }
    }

    // Заполняем gameInfo.next пустыми значениями (по умолчанию 0)
    for (int y = 0; y < nextTemplateSize; y++) {
        for (int x = 0; x < nextTemplateSize; x++) {
            gameInfo.next[y][x] = 0;  // Изначально все блоки считаем пустыми
        }
    }

    // Копируем активные блоки шаблона в gameInfo.next
    for (int y = 0; y < nextTemplateSize; y++) {
        for (int x = 0; x < nextTemplateSize; x++) {
            int index = y * nextTemplateSize + x;
            if (nextTemplateBlocks[index].b != 0) {
                gameInfo.next[y][x] = nextTemplateBlocks[index].b;  // Копируем блок, если он активен
            }
        }
    }

    // Копируем остальные данные
    gameInfo.score = tetGame->score;
    gameInfo.high_score = tetGame->high_score;
    gameInfo.level = tetGame->level;
    gameInfo.speed = tetGame->ticks;
    gameInfo.pause = pause;

    return gameInfo;
}


void freeGameInfo(GameInfo_t* gameInfo, TetField* field, TetFiguresT* figurest) {
    // Освобождение игрового поля (field)
    if (gameInfo->field != NULL) {
        for (int i = 0; i < field->height; i++) {
            free(gameInfo->field[i]);  // Освобождение каждой строки
        }
        free(gameInfo->field);  // Освобождение массива строк
        gameInfo->field = NULL;
    }

    // Освобождение следующей фигуры (next)
    if (gameInfo->next != NULL) {
        int nextTemplateSize = figurest->size;  // Размер следующего шаблона
        for (int i = 0; i < nextTemplateSize; i++) {
            free(gameInfo->next[i]);  // Освобождение каждой строки
        }
        free(gameInfo->next);  // Освобождение массива строк
        gameInfo->next = NULL;
    }
}


void userInput(UserAction_t action, bool hold, TetPlayer* player, int* pause) {
    if (hold) {
        return;
    }
    switch (action)
    {
    case Start:
        player->action = TET_PLAYER_NOP;
        break;
    case Pause:
        *pause = (*pause + 1) % 2;
        break;
    case Terminate:
        player->action = TET_PLAYER_NOP;
        break;
    case Left:
        player->action = TET_PLAYER_LEFT;
        break;
    case Right:
       player->action = TET_PLAYER_RIGHT;
        break;
    case Up:
        player->action = TET_PLAYER_NOP;
        break;
    case Down:
        player->action = TET_PLAYER_DOWN;
        break;
    case Action:
        player->action = TET_PLAYER_UP;
        break;
    default:
        break;
    }
}


void freeGameResources(TetGame *game) {
    if (game) {
        // Освобождение текущей фигуры
        if (game->figure) {
            free(game->figure->blocks); // Освобождение блоков фигуры
            free(game->figure);
        }
        // Освобождение игрового поля
        if (game->field) {
            if (game->field->blocks) {
                free(game->field->blocks);
            }
            free(game->field);
        }
        // Освобождение шаблонов фигур
        if (game->figurest) {
            if (game->figurest->blocks) {
                free(game->figurest->blocks);
            }
            free(game->figurest);
        }
        // Освобождение структуры самой игры
        free(game);
    }
}


#endif