#ifndef FROGGER_BACKEND_H
#define FROGGER_BACKEND_H

#include <ncurses.h>
#include "defines.h"
#include "objects.h"
#include "string.h"

int lvlproc(board_t *map, game_stats_t *stats); // Обрабатывает уровень игры.
void add_proggress(board_t *map); // Обновляет прогресс игры.
void stats_init(game_stats_t *stats); // Инициализирует статистику.
void frogpos_init(player_pos *frog); // Инициализирует позицию жабы.
void fill_finish(char *finish_line); // Заполняет линию финиша.
void shift_map(board_t *map); // Сдвигает игровое поле.

bool check_collide(player_pos *frog, board_t *map); // Проверяет столкновение.
bool check_finish_state(player_pos *frog, board_t *map); // Проверяет финишное состояние.
bool check_level_compl(board_t *map); // Проверяет завершение уровня.

#endif