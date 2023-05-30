#include <stdlib.h>
#include <ncurses.h>
#include <stdbool.h>
#include <time.h>

#include "include/snake.h"

snake snak;
fruit frut;
bool running;
unsigned int score;
int maxx, maxy;
int border_width;

//-----------Bounded random number function prototipe----------------//
int rand_from_to(int from, int to);
//------------------------------------------------------------------ //

void setup_game()
{
    initscr();
    noecho(); // dont print what i type
    cbreak(); // process input as it happens
    nodelay(stdscr, TRUE); // dont wait on getch()
    curs_set(0); // dont show cursor
    keypad(stdscr, TRUE); // allow arrow keys, Fkeys, etc.
    getmaxyx(stdscr,maxy,maxx); // get terminal dimentions

    box(stdscr,0,0);
    border_width = 1;
    score = 0;
    mvprintw(0,maxx/2 - 4,"SCORE:%u",score);
    
    snak = snake_create(maxx/2, maxy/2);
    frut = fruit_create(rand_from_to(border_width, maxx-border_width), rand_from_to(border_width,maxy-border_width));
    fruit_update(frut);

    running = true;
}

void pause(int x, int y)
{
    bool ispaused = true;
    int width = 20;
    int height = 5;
    int px = x-(width/2);
    int py = y-(height/2);
    int pause_maxx,pause_maxy;
    WINDOW *pause = newwin(height, width, py, px);
    box(pause,0,0);
    getmaxyx(pause,pause_maxy,pause_maxx);
    mvwaddstr(pause,pause_maxy/2 , pause_maxx/2 - 3, "PAUSED");
    wrefresh(pause);

    while (ispaused)
    {
        switch (wgetch(pause))
        {
        case 'p':
        case 'P':
            werase(pause);
            wrefresh(pause);
            delwin(pause);
            ispaused = false;
            break;

        case 'q':
        case 'Q':
            running = false;
            ispaused = false;
            break;
        }
    }   
}

void game_over(int x, int y)
{
    bool idle = true;
    int width = 20;
    int height= 11;
    int px = x - (width/2);
    int py = y - (height/2);
    WINDOW *go = newwin(height,width,py,px);
    int xmax = getmaxx(go);
    int ymax = getmaxy(go);
    mvwaddstr(go,ymax/2 -1,xmax/2 - 5, "YOU SUCK!");
    mvwaddstr(go,ymax/2 +1,xmax/2 - 7, "New Game? (Y/N)");
    wborder(go,'#','#','#','#','#','#','#','#');
    wrefresh(go);
    
    while (idle)
    {
    
        switch (wgetch(go))
        {
        case 'n':
        case 'N':
            running = false;
            idle = false;
            break;
        case 'y':
        case 'Y':
            snak = snake_destroy(snak);
            frut = fruit_destroy(frut);
            delwin(go);
            clear();
            refresh();
            endwin();
            setup_game();
            idle = false;
            break;
        }
    }
    
} 

void move_snake(snake s, int key)
{
    direction snake_current_dir = snake_get_dir(s);
    switch (key)
    {
    case KEY_UP:
        if(snake_current_dir != DOWN)
            snake_set_dir(s, UP);
        break;
    case KEY_DOWN:
        if(snake_current_dir != UP)
            snake_set_dir(s, DOWN);
        break;
    case KEY_LEFT:
        if(snake_current_dir != RIGHT)
            snake_set_dir(s, LEFT);
        break;
    case KEY_RIGHT:
        if(snake_current_dir != LEFT)
            snake_set_dir(s, RIGHT);
        break;
    default:
        break;
    }
}

bool is_coliding(snake s)
{
    int sx,sy;
    sx = snake_getx(s);
    sy = snake_gety(s);

    if(snake_is_suicidal(s) || 
       sx <= getbegx(stdscr) || 
       sy <= getbegy(stdscr) || 
       sx >= maxx-border_width || 
       sy >= maxy-border_width)
    {
        return true;
    }
    
    return false;
}

void check_input(int ch)
{
    switch (ch)
    {
    case 'q':
    case 'Q':
        running = false;
        break;
    case 'p':
    case 'P':
        pause(maxx/2, maxy/2);
    default:
        break;
    }
}

bool snake_is_eating(snake snak, fruit frut)
{
    return (fruit_getx(frut) == snake_getx(snak)) && (fruit_gety(frut) == snake_gety(snak));
}

int rand_from_to(int from, int to)
{
    return (rand() % (to - from)) + from;
}

void run_game()
{
    srand(time(NULL));
    setup_game();

    int ch;
    while (running)
    {
        ch = getch();
        check_input(ch);

        move_snake(snak,ch);
        if(is_coliding(snak))
        {
            game_over(maxx/2,maxy/2);
        }        
        if(snake_is_eating(snak,frut))
        {
            snake_eat(snak);
            fruit_setx(frut, rand_from_to(border_width,maxx-border_width));
            fruit_sety(frut, rand_from_to(border_width,maxy-border_width));
            mvprintw(0,getmaxx(stdscr)/2 +2,"%u",++score);
        }
        snake_update(snak);
        fruit_update(frut);
        refresh();
    
        delay_output(70);
    }
    
    snak = snake_destroy(snak);
    frut = fruit_destroy(frut);
    endwin();
}