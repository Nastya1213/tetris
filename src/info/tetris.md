
## Описание основных структур

### `TetBlock`
Структура, представляющая один блок в игре. Она хранит информацию о состоянии блока (например, занят или нет).

```c
typedef struct {
    int b; // 0 - блок пуст, ненулевое значение - блок занят
} TetBlock;
TetField
```
Структура, представляющая игровое поле. Содержит информацию о ширине, высоте и массив блоков.

```c
typedef struct {
    int width;
    int height;
    TetBlock* blocks; // Массив блоков, представляющих игровое поле
} TetField;
TetFiguresT
```
Структура, которая хранит информацию о фигурах (шаблонах) Тетриса.

```c
typedef struct {
    int count;    // Количество фигур
    int size;     // Размер каждой фигуры
    TetBlock* blocks; // Массив блоков для каждой фигуры
} TetFiguresT;
TetFigure
```
Структура, представляющая одну фигуру на поле. Содержит её положение и блоки.

```c
typedef struct {
    int x, y;   // Позиция фигуры на поле
    int size;   // Размер фигуры
    TetBlock* blocks; // Массив блоков, представляющих фигуру
} TetFigure;
TetGame
```
Структура, содержащая информацию о текущем состоянии игры.

```c
typedef struct {
    TetField* field;           // Игровое поле
    TetFiguresT* figurest;     // Шаблоны фигур
    TetFigure* figure;         // Текущая фигура
    int ticks;                 // Количество тактов для движения
    int ticks_left;            // Количество оставшихся тактов
    int score;                 // Очки игрока
    int high_score;            // Рекорд
    int playing;               // Статус игры (игра продолжается или завершена)
    int level;                 // Уровень
    int next_template;         // Индекс следующего шаблона фигуры
    // и другие параметры
} TetGame;
```
## Описание функций
```createTetFiguresT```
Создает структуру TetFiguresT, которая хранит данные о шаблонах фигур.

```c
TetFiguresT* createTetFiguresT(int count, int figures_size, TetBlock* figures_template);
freeTetFiguresT
```
Освобождает память, занятую объектом TetFiguresT.

```c
void freeTetFiguresT(TetFiguresT* tetft);
createTetField
```
Создает игровое поле с заданной шириной и высотой.

```c
TetField* createTetField(int width, int height);
freeTetField
```
Освобождает память, занятую игровым полем.

```c
void freeTetField(TetField* tetf);
createTetGame
```
Создает структуру TetGame и инициализирует игровые параметры.

```c
TetGame* createTetGame(int field_width, int field_height, int figure_size, 
                       int count, TetBlock* figures_template, int score);
freeTetGame
```
Освобождает память, занятую игрой.


```void freeTetGame(TetGame* tetg);```
Движение и управление фигурой
moveFigureDown - Перемещает текущую фигуру вниз.
moveFigureUp - Перемещает текущую фигуру вверх.
moveFigureRight - Перемещает текущую фигуру вправо.
moveFigureLeft - Перемещает текущую фигуру влево.
collusionTet
Проверяет столкновение текущей фигуры с другими блоками на поле.

```c
int collusionTet(TetGame* tetg);
plantFigure
```
Переносит фигуру на поле после её падения.

```c
void plantFigure(TetGame* tetg);
lineFilledTet
```
Проверяет, заполнена ли строка игрового поля.

```c
int lineFilledTet(int i, TetField* tfl);
dropLineTet
```
Сдвигает блоки игрового поля вниз после удаления строки.

```c
void dropLineTet(int i, TetField* tfl);
eraseLinesTet
```
Удаляет все заполненные строки на поле и возвращает количество удалённых строк.

```c
int eraseLinesTet(TetGame* tetg);
createTetFigure
```
Создает новую фигуру в памяти.

```c
TetFigure* createTetFigure(TetGame* tetg);
freeTetFigure
```
Освобождает память, занятую фигурой.

```c
void freeTetFigure(TetFigure* tf);
dropNewFigure
```
Генерирует новую фигуру и помещает её на поле.

```c
void dropNewFigure(TetGame* tetg);
rotTetFigure
```
Поворачивает текущую фигуру.

```c
TetFigure* rotTetFigure(TetGame* tetg);
convert_count_to_score
```
Преобразует количество удалённых строк в очки.

```c
int convert_count_to_score(int count);
calculateTet
```
Основная игровая логика, которая обновляет состояние игры, проверяет столкновения, двигает фигуры и рассчитывает очки.

```
void calculateTet(TetGame* tetg);
```
## Константы

```TET_TICKS_START``` — начальное количество тактов для перемещения.\
```TET_GAMEOVER``` — статус игры, когда игра окончена.\
```TET_PLAYER_*``` — различные действия игрока (влево, вправо, вверх, вниз).