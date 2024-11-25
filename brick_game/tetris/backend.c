#include "backend.h"

/** Обновление текущего состояния игры
 * Эта функция возвращает структуру,
 * которая содержит текущее состояние игры
 */
// Функция updateCurrentState даёт возможность получить текущие данные и работать с ними, не трогая данные, которые остаются в stateWrapper.
GameInfo_t updateCurrentState() {
  GameInfo_t *state = stateWrapper();
  return *state; 
}

/** Состояния игры при первом вызове
 * Выделяет память для поля игры и следующей фигуры
 * и начальные значения
 */
GameInfo_t *stateWrapper() { // stateWrapper возвращает указатель на уже существующий объект
  static GameInfo_t state[] = {0};
  if (state->field == NULL) {
    state->field = mallocMatrix(WIDTH, HEIGHT);
    state->next = mallocMatrix(MAX_BLOCKS, MAX_BLOCKS);
    state->score = 0;
    state->high_score = 0;
    state->level = 1;
    state->speed = 1000;
    state->pause = 0;
  }
  return state;
}

/** Обновляет текущую фигуру.
 * Вовращает показатели текущей фигуры
 */
Figures *updateCurrentFigure(bool flag) {
  static Figures figure[] = {0}; // Создаётся пустой массив и заполняет его нулями
  if (flag) { // если флаг flag равен true
    *figure = getRandomFigure();
    addFigure();
  }
  return figure;
}

/** Обновление следующей фигуры
 * Возвращает показатели следующей фигуры
 */
Figures *updateNextFigure(bool flag) {
  static Figures next[] = {0};
  if (flag) {
    *next = getRandomFigure();
    addNext(next);
  }
  return next;
}

/** обрабатывает действия игрока в зависимости от полученного состояния (action) 
 */
void userInput(UserAction_t action, bool hold) {
  if (hold) { // Если hold == true. Возвращает управление, не выполняя дальнейших действий.
    return;
  }
  GameInfo_t *state = stateWrapper();
  state->speed -= 100;
  if (state->speed <= 0) {
    moveDown(true);
  }
  chtype ch = 0;
  switch (action) {
    case Start:
      mvprintw(5, 5, "Press ENTER to start!");
      mvprintw(6, 10, "q to quit!");
      while (ch != '\n' && ch != 'q') {
        ch = getch();
      }
      if (ch == 'q') {
        exit(EXIT_SUCCESS);
      }
      clear();
      refresh();
      break;
    case Pause:
      attron(A_BLINK);
      attron(COLOR_PAIR(8));
      mvprintw(5, 1, "PAUSE");
      attroff(COLOR_PAIR(8));
      attroff(A_BLINK);
      while (ch != 'p' && ch != 'q') {
        ch = getch();
      }
      if (ch == 'q') {
        exit(EXIT_SUCCESS);
      }
      clear();
      refresh();
      break;
    case Terminate:
      return;
    case Left:
      moveLeft();
      break;
    case Up:
      rotate();
      break;
    case Right:
      moveRight();
      break;
    case Down:
      moveDown(false);
      break;
    case Action:
      while (!moveDown(false)) continue;
      break;
    default:
      break;
  }
}

/** Парсит клавиши */
UserAction_t isPress(int key) {
  UserAction_t action = -1; // Отсутствует действие
  switch (key) {
    case 'p':
      action = Pause;
      break;
    case 'q':
      action = Terminate;
      break;
    case KEY_LEFT:
      action = Left;
      break;
    case KEY_RIGHT:
      action = Right;
      break;
    case KEY_UP:
      action = Up;
      break;
    case KEY_DOWN:
      action = Down;
      break;
    case ' ':
      action = Action;
      break;
    default:
      break;
  }

  return action;
}

// Добавление текущей фигуры на поле
void addFigure() {
  GameInfo_t state = updateCurrentState();

  Figures *figure = updateCurrentFigure(false);
  choosedFig shape = shapes();
  for (int x = 0; x < MAX_BLOCKS; x++) {
    for (int y = 0; y < MAX_BLOCKS; y++) {
      if (shape[figure->piece][figure->rotation][x][y]) {
        state.field[figure->y + y][figure->x + x] = figure->piece + 1;
      }
    }
  }
}

