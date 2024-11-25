#ifndef OBJECTS_H
#define OBJECTS_H

#include <ncurses.h>
#include "defines.h"
// Структура для хранения информации о позиции жабы
typedef struct
{
    int x; // горизонтальная координата
    int y; // вертикальная координата
} player_pos;

// Структура для хранения информации о состоянии поля игры
typedef struct
{
    char finish[BOARD_M + 2]; // Массив символов для отслеживания прогресса игрока по достижению цели (конца карты)
    char ways[ROWS_MAP + 2][COLS_MAP + 2]; // сама карта игрового поля
} board_t;

// Структура для хранения статистики игры
typedef struct
{
    int score; // счёт
    int level; // уровень
    int speed; // скорость
    int lives; // жизни
    int won; // ЕСЛИ победа
} game_stats_t;

// Структура для хранения информации о баннере, который используется для вывода сообщений о победе или поражении в игре
typedef struct
{
    char matrix[BANNER_N + 1][BANNER_M + 1]; // матрица для баннера
} banner_t;

#endif