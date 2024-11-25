#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ncurses.h>

#include "tetris.h"

#ifndef TETRIS_C
#define TETRIS_C

// Алгоритм инициализации набора шаблонов фигур
// Принимает количество шаблонов фигур; размер области фигуры; сведения о блоках, соответствующих набору шаблонов.
TetFiguresT *createTetFiguresT(int count, int figures_size, TetBlock *figures_template){
    // Выделяю память для для описания типовых фигур
    TetFiguresT *tetft = (TetFiguresT *)malloc(sizeof(TetFiguresT));
    tetft->count = count; // Количество типовых фигур (количество шаблонов)
    tetft->size = figures_size; // Размер стороны квадратной области, на которой изображена фигура (Размер области фигуры)
    tetft->blocks = figures_template; // Считаю, что массив блоков для серии типовых фигур (шаблонов фигур) определён заранее
    return tetft;
}
// Для каждой функции инициализации необходимо создать соответствующую функцию ОЧИСТКИ ПАМЯТИ
void freeTetFiguresT(TetFiguresT *tetft){
    if(tetft)
        free(tetft);
}

// Создание поля для игры
TetField *createTetField(int width, int height){
// Выделение памяти для описания будущего игрового поля
    TetField *tetf = (TetField*)malloc(sizeof(TetField));
// Установка ширины и высоты поля
    tetf->width = width;
    tetf->height = height;
// Выделение памяти для хранения информации о блоках. Её объём будет равен произведению объёма структуры TetBlock на ширину и высоту поля
    tetf->blocks = (TetBlock*)malloc(sizeof(TetBlock) * width * height);
// Будем считать, что на поле блоков нет. По этому параметр "b" из струкутры TetBlock будет равен нулю для ВСЕХ блоков
    for (int i = 0; i < width * height; i++)
    tetf->blocks[i].b = 0;
    return tetf;
}

// Для каждой функции инициализации необходимо создать соответствующую функцию ОЧИСТКИ ПАМЯТИ (для поля память выделили ДВА РАЗА - два маллока - для описания поля(высота и ширина), для набора блоков). Освождаю память В ОБРАТНОМ ПОРЯДКЕ
void freeTetField(TetField *tetf){
    if(tetf) {
        if(tetf->blocks)
            free(tetf->blocks);
        free(tetf);
    }
}

// 6. Прототип функции, инициализирующей основную игровую структуру.
// Принимает в качестве параметров ширину и высоту поля; размер ОБЛАСТИ фигуры; количество шаблонов фигур; сведения о блоках, соответствующих набору шаблонов.
TetGame *createTetGame(int field_width, int field_height, int figures_size, int count, TetBlock *figures_template){ 
// Выделение памяти для основной структуры игры
    TetGame *tetg = (TetGame*)malloc(sizeof(TetGame));
// Предположим что уже есть функции инициализации переменных основной структуры игры. Аргументы указаны исходя из определения структур поля и набора шаблонов
    tetg->field = createTetField(field_width, field_height); // принимает размеры
// Принимает количество шаблонов; размер области фигуры; информацию о блоках
    tetg->figurest = createTetFiguresT(count, figures_size, figures_template);
    tetg->ticks = TET_TICKS_START;
    tetg->ticks_left = TET_TICKS_START;
    tetg->score = 0;
    tetg->playing = TET_PLAYING;
// Возвращаю указатель на инициализированную ОСНОВНУЮ структуру игры как результат действия функции createTetGame
return tetg;
}
// Очистка памяти для TetGame *createTetGame
void freeTetGame(TetGame *tetg){
    if(tetg) {
        freeTetField(tetg->field);
        freeTetFiguresT(tetg->figurest);
        free(tetg);
    }
}