/** Добавление следующей фигуры на поле
 * Добавляем следующую фигуру в обасть next
 */
void addNext(Figures *next) {
  GameInfo_t state = updateCurrentState();
//  Figures *next = updateNextFigure(false); //! обновляем следующую фигуру
  choosedFig shape = shapes();
  for (int x = 0; x < MAX_BLOCKS; x++) {
    for (int y = 0; y < MAX_BLOCKS; y++) {
      if (shape[next->piece][next->rotation][x][y]) {
        state.next[y][x] = next->piece + 1;
      }
    }
  }
}

/** Удаляет текущую фигуру с игрового поля
 *  перед её перемещением или поворотом */
void delFigure() {
  GameInfo_t state = updateCurrentState();
  Figures *figure = updateCurrentFigure(false);
  choosedFig shape = shapes();
  for (int x = 0; x < MAX_BLOCKS; x++) {
    for (int y = 0; y < MAX_BLOCKS; y++) {
      if (shape[figure->piece][figure->rotation][x][y]) {
        state.field[figure->y + y][figure->x + x] = 0;
      }
    }
  }
}

/** Удаляет следующию фигуру с области предпросмотра*/
void delNext() {
  GameInfo_t state = updateCurrentState();
  Figures *next = updateNextFigure(false);
  choosedFig shape = shapes();
  for (int x = 0; x < MAX_BLOCKS; x++) {
    for (int y = 0; y < MAX_BLOCKS; y++) {
      if (shape[next->piece][next->rotation][x][y]) {
        state.next[y][x] = 0;
      }
    }
  }
}

/** Перемещает фигуру вниз */
int moveDown(bool timeout) {
  int message = OK;
  delFigure();
  Figures *figure = updateCurrentFigure(false);
  figure->y++;
  if (isCollision()) {
    message = IS_FALL;
    figure->y--;
    addFigure();
    lineElimination();
    takeNextFigure();
  } else {
    addFigure();
  }

  GameInfo_t *state = stateWrapper();
  if (timeout) state->speed = 1300 * pow(0.8, state->level); 

  return message;
}

/** Перемещает фигуру влево */
void moveLeft() {
  delFigure();
  Figures *figure = updateCurrentFigure(false);
  figure->x--; // обеспечивает сдвиг
  if (isCollision()) figure->x++; // отменяет сдвиг, если произошло конфликт
  addFigure();
}

/** Перемещает фигуру вправо */
void moveRight() {
  delFigure();
  Figures *figure = updateCurrentFigure(false);
  figure->x++;
  if (isCollision()) figure->x--;
  addFigure();
}

/** Поворачивает фигуру
 *  сначала удаляет фигуру с текущего положения,
 *  затем пробует повернуть её,
 *  проверяя столкновения с границами поля
 *  или другими фигурами.
 */
void rotate() {
  delFigure();
  Figures *figure = updateCurrentFigure(false);
  int tmp_x = figure->x; // сохраняет текущее положение фигуры по оси X
  figure->rotation = (figure->rotation + 1) % MAX_BLOCKS; 

  if (isCollision() == R_BOUNCE)
    while (isCollision() == R_BOUNCE) figure->x--;  // толчок от правой границы
  if (isCollision() == L_BOUNCE)
    while (isCollision() == L_BOUNCE) figure->x++;  // толчок от левой границы
  if (isCollision() == COLLIDE) {
    figure->rotation = (figure->rotation - 1) % MAX_BLOCKS;  // увеличивает текущий поворот фигуры на 1 но не более 4
    figure->x = tmp_x;
  }
  addFigure();
}

/** Проверяет столкновения между фигурой
 * и границами поля или другими блоками на поле */
