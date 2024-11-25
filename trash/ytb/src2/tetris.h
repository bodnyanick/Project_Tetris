#ifndef TETRIS_H
#define TETRIS_H

#define WIN_INIT(time) {\
                             initscr();\
                             noecho();\
                             curs_set(0);\
                             keypad(stdscr, TRUE);\
                             timeout(time);\
                        }

#define TET_TICKS_START 30  // Константа для кол-ва тактов между моментами падения на ОДИН блок

// 3. Структура по присутствию блока на поле (1/0)?
typedef struct TetBlock {
  int b;  // Переменная присутствия (булево проще?)
} TetBlock;

// 4. Структура для падающей фигуры
typedef struct TetFigure {
  int x;      // Позиция по горизонтали
  int y;      // Позиция по вертикали
  int size;   // Размер фигуры
  TetBlock *blocks;  // Массив из блоков, соответсвующий фигуре
} TetFigure;

// 5. Описание типовых фигур
typedef struct TetFiguresT {
  int count;  // Количество типовых фигур
  int size;  // Длина стороны квадратной области, на которой изображена фигура
  TetBlock *blocks;  // Набор блоков. Переменная blocks ссылается на нерперывную
                     // область памяти с информацией о блоках все шаблонов сразу
} TetFiguresT;

// 2. Структура игрового поля
typedef struct TetField {
  int width;         // Ширина поля
  int height;        // Высота поля
  TetBlock *blocks;  // Хранение массива из блоков
} TetField;

// Набор констант для описания состояния игры
enum {
  TET_GAMEOVER = 0,
  TET_PLAYING,
};

//! Набор констант состояния игры (ШКОЛА-21)
/*typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
} UserAction_t;
*/

// Набор констант действий игрока (моЁ)
enum {
  TET_PLAYER_NOP = 0,  // бездействие
  TET_PLAYER_UP,
  TET_PLAYER_DOWN,
  TET_PLAYER_LEFT,
  TET_PLAYER_RIGHT,
};

// Структура по описанию действий игрока
typedef struct TetPlayer {
  int action;
} TetPlayer;

// 1. Данные для описания игровой ситуации
typedef struct TetGame {
  TetField *field;       // игровое поле с блоками
  TetFigure *figure;     // текущая фигура
  TetFiguresT *figurest;  // как выглядят типовые фигуры
  TetPlayer *player;  // Переменная для взаимодействия с игроком (определяется пользователем, им же присвоится значение указателя на структуру TetPlayer)
  int ticks;
  int ticks_left;
  int playing;
  int score;
} TetGame;

// Прототип ПРОЦЕДУРЫ, рассчитывающий один игровой такт
void calculateTet(TetGame *tetg);
TetFiguresT *createTetFiguresT(int count, int figures_size, TetBlock *figures_template); // Алгоритм инициализации набора шаблонов фигур
void freeTetFiguresT(TetFiguresT *tetft);
TetField *createTetField(int width, int height); // Создание поля для игры
void freeTetField(TetField *tetf);
TetGame *createTetGame(int field_width, int field_height, int figures_size, int count, TetBlock *figures_template); //Прототип функции, инициализирующей основную игровую структуру.
void freeTetGame(TetGame *tetg);
//! Реализация функции из одного игрового ТАКТА
// Перемещение фигуры по стрелкам
void moveFigureDown(TetGame *tetg);
void moveFigureUp(TetGame *tetg);
void moveFigureRight(TetGame *tetg);
void moveFigureLeft(TetGame *tetg);
void plantFigure(TetGame *tetg); // Размещение фигуры после падения
int collisionTet(TetGame *tetg); // Проверка на столкновение
int lineFilledTet(int i, TetField *tfl); // Проверка на заполненность строки
void dropLineTet(int i, TetField *tfl); // Сдвиг блоков вниз на единицу (сдвиг всех НЕ ПУСТЫХ строк)
int eraseLinesTet(TetGame *tetg); // Удаляет заполненные строки и подсчитывает количество строк 
TetFigure *createTetFigure(TetGame *tetg); // Создание и инициализация фигуры
void freeTetFigure(TetFigure *tf);
void dropNewFigure(TetGame *tetg); // Выброс новой фигуры на поле
TetFigure *rotTetFigure(TetGame *tetg); // Выполняет вращение фигуры
void calculateTet(TetGame *tetg);// Прототип ПРОЦЕДУРЫ, рассчитывающий один игровой ТАКТ одного игрового ЦИКЛА (вызыввается примерно 30 РАЗ в секунду)
void printTetGame(TetGame *tetg);

#endif  // TETRIS_H