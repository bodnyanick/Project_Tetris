#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ncurses.h>
#include <time.h>

#include "tetris.h"
// Глобальная переменная с шаблонами фигур
TetBlock tet_templates[] = { // 0 - пустой блок. 1 - НЕ пустой блок
    {0}, {0}, {1}, {0}, {0},
    {0}, {0}, {1}, {0}, {0},
    {0}, {0}, {1}, {0}, {0},
    {0}, {0}, {1}, {0}, {0},
    {0}, {0}, {1}, {0}, {0},

    {0}, {0}, {0}, {0}, {0},
    {0}, {0}, {1}, {0}, {0},
    {0}, {1}, {1}, {1}, {0},
    {0}, {0}, {0}, {0}, {0},
    {0}, {0}, {0}, {0}, {0},

    {0}, {0}, {0}, {0}, {0},
    {0}, {0}, {1}, {1}, {0},
    {0}, {0}, {1}, {0}, {0},
    {0}, {0}, {1}, {0}, {0},
    {0}, {0}, {0}, {0}, {0},

    {0}, {0}, {0}, {0}, {0},
    {0}, {1}, {1}, {0}, {0},
    {0}, {0}, {1}, {0}, {0},
    {0}, {0}, {1}, {0}, {0},
    {0}, {0}, {0}, {0}, {0},

    {0}, {0}, {0}, {0}, {0},
    {0}, {0}, {1}, {1}, {0},
    {0}, {1}, {1}, {0}, {0},
    {0}, {0}, {0}, {0}, {0},
    {0}, {0}, {0}, {0}, {0},

    {0}, {0}, {0}, {0}, {0},
    {0}, {1}, {1}, {0}, {0},
    {0}, {0}, {1}, {1}, {0},
    {0}, {0}, {0}, {0}, {0},
    {0}, {0}, {0}, {0}, {0},
};
// отобржаение игровой ситуации на экране
void printTetGame(TetGame *tetg){
    TetField *tf = tetg->field; // Переменная поля игры
    TetFigure *t = tetg->figure; // Переменная фигуры на поле игры
    for(int i = 0; i<tf->height; i++){
        for(int j=0; j<tf->width; j++){
            int sym = 1;
            if(tf->blocks[i*tf->width+j].b != 0){ // если текущй блок поля НЕ пуст
                sym = 2; // вывод на экран
            } else { // либо определяю
                int x = j-t->x; // располагается ли блок ...
                int y = i-t->y; // ... по данным координатам
                if(x >= 0 && x <t->size && y >= 0 && y < t->size) // есди текущие координаты соответствуют блоку в области фигуры
                    if(t->blocks[y*t->size+x].b != 0) // выполняю проверку блока на пустоту
                        sym = 2;
            }
            attron(COLOR_PAIR(sym));
            mvaddch(i, j, ' ');
            attroff(COLOR_PAIR(sym));
// printf("%d", sym); // после всех проверок - вывод на экран
        }
    }
// fflush(stdout); // очистка буфера вывода, для немедленного отображения результата
}
// int argc, char *argv[]
// int field_width, int field_height, int figures_size, int count, TetBlock *figures_template
int main(){

    struct timespec sp_start, sp_end, ts1, ts2 = {0, 0}; // структура для фиксации моментов времени

    initscr();
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_MAGENTA);
    init_pair(2, COLOR_GREEN, COLOR_GREEN);
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    scrollok(stdscr, TRUE);

    TetGame *tetg = createTetGame(15, 20, 5, 6, tet_templates);

// int make_des_left = rand() % 30;

    TetPlayer player; // Структура указания действий игрока
    player.action = TET_PLAYER_NOP; // Считаем, что игрок бездействует
    tetg->player = &player; // Данные о действиях игрока передаю в основную структуру
    dropNewFigure(tetg); // Выброс новой фигуры на поле
// Основной игровой цикл. Действует, пока игра не завершена
    while(tetg->playing != TET_GAMEOVER){
        clock_gettime(CLOCK_MONOTONIC, &sp_start); // Получаем текущее время

        int ch = getch();
        switch(ch) {
            case 'w':
                player.action = TET_PLAYER_UP;
                break;
            case 's':
                player.action = TET_PLAYER_DOWN;
                break;
            case 'a':
                player.action = TET_PLAYER_LEFT;
                break;
            case 'd':
                player.action = TET_PLAYER_RIGHT;
                break;
            default:
                player.action = TET_PLAYER_NOP; // Считаем, что игрок бездействует
                break;
        }

/*if(make_des_left <= 0){
            make_des_left = rand() % 30;
            player.action = rand () % 5;
            }*/ // 5 const 
        calculateTet(tetg); // Рассчитываю один игровой такт
        printTetGame(tetg); // Вызываю процедуру по отобржаению игровой ситуации на экран

        attron(COLOR_PAIR(1));
        mvprintw(0, 0, "Score: %d", tetg->score);
        attroff(COLOR_PAIR(1));
        move(tetg->field->height + 1, tetg->field->width + 1);

        refresh();

// make_des_left--;
        clock_gettime(CLOCK_MONOTONIC, &sp_end); // Получаем текущее время
        if(sp_end.tv_sec - sp_start.tv_sec <= 0 && (ts2.tv_nsec = 33000000 - (sp_end.tv_nsec - sp_start.tv_nsec)) >0 ) //
        nanosleep(&ts2, &ts1); //
    }
    freeTetGame(tetg); // При завершении освобождается память от основной структуры
    endwin();

    return 0;
}