int isCollision() {
  GameInfo_t state = updateCurrentState();
  Figures *figure = updateCurrentFigure(false);
  choosedFig shape = shapes();
  for (int x = 0; x < MAX_BLOCKS; x++) {
    for (int y = 0; y < MAX_BLOCKS; y++) {
      if (shape[figure->piece][figure->rotation][x][y]) {
        if (figure->y + y >= HEIGHT ||
            state.field[figure->y + y][figure->x + x])
          return COLLIDE; // Нижняя граница поля
        if (figure->x + x >= WIDTH) return R_BOUNCE; // правая граница поля
        if (figure->x + x < 0) return L_BOUNCE; // левая граница поля
      }
    }
  }
  return OK;
}

/** Удаляет заполненные линий
 * Если строка полностью заполнена блоками, она удаляется,
 * а строки выше перемещаются вниз
 * После этого обновляется количество очков и уровень игры
 */
void lineElimination() {
  GameInfo_t *state = stateWrapper();

  int lines = 0; // счётчик удаленных линий
  for (int row = 0; row < HEIGHT; row++) {
    int col; 
    for (col = 0; col < WIDTH && state->field[row][col]; col++) 
      ;

    if (col < WIDTH) continue;
    for (int h = row; h > 2; h--) { 
      for (int k = 0; k < WIDTH; k++) {
        state->field[h][k] = state->field[h - 1][k];
      }
    }
    lines++;
  }
  const int linesPrice[MAX_BLOCKS + 1] = {0, 100, 300, 700, 1500};
  state->score += linesPrice[lines];

  if (state->level < 10) state->level = 1 + state->score / 600;
}

/** Выводит сообщение о завершении игры
 * и предлагает начать новую игру
 * Если набран новый рекорд, он сохраняется,
 * и предлагается продолжить или выйти. */
void promptNewGame() {
  if (isCollision()) {
    clear();
    GameInfo_t *info = stateWrapper();

    int x = 10;
    mvprintw(3, x + 1, "GAME OVER!");
    mvprintw(4, x, "You score: %d\n", info->score); 
    if (info->score >= info->high_score) mvprintw(5, x, "New record!"); 

    mvprintw(7, x - 3, "Start new game? (y/n)");

    if (info->high_score < info->score) info->high_score = info->score;

    info->score = 0;
    info->level = 1;
    chtype ch = 0;
    while ((ch = getch()) != 'y') {
      if (ch == 'n') exit(EXIT_SUCCESS);
    }

    for (int i = 0; i < WIDTH; i++)
      for (int j = 0; j < HEIGHT; j++) info->field[j][i] = 0;

    clear();
  }
}

/** переносит следующую фигуру на игровое поле */
void takeNextFigure() {
  delNext();
  Figures *next = updateNextFigure(false);
  Figures *figure = updateCurrentFigure(false);
  *figure = *next;

  promptNewGame(); // проверка на конец игры

  addFigure(); // Добавляет новую текущую фигуру (которая теперь является бывшей следующей) на игровое поле
  updateNextFigure(true); // Генерирует новую следующую фигуру и добавляет ее в область предпросмотра
}

/* выделяет память для матрицы */
int **mallocMatrix(int W, int H) {
  int error = OK; // Переменная для ошибок памяти, инициализируется нулем (константа из curses.h)
  int **matrix = calloc(H, sizeof(int *));
  if (matrix != NULL) {
    int *val_arr = calloc(H * W, sizeof(int));
    if (val_arr != NULL) {
      for (int i = 0; i < H; i++) {
        matrix[i] = val_arr + W * i;
      }
    } else {
      error++;
    }
  } else {
    error++;
  }

  if (error) {
    printf("ERROR: Can't allocate memory!\n");
    exit(EXIT_FAILURE);
  }

  return matrix;
}

/** освобождает память */
void clearMatrix() {
  GameInfo_t state = updateCurrentState();
  if (state.field != NULL) {
    if (state.field[MATRIX_PTR] != NULL) free(state.field[MATRIX_PTR]);
    free(state.field);
  }
  if (state.next != NULL) {
    if (state.next[MATRIX_PTR] != NULL) free(state.next[MATRIX_PTR]);
    free(state.next);
  }
}