#include "fsm.h"

// This is a finite state machine realisation based on switch-case statement.
/* 
    Function sigact() describes state switching logic. 
    States are checked in order specified in function sigact().
    It enters a state-case which it corresponds to, where begins another switch-case statement.
    Inner switch-case statement is looking for a signal given by get_signal().
    After finding it makes some action and switches state to the next one.

    Pros: 
        1) Less memory usage.
    Cons: 
        1) A lot of codelines.
*/
// Преобразует пользовательский ввод в соответствующий сигнал для управления игрой. Преобразует низкоуровневые коды клавиш в более абстрактные сигналы, которые могут быть легко обработаны остальной частью игровой логики.
signals get_signal(int user_input) // Принимает user_input: целое число, представляющее ввод пользователя
{
    signals rc = NOSIG; // Заводим переменную rc и инициализируем состоянием NOSIG (обозначает отсутствие сигнала)

    if (user_input == KEY_UP) // Если пользователь нажал стрелку вверх (KEY_UP)
        rc = MOVE_UP; // , устанавливает rc в MOVE_UP
    else if (user_input == KEY_DOWN) // Если пользователь нажал стрелку вниз (KEY_DOWN)
        rc = MOVE_DOWN; // , устанавливает rc в MOVE_DOWN
    else if (user_input == KEY_LEFT) // Если пользователь нажал стрелку влево (KEY_LEFT)
        rc = MOVE_LEFT; // , устанавливает rc в MOVE_LEFT
    else if (user_input == KEY_RIGHT) // Если пользователь нажал стрелку вправо (KEY_RIGHT)
        rc = MOVE_RIGHT; // , устанавливает rc в MOVE_RIGHT
    else if (user_input == ESCAPE) // Если пользователь нажал клавишу Escape (ESCAPE)
        rc = ESCAPE_BTN; // , устанавливает rc в ESCAPE_BTN
    else if (user_input == ENTER_KEY) // Если пользователь нажал клавишу Enter (ENTER_KEY)
        rc = ENTER_BTN; // , устанавливает rc в ENTER_BTN

    return rc;
}
// Перемещение жабы ВВЕРХ, с очисткой предыдущих координат жабы
void moveup(player_pos *frog_pos) // принимает указатель на структуру player_pos, с текущей позицией жабы
{
    if (frog_pos->y != 1) // проверка на самму ВЕРХНЮЮ позицию на поле для жабы
    {
        CLEAR_BACKPOS(frog_pos->y, frog_pos->x); // очистка предыдущей позиции жабы функкцией mvaddch() из библиотеки ncurses
        frog_pos->y -= 2; // уменьшает вертикальную координату на 2 - таким образом перемещает жабу вверх на одну ячейку (изменеие на 2, а не на 1, т.к. ячейки 2х2)
    }
}
// Перемещение жабы ВНИЗ, с очисткой предыдущих координат жабы
void movedown(player_pos *frog_pos) // принимает указатель на структуру player_pos, с текущей позицией жабы
{
    if (frog_pos->y != BOARD_N) // проверка на самму НИЖНЮЮ позицию на поле для жабы
    {
        CLEAR_BACKPOS(frog_pos->y, frog_pos->x); // очистка предыдущей позиции жабы функкцией mvaddch() из библиотеки ncurses
        frog_pos->y += 2; // увеличивает вертикальную координату на 2 - таким образом перемещает жабу вниз на одну ячейку (изменение на 2, а не на 1, т.к. ячейки 2х2)
    }
}
// Перемещение жабы ВПРАВО, с очисткой предыдущих координат жабы
void moveright(player_pos *frog_pos) // принимает указатель на структуру player_pos, с текущей позицией жабы
{
    if (frog_pos->x != BOARD_M) // проверка на саммую ПРАВУЮ позицию на поле для жабы
    {
        CLEAR_BACKPOS(frog_pos->y, frog_pos->x); // очистка предыдущей позиции жабы функкцией mvaddch() из библиотеки ncurses
        frog_pos->x++; // Увеличивает горизонтальную координату лягушки на 1 единицу, что перемещает ее вправо
    }
}
// Перемещение жабы ВЛЕВО, с очисткой предыдущих координат жабы
void moveleft(player_pos *frog_pos) // принимает указатель на структуру player_pos, с текущей позицией жабы
{
    if (frog_pos->x != 1) // проверка на саммую ЛЕВУЮ позицию на поле для жабы
    {
        CLEAR_BACKPOS(frog_pos->y, frog_pos->x); // очистка предыдущей позиции жабы функкцией mvaddch() из библиотеки ncurses
        frog_pos->x--; // Уменьшает горизонтальную координату лягушки на 1 единицу, что перемещает ее влево
    }
}
// Определяет, как игра должна реагировать на различные пользовательские действия в начальном состоянии
void on_start_state(signals sig, frog_state *state) // Принимает текущий сигнал (например, нажатие клавиши) И указатель на текущее состояние игры
{
    switch (sig) // в зависимости от тек. сигнала выполняется определённая реакция
    {
        case ENTER_BTN:
            *state = SPAWN; // если пользователь нажал Enter - то SPAWN - как он работает в коде нет
            break;
        case ESCAPE_BTN: 
            *state = EXIT_STATE; // если пользователь нажал Escape - то EXIT_STATE - как он работает в коде нет
            break;
        default:
            *state = START; // любые остальные сигналы приводят к состоянию START
            break;
    }
}

