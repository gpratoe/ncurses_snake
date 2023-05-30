#ifndef _SNAKE_H
#define _SNAKE_H
#include <stdbool.h>

typedef struct _snake_s *snake;

typedef struct point *fruit;

typedef enum direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT
}direction;

// Snake

snake snake_create(int x, int y);

void snake_eat(snake s);

void snake_update(snake s);

// void snake_setx(snake s, int x);

// void snake_sety(snake s, int y);

int snake_getx(snake s);

int snake_gety(snake s);

void snake_set_dir(snake s, direction d);

direction snake_get_dir(snake s);

bool snake_is_suicidal(snake s);

snake snake_destroy(snake s);

// Fruit

fruit fruit_create(int x, int y);

void fruit_update(fruit f);

int fruit_getx(fruit f);

int fruit_gety(fruit f);

void fruit_setx(fruit f, int x);

void fruit_sety(fruit f, int y);

fruit fruit_destroy(fruit f);

#endif