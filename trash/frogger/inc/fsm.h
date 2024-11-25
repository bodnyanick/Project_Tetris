#ifndef FSM_H
#define FSM_H

#include "defines.h"
#include "objects.h"
#include "frog_backend.h"
#include "frog_frontend.h"
// Структура ПЕРЕЧИСЛЕНИЯ (enumeration) ФАЗЫ игры для управления ЛОГИКОЙ игры и реагирования на события (действия игрока)
typedef enum // вид структуры для создания именованных констант
{
    START = 0,
    SPAWN,
    MOVING,
    SHIFTING,
    REACH,
    COLLIDE,
    GAMEOVER,
    EXIT_STATE,
    FILE_ERROR_STATE
} frog_state; // позволяет использовать наименования вместо чисел

// Структура ПЕРЕЧИСЛЕНИЯ (enumeration) для управления СОСТОЯНИЕМ и обработки СИГНАЛОВ в игре
typedef enum
{
    MOVE_UP = 0, // направление лягушки вверх
    MOVE_DOWN, // направление лягушки вниз
    MOVE_RIGHT, // направление лягушки вправо
    MOVE_LEFT, // направление лягушки влево
    ESCAPE_BTN, // это ESC - кнопка для выхода из игры
    ENTER_BTN, // это ENTER - кнопка для продолжения игры
    NOSIG // обозначает отсутствие сигнала
} signals;

//! Реализация конечно автомата
signals get_signal(int user_input); // Преобразует пользовательский ввод в соответствующий сигнал для управления игрой. Преобразует низкоуровневые коды клавиш в более абстрактные сигналы, которые могут быть легко обработаны остальной частью игровой логики.
void sigact(signals sig, frog_state *state, game_stats_t *stats, board_t *map, player_pos *frog_pos); // Обеспечивает определённую РЕАКЦИЮ на сигналы (ввод пользователя, тайминг и т.д.), в зависимости от состояния игры НА ДАННЫЙ МОМЕНТ

#endif