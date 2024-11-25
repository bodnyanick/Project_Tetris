#ifndef FROGGER_FRONTEND_H
#define FROGGER_FRONTEND_H

#include <string.h>
#include "defines.h"
#include "objects.h"

void print_overlay(void); // Выводит рамку игры.
void print_levelerror(void); // Выводит ошибку уровня.
void print_rectangle(int top_y, int bottom_y, int left_x, int right_x); // Рисует прямоугольник.
void print_stats(game_stats_t *stats); // Выводит статистику игры.
void print_board(board_t *game, player_pos *frog); // Выводит игровое поле.
void print_cars(board_t *game); // Выводит движущиеся машины.
void print_finished(board_t *game); // Выводит линию финиша.
void print_banner(game_stats_t *stats); // Выводит баннер игры.
int read_banner(game_stats_t *stats, banner_t *banner); // Читает баннер из файла.

#endif