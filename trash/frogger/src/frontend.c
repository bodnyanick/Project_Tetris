#include "frog_frontend.h"
// Что делает эта функция?
void print_overlay(void)
{
// Рамка игрового поля    
    print_rectangle(0, BOARD_N + 1, 0, BOARD_M + 1); // внутренняя рамка игрового поля
    print_rectangle(0, BOARD_N + 1, BOARD_M + 2, BOARD_M + HUD_WIDTH + 3); // внутренняя рамка с HUD (область для статистики)
// Рамка HUD (область для статистики), создание разделителей, 4 части - уровень, счет, скорость, жизни
    print_rectangle(1, 3, BOARD_M + 3, BOARD_M + HUD_WIDTH + 2);
    print_rectangle(4, 6, BOARD_M + 3, BOARD_M + HUD_WIDTH + 2);
    print_rectangle(7, 9, BOARD_M + 3, BOARD_M + HUD_WIDTH + 2);
    print_rectangle(10, 12, BOARD_M + 3, BOARD_M + HUD_WIDTH + 2);
// Заголовки в статистике (HUD)
    MVPRINTW(2, BOARD_M + 5, "LEVEL");
    MVPRINTW(5, BOARD_M + 5, "SCORE");
    MVPRINTW(8, BOARD_M + 5, "SPEED");
    MVPRINTW(11, BOARD_M + 5, "LIVES");
// Вывод сообщения о начале игры
    MVPRINTW(BOARD_N / 2, (BOARD_M - INTRO_MESSAGE_LEN) / 2 + 1, INTRO_MESSAGE);
}
// Вывод ошибки при не нахождении файлов поля
void print_levelerror(void)
{
    clear();
    MVPRINTW(0, 0, "An error occured openning level file!");
    MVPRINTW(2, 0, "Please check ./tests/ directory.");
    MVPRINTW(3, 0, "There should be 5 level files named level_(1-5).txt.");
    MVPRINTW(4, 0, "Also try to open the game nearby ./tests/ directory.");
    MVPRINTW(6, 0, "Press any key to exit.");
}
// функция для рисования прямоугольника на экране с использованием символов ASCII-art
void print_rectangle(int top_y, int bottom_y, int left_x, int right_x)
{
// ОРИСОВКА верха прямоугольника
    MVADDCH(top_y, left_x, ACS_ULCORNER); // левый верхний угол символами ASCII-art

    int i = left_x + 1;

    for (;i < right_x; i++)
        MVADDCH(top_y, i, ACS_HLINE); // верхняя горизонтальная линия из символов HLINE ASCII-art
    MVADDCH(top_y, i, ACS_URCORNER); // правый верхний угол ASCII-art
// ОТРИСОВКА боковых СТОРОН прямоугольника
    for (int i = top_y + 1; i < bottom_y; i++)
    {
        MVADDCH(i, left_x, ACS_VLINE); // левая вертикальная линия из символов VLINE ASCII-art
        MVADDCH(i, right_x, ACS_VLINE); // правая вертикальная линия из символов VLINE ASCII-art
    }
// ОТРИСОВКА нижней части прямоугольника
    MVADDCH(bottom_y, left_x, ACS_LLCORNER); // левый нижний угол ASCII-art
    i = left_x + 1;
    for (;i < right_x; i++)
        MVADDCH(bottom_y, i, ACS_HLINE); // нижняя горизонтальная линия из символов HLINE ASCII-art
    MVADDCH(bottom_y, i, ACS_LRCORNER); // правый нижний угол символами ASCII-art
}


/*
Эта функция использует специальные символы ASCII-art для рисования рамки прямоугольника:

ACS_ULCORNER: Левый верхний угол
ACS_HLINE: Горизонтальная линия
ACS_URCORNER: Правый верхний угол
ACS_VLINE: Вертикальная линия
ACS_LLCORNER: Левый нижний угол
ACS_LRCORNER: Правый нижний угол
*/


