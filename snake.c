#include <stdlib.h>
#include <ncurses.h>

#include "include/snake.h"

struct point
{
    int x;
    int y;
};

struct _snake_s
{
    struct point point;
    direction dir;
    struct _snake_s *next;
};

snake snake_create(int x, int y)
{
    snake s = malloc(sizeof(struct _snake_s));
    s->point.x = x;
    s->point.y = y;
    s->dir = LEFT;
    s->next = NULL;
    snake_eat(s);
    snake_eat(s);

    return s;
}

void snake_eat(snake s)
{
    snake sp;
    snake ns = malloc(sizeof(struct _snake_s));

    ns->next = NULL;
    sp = s;
    
    while (sp->next != NULL)
    {
        sp = sp->next;
    }

    switch (sp->dir)
    {
    case LEFT:
        ns->dir = LEFT;
        ns->point.x = sp->point.x + 1;
        ns->point.y = sp->point.y;
        break;
    
    case RIGHT:
        ns->dir = RIGHT;
        ns->point.x = sp->point.x - 1;
        ns->point.y = sp->point.y;
        break;
    
    case UP:
        ns->dir = UP;
        ns->point.x = sp->point.x;
        ns->point.y = sp->point.y +1;
        break;
    
    case DOWN:
        ns->dir = DOWN;
        ns->point.x = sp->point.x;
        ns->point.y = sp->point.y - 1;
        break;
    }
    sp->next = ns;        
}

void act_on_direction(snake s)
{
    switch (s->dir)
    {
    case LEFT:
        s->point.x--;
        break;
    
    case RIGHT:
        s->point.x++;
        break;
    
    case UP:
        s->point.y--;
        break;
    
    case DOWN:
        s->point.y++;
        break;
    }
}

void clear_last_section(snake s)
{
    switch (s->dir)
    {
    case LEFT:
        mvaddch(s->point.y, s->point.x+1, ' ');
        break;
    
    case RIGHT:
        mvaddch(s->point.y, s->point.x-1, ' ');        
        break;
    
    case UP:
        mvaddch(s->point.y+1, s->point.x, ' ');
        break;
    
    case DOWN:
        mvaddch(s->point.y-1, s->point.x, ' ');
        break;
    }
}

void snake_update(snake s)
{
    snake sp;
    direction prevdir, curdir;

    sp = s;
    prevdir = s->dir;
    while (sp != NULL) 
    {
        act_on_direction(sp);                             //moves and prints snake
                                                          //  
        mvaddch(sp->point.y, sp->point.x, ACS_CKBOARD);   //     
        if(sp->next == NULL)                              //
            clear_last_section(sp);

        curdir = sp->dir;       //updates directions
        if (sp->dir != prevdir)
        {
            sp->dir = prevdir;
            prevdir = curdir;
        }
        
        sp = sp->next;
    }
}

bool snake_is_suicidal(snake s)
{
    snake sp = s->next;
    while (sp != NULL)
    {
        if((s->point.x == sp->point.x) && (s->point.y == sp->point.y)) return true;

        sp = sp->next;
    }

    return false;
}

void snake_set_dir(snake s, direction dir)
{
    s->dir = dir;
}

direction snake_get_dir(snake s)
{
    return s->dir;
}

int snake_getx(snake s)
{
    return s->point.x;
}

int snake_gety(snake s)
{
    return s->point.y;
}

snake snake_destroy(snake s)
{
    snake sp;
    while (s != NULL)
    {
        sp = s;
        s = s->next;
        free(sp);
    }

    return s;
}

//Fruit
fruit fruit_create(int x, int y)
{
    fruit f = malloc(sizeof(struct point));
    f->x = x;
    f->y = y;

    return f;
}

void fruit_update(fruit f)
{
    mvaddch(f->y,f->x, ACS_DIAMOND);
}

int fruit_getx(fruit f)
{
    return f->x;
}
int fruit_gety(fruit f)
{
    return f->y;
}

void fruit_setx(fruit f, int x)
{
    f->x = x;
}
void fruit_sety(fruit f, int y)
{
    f->y = y;
}

fruit fruit_destroy(fruit f)
{
    free(f);
    f = NULL;
    return f;
}