//! Реализация функции из одного игрового ТАКТА
// Перемещение фигуры по стрелкам
void moveFigureDown(TetGame *tetg){
    tetg->figure->y++; // счётчик перемещения по оси Y
}
void moveFigureUp(TetGame *tetg){
    tetg->figure->y--; // счётчик перемещения по оси Y
}
void moveFigureRight(TetGame *tetg){
    tetg->figure->x++; // счётчик перемещения по оси X
}
void moveFigureLeft(TetGame *tetg){
    tetg->figure->x--; // счётчик перемещения по оси X
}
// Размещение фигуры после падения
// Вызывается ПОСЛЕ обработки столкновения (т.е. у фигуры нет нулевых блоков, вышедших за пределы поля)
void plantFigure(TetGame *tetg){
    TetFigure *t = tetg->figure; // переменная ссылающая на падающую фигуру
    for(int i =0; i < t->size; i++) // перемещаю все блоки падающей фигуры на поле
        for(int j = 0; j < t->size; j++)
            if (t->blocks[i*t->size+j].b != 0){ // Если данный блок фигуры НЕ пуст - рассчитываю его координаты в игровом поле:
                int fx = t->x + j; // координаты по X
                int fy = t->y + i; // координаты по Y
                tetg->field->blocks[fy*tetg->field->width+fx].b = t->blocks[i*t->size+j].b; // помещаю блок фигуры на поле
            }
}
// Проверка на столкновение
int collisionTet(TetGame *tetg){
    TetFigure *t = tetg->figure; // переменная ссылающаяся на падающую фигуру 
    TetField *tf = tetg->field; // переменная ссылающаяся на игровое поле
    for (int i = 0; i < t->size; i++) // Считаю, что столкновение может произойти только ...
        for (int j = 0; j < t->size; j++){ // ... в области отрисовки фигуры
 // т.к. все двумерные массивы хранятся в одномерных массивах, пишу формулу расчёта индекса массива по двумерным координатам (i это Y, j это X)
            if (t->blocks[i*t->size+j].b != 0){ // Если данный блок фигуры НЕ пуст - рассчитываю его координаты в игровом поле:
                int fx = t->x + j; // координаты по X
                int fy = t->y + i; // координаты по Y
                // (fx < 0 || fx >= tf->width || fy < 0 || fy >= tf->height)
                if(tf->blocks[fy*tf->width+fx].b != 0){ // Если в том же месте на поле есть НЕ ПУСТОЙ блок, значит произошло столкновение
                    return 1;
                    } // Значит верну ОДЫН
                if(fx<0 || fx >= tf->width || fy<0 || fy >= tf->height){ // Если произошло столкновение с границами поля
                    return 1;
                }
            }
        }
    return 0; // Если столкновений нет - возвращаю 0
}
// Проверка на заполненность строки
int lineFilledTet(int i, TetField *tfl){
    for(int j=0; j<tfl->width; j++){ // Строчка вертикали
        if(tfl->blocks[i*tfl->width+j].b == 0){ // если ОДИН ИЗ блоков = 0 ...
            return 0; // ... ТО возвращаю ноль
            }
        }
        return 1; // есди нулей нет - вернуть 1
}
// Сдвиг блоков вниз на единицу (сдвиг всех НЕ ПУСТЫХ строк)
void dropLineTet(int i, TetField *tfl){
    if(i == 0){ // Если строка нулевая
        for(int j=0; j<tfl->width; j++) // Очищаю её циклом
            tfl->blocks[j].b = 0; // Приравнивая к нулю все блоки (по оси X )
        }else{ // Либо
            for(int k=i; k>0; k--) // переношу ВСЕ НЕПУСТЫЕ блоки вниз на единицу (определяю по оси X)
                for(int j=0; j<tfl->width; j++)
                    tfl->blocks[k*tfl->width+j].b = tfl->blocks[(k-1)*tfl->width+j].b;
        }
}

// Удаляет заполненные строки и подсчитывает количество строк 
int eraseLinesTet(TetGame *tetg){
    TetField *tfl = tetg->field;
    int count = 0; // Переменная для количества удалённых строк 
    for(int i=tfl->height-1; i>=0; i--){ // Строки удаляю с последней, выполняю сдвиг строк вниз
        while(lineFilledTet(i, tfl)){ // Пока текущая строка заполнена,
            dropLineTet(i, tfl); // Удаляю её со сдвигом вниз
            count++; // Увеличивая количество УДАЛЁННЫХ строк на ОДИН (счётчиком)
        }
    }
    return count; // Возвращаю количество удалённых строк
}
// Создание и инициализация фигуры
TetFigure *createTetFigure(TetGame *tetg){
    TetFigure *t = (TetFigure*)malloc(sizeof(TetFigure)); // Выделяю память для структуры фигуры
    t->x = 0; // По оси X значение в 0
    t->y = 0; // По оси Y значение в 0
    t->size = tetg->figurest->size; // Размер области фигуры тот же что и у шаблона
    t->blocks = (TetBlock*)malloc(sizeof(TetBlock)*t->size*t->size); // Выделяю память для хранения блоков фигуры
    return t;
}
// Освобождение памяти
void freeTetFigure(TetFigure *tf){
    if(tf){ // Освобождаю в обратном порядке
        if(tf->blocks)
            free(tf->blocks); // Сначала освобождаю память для блоков
        free(tf); // Затем освобождаю память для структуры фигуры
    }
}