// Отображает СТАТИСТИКУ на экране - сами ЧИСЛА, надписи выше в коде
void print_stats(game_stats_t *stats) // принимает указатель на структуру статистики
{
    MVPRINTW(2, BOARD_M + 12, "%d", stats->level); // Эта строка выводит текущий УРОВЕНЬ игры с помощью функции mvprintw из библиотеки ncurses
    MVPRINTW(5, BOARD_M + 12, "%d", stats->score); // Эта строка выводит текущий СЧЁТ игры с помощью функции mvprintw из библиотеки ncurses
    MVPRINTW(8, BOARD_M + 12, "%d", stats->speed); // Эта строка выводит текущую СКОРОСТЬ игры с помощью функции mvprintw из библиотеки ncurses
    MVPRINTW(11, BOARD_M + 12, "%d", stats->lives); // Эта строка выводит текущие ЖИЗНИ игры с помощью функции mvprintw из библиотеки ncurses
}
// Создает полное визуальное представление текущего состояния игры, включая дорогу и положение жабы на ней. Обновляется отображения после каждого действия игрока
void print_board(board_t *game, player_pos *frog) // принимает указатель на текущее состояние игрового поля И указатель на позицию жабы
{
    print_cars(game); // Отображает дорогу с движущимися автомобилями
    PRINT_FROG(frog->x, frog->y); // Выводит жабу в ее текущей позиции на этой дороге
}
// Создает визуальное представление дороги с движущимися автомобилями
void print_cars(board_t *game) // принимает указатель на текущее состояние игрового поля
{
    for(int i = MAP_PADDING + 1; i < BOARD_N - MAP_PADDING + 1; i++) // проходит по всем строкам игрового поля, не считая отступов сверху и снизу (MAP_PADDING)
    {
        if (i % 2 == (MAP_PADDING + 1) % 2) // Проверка на чётность/нечётность путём деления с остатком
        {
            for (int j = 1; j < BOARD_M + 1; j++) // счётчик по вертикали работает, если ЧЁТНАЯ
                MVADDCH(i, j, ACS_BLOCK); // функция mvaddch() из константы MVADDCH выводит символы ACS_BLOCK в координатах (i, j)
        }
        else
        {
            for (int j = 1; j < BOARD_M + 1; j++) // счётчик по вертикали, если НЕЧЁТНАЯ
            {
                if (game->ways[i - MAP_PADDING - 1][j - 1] == '0') // это обращение к элементу массива ways в структуре game. i - MAP_PADDING - 1 и j - 1 используются для корректировки индексов, так как i и j начинаются с 1, а массивы в C начинаются с 0.
                    MVADDCH(i, j, ' '); // функция mvaddch() из константы MVADDCH вводит символ пробела в координаты [i,j], если там стоит '0'
                else
                    MVADDCH(i, j, ']'); // в другом случае вводится символ ']'
            }
        }
    }
}
// Отображает линию финиша игры (с использованием символов ASCII-art)
void print_finished(board_t *game) // принимает указатель на текущее состояние игрового поля (включая линию финиша игры)
{
    for (int i = 0; i < BOARD_M; i++) // сравнивает каждый элемент массива с константой BOARD_M (30 элементов)
    {
        if (game->finish[i] == '0') // если элемент массива не равен нулю
            MVADDCH(1, i + 1, ACS_BLOCK); // функция mvaddch() из константы MVADDCH выводит символ ACS_BLOCK в координатах (1, i + 1)
        else
            MVADDCH(1, i + 1, ' '); // в противном случае функция mvaddch() из константы MVADDCH выводит символ пробела в координатах (1, i + 1)
    }       
}
// Отвечает за отображение баннера на экране
void print_banner(game_stats_t *stats) // Принимает указатель на структуру статистики игры
{
    banner_t banner; // Инициализируется структура banner_t и заполняется нулями
// Функция memset заполняет указанную область памяти заданным значением, инициализируя или очищая блок данных.
    memset(banner.matrix, 0, (BANNER_N + 1) * (BANNER_M + 1)); // принимает указатель на область памяти и заполняет ее НУЛЯМИ, (BANNER_N + 1) * (BANNER_M + 1): это количество байтов, которые нужно заполнить

    clear(); // Очищает экран
// Читает баннер из файла и заполняет структуру баннера
    if (!(read_banner(stats, &banner))) // принимает структуру статистики игры и адрес на структуру баннера
    {
        for (int i = 0; i < BANNER_N; i++) // проходит по вертикали баннера
            for (int j = 0; j < BANNER_M; j++) // проходит по горизонтали баннера
                if (banner.matrix[i][j] == '#') // если увстретил символ '#'
                    MVADDCH(i, j, ACS_BLOCK); // наполняет символами ACS_BLOCK
                else
                    MVADDCH(i, j, ' '); // не встретил - наполняет пробелом
        refresh(); // обновляет экран, чтобы отобразить все изменения
        napms(20000); // задерживает выполнение программы на 2000 миллисекунд (2 секунды), чтобы игрок мог увидеть баннер из библиотеки ncurses
    }
}
// Отвечает за чтение баннера из файла, в зависимости от статуса игры
int read_banner(game_stats_t *stats, banner_t *banner) // Принимает указатель на статистику игры И указатель на структуру для хранения баннера
{
    int rc = SUCCESS; // Инициализирует флаг для проверки успешности чтения баннера
    FILE *file = NULL; // Инициализирует флаг для проверки успешности открытия файла

    if (stats->lives) // Если жизни не равны нулю
        file = fopen(YOU_WON, "r"); // Открывается этот файл
    else
        file = fopen(YOU_LOSE, "r"); // Если жизни равны нулю

    if (file) // Проверяется, успешно ли открыт файл
    {
        for (int i = 0; i < BANNER_N - 1 && !rc; i++) // Читает строки из файла и записывает их в матрицу баннера.
        {
            if (fgets(banner->matrix[i], BANNER_M + 2, file) == NULL) // fgets читает строку из файла
                rc = ERROR; // Если ошибка при чтении строки - ERROR
            else
                banner->matrix[i][strcspn(banner->matrix[i], "\n")] = '\0'; // удаляет символ новой строки в конце строки и ставит НУЛЬ-СИМВОЛ
        }

        fclose(file); // Закрывается файл
    }
    else
        rc = ERROR; // Если файл не удалось открыть, устанавливается код ошибки.

    return rc;
}