// Подготовка игры к новому уровню или завершению игры (проверка макс. уровня, загрузка нового уровня)
void on_spawn_state(frog_state *state, game_stats_t *stats, board_t *map, player_pos *frog_pos) // Принимает указателИ на: 1.текущую ФАЗУ игры 2.статистику игры 3.состояние ПОЛЯ игры 4.Позицию жабы
{
    if (stats->level > LEVEL_CNT) // если уровень игры больше максимального уровня 
        *state = GAMEOVER; // - то state становится GAMEOVER
    else
        if (!lvlproc(map, stats)) // вызов функции lvlproc() с передачей уровня игры (карты и статистики) из файла с уровнем
        {
            fill_finish(map->finish); // инициализация линии финиша игры
            print_finished(map); // вывод линии финиша игры в консоль
            frogpos_init(frog_pos); // инициализация НАЧАЛЬНОЙ позиции жабы
            *state = MOVING; // смена состояния на MOVING
        }
        else
            *state = FILE_ERROR_STATE; // если файл с уровнем НЕ открыт - FILE_ERROR_STATE
}
// Обеспечивает РЕАКЦИЮ на ввод сигнала MOVE_UP, MOVE_DOWN, MOVE_RIGHT, MOVE_LEFT, ESCAPE_BTN
void on_moving_state(signals sig, frog_state *state, board_t *map, player_pos *frog_pos) // Принимает указателИ на: 1.текущий СИГНАЛ 2.Указатель на тек. состояние игры 3.Указатель на структуру ПОЛЯ игры 4.Указатель на Позицию жабы
{
    switch (sig) // В зависимости от тек. сигнала выполняется определённая реакция
    {
        case MOVE_UP:
            moveup(frog_pos); // Перемещает жабу ВВЕРХ
            break;
        case MOVE_DOWN:
            movedown(frog_pos); // Перемещает жабу ВНИЗ
            break;
        case MOVE_RIGHT:
            moveright(frog_pos); // Перемещает жабу ВПРАВО
            break;
        case MOVE_LEFT:
            moveleft(frog_pos); // Перемещает жабу ВЛЕВО
            break;
        case ESCAPE_BTN: 
            *state = EXIT_STATE; // Устанавливает состояние выхода
            break;
        default:
            break;
    }
// Обязательно объясни за этот цикл!!!    
    if (*state != EXIT_STATE) // если состояние EXIT_STATE
    {
        if (check_collide(frog_pos, map)) // проверка на столкновение жабы с объектом
            *state = COLLIDE; // установка состояния в структуре frog_state, если столкновение БЫЛО
        else if (check_finish_state(frog_pos, map)) // если нет - проверка на финишную линию
            *state = REACH; // установка состояния в структуре frog_state, если ФИНИШНАЯ ЛИНИЯ БЫЛА ДОСТИГНУТА
        else
            *state = SHIFTING; // Если нет, устанавливает состояние в SHIFTING (сдвиг игрового поля)
    }
}
// Обеспечивает постоянное движение игрового поля и ОБНОВЛЕНИЕ ИНФОРМАЦИИ НА ЭКРАНЕ
void on_shifting_state(frog_state *state, game_stats_t *stats, board_t *map, player_pos *frog_pos) // Структура Тек. ФАЗЫ игры; Статистика игры; Состояние поля; Позиция жабы.
{
    shift_map(map); // сдвигает элементы игрового поля вниз

    if (check_collide(frog_pos, map)) // проверка на столкновение жабы с объектом
        *state = COLLIDE; // установка состояния в структуре frog_state
    else // если столкновения не произошло, ТО: 
    {
        *state = MOVING; // установка состояния в структуре frog_state 
        print_board(map, frog_pos); // отображение игрового поля на экране с УЧЁТОМ НОВОЙ ПОЗИЦИИ ЖАБЫ
        print_stats(stats); // вывод ОБНОВЛЁННОЙ статистики на экране (числа)
    }
}
// Вызывается, когда жаба достигает ФИНИШНОЙ линии, и определяет дальнейшее развитие игры в зависимости от того, завершен ли текущий уровень
void on_reach_state(frog_state *state, game_stats_t *stats, board_t *map, player_pos *frog_pos) // принимает 1.указатель на текущее состояние игры; 2.указатель на статистику игры; 3.указатель на состояние поля; 4.указатель на позицию жабы
{
    stats->score += 1; // устанавливает в структуру stats переменную score на значение score + 1 ПРИ ДОСТИЖЕНИИ ИГРОКОМ ФИНИШНОЙ ЛИНИИ
    add_proggress(map); // обновляет прогресс игрока на финишной линии

    if (check_level_compl(map)) // Проверяет, завершен ли текущий уровень (заполнена ли вся финишная линия)
    {// Если да, то: 
        stats->level++; // Увеличивает уровень на 1 (увеличивается счётчик уровня)
        stats->speed++; // Увеличивает скорость на 1 (увеличивается счётчик скорости)
        *state = SPAWN; // устанавливает состояние в структуру frog_state на SPAWN (начало нового уровня)
    }
    else
    {// Если нет, то: 
        frogpos_init(frog_pos); // инициализирует начальную позицию жабы
        print_finished(map); // обновляет отображение финишной линии
        *state = MOVING; // Устанавливает состояние игры в MOVING, что означает, что жаба может продолжить движение.
    }
}
// Отвечает за обработку столкновения жабы с препятствиями или другими объектами на игровом поле
void on_collide_state(frog_state *state, game_stats_t *stats, player_pos *frog_pos) // принимает 1.указатель на текущее состояние игры; 2.указатель на статистику игры; 3.указатель на позицию жабы
{
    if (stats->lives) // проверка на наличие жизней в структуре game_stats_t
    {
        stats->lives--; // счётчик по снижению жизней на 1 в структуре game_stats_t
        frogpos_init(frog_pos); // инициализирует начальную позицию жабы
        *state = MOVING; // устанавливает состояние в MOVING, что означает, что жаба может продолжить движение.
    }
    else // ИЛИ ЖИЗНИ НЕТУ
        *state = GAMEOVER; // Устанавливает состояние в GAMEOVER, что означает, что игра закончилась - ВЫХОД из программы
}
// Обеспечивает определённую РЕАКЦИЮ на сигналы (ввод пользователя, тайминг и т.д.), в зависимости от состояния игры НА ДАННЫЙ МОМЕНТ
void sigact(signals sig, frog_state *state, game_stats_t *stats, board_t *map, player_pos *frog_pos) // Структура сигнала; Тек. ФАЗЫ игры; Статистика игры; Состояние поля; Позиция жабы.
{
    switch (*state) // В зависимости от тек. состояния игры выполняется определённая реакция
    {
        // Реализация конечного афтомата (его части) 
        case START:
            on_start_state(sig, state); // определяет реакцию на действия пользователя в начальном состоянии игры
            break;
        case SPAWN:
            on_spawn_state(state, stats, map, frog_pos); // Подготовка игры к новому уровню или завершению игры
            break;
        case MOVING:
            on_moving_state(sig, state, map, frog_pos); // Обеспечивает РЕАКЦИЮ на ввод ПОЛЬЗОВАТЕЛЕМ сигналов MOVE_UP, MOVE_DOWN, MOVE_RIGHT, MOVE_LEFT, ESCAPE_BTN
            break;
        case SHIFTING:
            on_shifting_state(state, stats, map, frog_pos); // Обеспечивает постоянное движение игрового поля и ОБНОВЛЕНИЕ ИНФОРМАЦИИ НА ЭКРАНЕ
            break;
        case REACH:
            on_reach_state(state, stats, map, frog_pos); // Вызывается, когда жаба достигает ФИНИШНОЙ линии, и определяет дальнейшее развитие игры в зависимости от того, завершен ли текущий уровень
            break;
        case COLLIDE:
            on_collide_state(state, stats, frog_pos); // Отвечает за обработку столкновения жабы с препятствиями или другими объектами на игровом поле
            break;
        case GAMEOVER:
            print_banner(stats); // Отвечает за отображение баннера на экране
            break;
        default:
            break;
    }
}