// Выброс новой фигуры на поле
void dropNewFigure(TetGame *tetg){
    TetFigure *t = createTetFigure(tetg); // Создаю новую фигуру в памяти
    t->x = tetg->field->width/2 - t->size/2; // Задаю координаты по оси X, что бы фигура была по середине
    t->y = 0; // По оси Y = 0, что бы фигура появилась вверху
    int fnum = rand() % tetg->figurest->count; // Переменная для выброса РАНДОМНОЙ фигуры (исходя из количества ШАБЛОНОВ фигур)
    for(int i=0; i<t->size; i++)
 // Скопируем блоки шаблона в область фигуры. Номер шаблона определяет смещение в массиве ... 
        for(int j=0; j<t->size; j++) { // ... блоков, соответствующее выбранному шаблону.
            t->blocks[i*t->size+j].b = tetg->figurest->blocks[fnum*t->size*t->size + i*t->size + j].b;
        }
    freeTetFigure(tetg->figure); // Удаление предыдущей фигуры
    tetg->figure = t; // Запись только что созданной
}
// Выполняет вращение фигуры
TetFigure *rotTetFigure(TetGame *tetg){
    TetFigure *t = createTetFigure(tetg);
    TetFigure *told = tetg->figure;
    t->x = told->x;
    t->y = told->y;
    for(int i=0; i<t->size; i++)
        for(int j=0; j<t->size; j++)
            t->blocks[i*t->size+j].b = told->blocks[j*t->size+t->size-1-i].b;
    return t;
}

// Прототип ПРОЦЕДУРЫ, рассчитывающий один игровой ТАКТ одного игрового ЦИКЛА (вызыввается примерно 30 РАЗ в секунду)
void calculateTet(TetGame *tetg) {
// Т.к. фигура падает медленнее, чем количество тактов в секунду, нужно ПРОПУСТИТЬ какое-то количество тактов    
    if(tetg->ticks_left <= 0){ // переменная для хранения количества тактов до перемещения фигуры вниз
        tetg->ticks_left = tetg->ticks; // обновляю количество тактов до перемещения фигуры вниз (хранится в переменной ticks)
// передвигаю фигуру на один блок вниз
        moveFigureDown(tetg);
// если произошло столкновение с другим объектом игрового поля
        if(collisionTet(tetg)){
            moveFigureUp(tetg); // Фигура передвигается в исходное положение (т.е. обратно вверх)
            plantFigure(tetg); // Её блоки переносятся на поле
// Прежде чем выбрасывать на поле новую фигуру - считаем количество строк, которые нужно удалить (т.к. они заполнены блоками фигуры)
// Добавляем эти строки к очкам и уменьшаем количество строк, которые нужно удалить
            tetg->score += eraseLinesTet(tetg);
            dropNewFigure(tetg); // Выбрасывается новая фигура
// Если после появления новой фигуры в игре происходит СТОЛКНОВЕНИК с другим объектом игрового поля
            if(collisionTet(tetg)){
// игра закончилась
                tetg->playing = TET_GAMEOVER; // playing - хранит состояние игры (определю позже с остальными константами)
                return;
            }
        }
    }
// ЭТО  МОЖНО НАЗВАТЬ КОНЕЧНЫМ АВТОМАТОМ?
// Обработка действий игрока
    switch(tetg->player->action){
        case TET_PLAYER_RIGHT: // при нажатии ВПРАВО
            moveFigureRight(tetg); // перемещение вправо
            if(collisionTet(tetg)) // если столкновение с другим объектом игрового поля
                moveFigureLeft(tetg); // перемещение влево (возвращаем обратно в предыдущее положение)
            break;
        case TET_PLAYER_LEFT: // при нажатии ВЛЕВО
            moveFigureLeft(tetg); // перемещение влево
            if(collisionTet(tetg)) // если столкновение с другим объектом игрового поля
                moveFigureRight(tetg); // перемещение вправо (возвращаем обратно в предыдущее положение)
            break;
        case TET_PLAYER_DOWN: // при нажатии ВНИЗ
            moveFigureDown(tetg); // перемещение вниз
            if(collisionTet(tetg)) // если столкновение с другим объектом игрового поля
                moveFigureUp(tetg); // перемещение вверх (возвращаем обратно в предыдущее положение)
            break;
        case TET_PLAYER_UP: {// при нажатии ВВЕРХ - фигура поворачивается
            TetFigure *t = rotTetFigure(tetg); // переменная для хранения фигуры после поворота
            TetFigure *told = tetg->figure; // переменная для хранения сторой версии фигуры
            tetg->figure = t; // перевёрнутая версия фигуры = ткущая версия фигуры
            if(collisionTet(tetg)){ // в случае столкновения
                tetg->figure = told; // возвращаем обратно в предыдущее положение блоков
                freeTetFigure(t); // удаление новой (очистка памяти?)
            }else{
                freeTetFigure(told); // ЛИБО удаление старой фигуры
            }
        }
        break;
    case TET_PLAYER_NOP: // бездействие или неизвестной значение ввода - не предпринимать никаких действий
    default: //
        break;
    }
    tetg->ticks_left--; // уменьшаем количество тактов до перемещения фигуры вниз
}

#endif // TETRIS_H