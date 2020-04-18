#include "snake.h"
#include <ncurses.h>
#include <cstdio>
#include <unistd.h>
#include <cstring>

Snake::Snake()
{
    direct = Down;
    body.push_front(Element(4,3));
    body.push_front(Element(5,3));
    egg.x = 8;
    egg.y = 3;
    
    status = ALIVE;
    score = 0;
    getmaxyx(stdscr, y_max, x_max);

    record_size = 0;
    for(int i = 0; i < y_max; ++i)
    {
        for(int j = 0; j < x_max; ++j)
        {
            if( i == 0 || i == y_max-1 ||
                j == 0 || j == x_max-1 ||
                (i == 3 && j == 7) || 
                (i == 3 && j == 2) || 
                (i == 4 && j == 2))
            {
                continue;
            }
            record[record_size] = i *  x_max + j;
            ++record_size;
        }
    }
    srandom(time(NULL));
}

void Snake::print_score()
{
    char buff[50];
    sprintf(buff, "Game Over, Your Score is %d, press Enter key to continue.", score);
    mvprintw(y_max/2 ,(x_max-strlen(buff))/2, "%s", buff);
    nocbreak();
    cbreak();
	
	int key = 0;
    while((key=getch()) && key != ERR && key != '\n');
}

void Snake::start_scene()
{
    int i = 5;  //5 seconds left
    char buff[50];
    box(stdscr, 0, 0);
    while(i > 0)
    {
        sprintf(buff, "%d seconds left to start snake game", i--);
        mvprintw(y_max/2 ,(x_max-strlen(buff))/2, "%s", buff);
        refresh();
        sleep(1);
    }   
}

void Snake::end_scene()
{
    clear();
    refresh();
    int i, j;
    for(i = 0; i < y_max; ++i)
    {
        for(j = 0; j < x_max; ++j)
        {
            mvaddch(i, j, '*');
        }   
    }
    refresh();
    usleep(500000);
}

void Snake::draw()
{
    int x, y;

    attron(COLOR_PAIR(1));
    mvaddch(egg.y, egg.x, '*');
    attroff(COLOR_PAIR(1));

    for(const auto &elem : body)
    {
        x = elem.x;
        y = elem.y;
        mvaddch(y, x, '*');
    }    
}

void Snake::crawl()
{
    Element val = body.front();
    switch(direct)
    {
        case Left:
        {
            --val.x;
            break;
        }
        case Up:
        {
            --val.y;
            break;
        }
        case Right:
        {
            ++val.x;
            break;
        }
        case Down:
        {
            ++val.y;
            break;
        }
    }
    if( DEAD == is_bite_self(val) ||
        val.x < 1 || val.x > x_max-2 ||
        val.y < 1 || val.y > y_max-2)
    {
        status = DEAD;
        return;
    }

    body.push_front(val);
    record[val.y * y_max + val.x] = record[record_size-1];
    --record_size;

    if(val.x == egg.x && val.y == egg.y)
    {
        int tmp = record[random() % record_size];
        egg.y = tmp / x_max;
        egg.x = tmp % x_max;
        ++score;
        return;
    }

    val = body.back();
    body.pop_back();
    record[record_size-1] = val.y * y_max + val.x;
    ++record_size;
    mvaddch(val.y, val.x, ' ');
}

LifeStatus Snake::is_bite_self(Element val)
{    
    for(auto elem : body)
    {
        if(val.x == elem.x && val.y == elem.y)
        {
            return DEAD;
        }
    }
    return  ALIVE;
}