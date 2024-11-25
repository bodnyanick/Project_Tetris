#ifndef DEFINES_H
#define DEFINES_H

// макрос инициализации окна для работы с бибилиотекой ncurses
#define WIN_INIT(time) {\
                             initscr();\
                             noecho();\
                             curs_set(0);\
                             keypad(stdscr, TRUE);\
                             timeout(time);\
                        }
// инициализация окна ncurses
// отключение ввода символов на экран пользователем 
// отключение видимости курсора
// поддержка клавиатуры для стрелок и т.д.
// установка времени ожидания для ввода символов пользователем

#define GET_USER_INPUT getch()

#define PRINT_FROG(x, y) mvprintw(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), "@") // макрос для вывода жабы в нужной позиции на игровом поле. Сначала BOARDS_BEGIN + (frog->y): Вычисляется вертикальная координата. Затем BOARDS_BEGIN + (frog->x): Вычисляется горизонтальная координата. Функция mvprintw выводит символ "@" в вычисленные координаты на экране
#define MVPRINTW(y, x, ...) mvprintw(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), __VA_ARGS__) // макрос вывода текста на экран с помощью идентификатора __VA_ARGS__ в C, который представляет все аргументы, переданные после y и x. Когда макрос вызывается, например, MVPRINTW(2, 3, "Hello"), он расширяется в: mvprintw(BOARDS_BEGIN + 2, BOARDS_BEGIN + 3, "Hello")
#define MVADDCH(y, x, c) mvaddch(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), c) // макрос оборачивает функцию mvaddch(добавляет символ в окно на заданных координатах) из библиотеки ncurses - что он по факту делает? "Смещает координаты" 
#define CLEAR_BACKPOS(y, x) mvaddch(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), ' ') // макрос создания динамического отображения игрового поля, включая очистку предыдущей позиции объекта, за счёт функции mvaddch(), которая добавляет символ в окно на заданных координатах (BOARDS_BEGIN + (y), BOARDS_BEGIN + (x)), и вставляет пробел для очистки позиции

#define YOU_WON "tests/game_progress/you_won.txt"
#define YOU_LOSE "tests/game_progress/you_lose.txt"
#define LEVEL_DIR "tests/levels/level_"
#define INTRO_MESSAGE "Press ENTER to start!"
#define INTRO_MESSAGE_LEN     21
#define LEVEL_CNT 5 // макрос для указания максимального количества уровней в игре
#define LEVELNAME_MAX 25 // максимальное количество символов в имени файла с уровнем

#define MAX_WIN_COUNT    10

#define ROWS_MAP 21 // строки карты
#define COLS_MAP 90 // столбцы карты

#define BOARDS_BEGIN 2 // отступ от края окна

#define FROGSTART_X      (BOARD_M / 2) // начальная позиция жабы - посередине карты по горизонтали
#define FROGSTART_Y      (BOARD_N) // нижняя граница карты по вертикали
#define INITIAL_TIMEOUT  150 // таймаут

#define BOARD_N     (ROWS_MAP + MAP_PADDING * 2) // высота игрового поля
#define BOARD_M     30 // ширина игрового поля
#define HUD_WIDTH   12 // Ширина области отображения статистики (HUD)
#define MAP_PADDING 3 // толщина отступов вокруг карты

#define BANNER_N    20  // Высота баннера
#define BANNER_M    100  // Ширина баннера

#define SUCCESS   0 // Успешное завершение функции
#define ERROR     1 // Ошибка в функции

#define NO_INPUT -1

#define ESCAPE 27
#define ENTER_KEY 10

#endif