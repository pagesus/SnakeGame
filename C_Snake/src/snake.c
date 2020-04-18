#include "snake.h"
#include <stdlib.h>
#include <curses.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

//Debug Marco
#ifdef DEBUG
#define debug_info(format, args...)\
    do{\
        fprintf(stderr, "%s:%d - ", __FILE__, __LINE__);\
        fprintf(stderr, format, ##args);\
    }while(0)
#else
    #define debug_info(format, args...) 
#endif

static 
LifeStatus is_bite_self(Element val, const Snake *snake)
{
    int len   = snake->body->len;
    Node *tmp = snake->body->head;
    while(len--)
    {
        if(val.y == tmp->pos.y && val.x == tmp->pos.x)
        {
            return DEAD;
        }
        tmp = tmp->next;
    }    
    return  ALIVE;
}

Snake* init_snake(void)
{
    Snake* snake = (Snake*)malloc(sizeof(Snake));
    int i, j;
    if(snake)
    {   
        init_dq();
        if(NULL == (snake->body=init_dq()))
        {
            fprintf(stderr, "Can't init the snake\n");
            exit(EXIT_FAILURE);
        }

        snake->body->push_head(snake->body, 4, 3);
        snake->body->push_head(snake->body, 5, 3);
        snake->direct = Down;
        snake->egg.x  = 8;
        snake->egg.y  = 3;
        snake->status = ALIVE;
        snake->draw   = draw_func;
        snake->crawl  = crawl_func;        
        snake->set_direct = set_direct_func;
        snake->get_direct = get_direct_func;
        snake->get_lifestatus = get_lifestatus_func;
        snake->score = 0;
        getmaxyx(stdscr, snake->y_max, snake->x_max);
        if((snake->record=(int*)malloc(sizeof(int) * (snake->x_max * snake->y_max + 1))))
        {
            snake->record_size = 0;
            for(i = 0; i < snake->y_max; ++i)
            {
                for(j = 0; j < snake->x_max; ++j)
                {
                    if( i == 0 || i == snake->y_max-1 ||
                        j == 0 || j == snake->x_max-1 ||
                        (i == 3 && j == 7) || 
                        (i == 3 && j == 2) || 
                        (i == 4 && j == 2))
                    {
                        continue;
                    }
                    snake->record[snake->record_size] = i * snake->x_max + j;
                    ++snake->record_size;
                }
            }
        }
        else
        {
            fprintf(stderr, "malloc memory error!\n");
            exit(EXIT_FAILURE);
        }
        srandom(time(NULL));
    }
    return snake;
}

void delete_snake(Snake *snake)
{    
    destroy_dq(snake->body);
    free(snake->record);
    free(snake);
}

void draw_func(Snake *snake)
{
    int x, y;
    Node *tmp = snake->body->head;

    attron(COLOR_PAIR(1));
    mvaddch(snake->egg.y, snake->egg.x, '*');
    attroff(COLOR_PAIR(1));
    int len = snake->body->len;
    while(len--)
    {
        x = tmp->pos.x;
        y = tmp->pos.y;
        mvaddch(y, x, '*');
        tmp = tmp->next;
    }
}

void crawl_func(Snake *snake)
{
    Element val;
    snake->body->get_head(snake->body, &val);
    switch(snake->direct)
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
    if( DEAD == is_bite_self(val, snake) ||
        val.x < 1 || val.x > snake->x_max-2 ||
        val.y < 1 || val.y > snake->y_max-2)
    {
        snake->status = DEAD;
        return;
    }

    snake->body->push_head(snake->body, val.x, val.y);
    snake->record[val.y * snake->y_max + val.x] = snake->record[snake->record_size-1];    
    --snake->record_size;
    if(val.x == snake->egg.x && val.y == snake->egg.y)
    {
        int tmp = snake->record[random() % snake->record_size];
        snake->egg.y = tmp / snake->x_max;
        snake->egg.x = tmp % snake->x_max;
        ++snake->score;
        return;
    }
    snake->body->pop_tail(snake->body,&val);
    snake->record[snake->record_size-1] = val.y * snake->y_max + val.x;    
    ++snake->record_size;
    mvaddch(val.y, val.x, ' ');
}

void set_direct_func(Snake *snake, Direct new_direct)
{
    snake->direct = new_direct;
}

Direct get_direct_func(Snake *snake)
{
    return snake->direct;
}

LifeStatus get_lifestatus_func(Snake *snake)
{
    return snake->status;
}

void print_score(Snake* snake)
{
    char buff[50];
    sprintf(buff, "Game Over, Your Score is %d, press Enter key to continue.", snake->score);
    mvprintw(snake->y_max/2 ,(snake->x_max-strlen(buff))/2, "%s", buff);
    nocbreak();
    cbreak();

    int key = 0;
    while((key=getch()) && key != ERR && key != '\n');
}

void start_scene(Snake* snake)
{
    int i = 5;  //5 seconds left
    char buff[50];
    box(stdscr, 0, 0);
    while(i > 0)
    {
        sprintf(buff, "%d seconds left to start snake game", i--);
        mvprintw(snake->y_max/2 ,(snake->x_max-strlen(buff))/2, "%s", buff);
        refresh();
        sleep(1);
    }   
}

void end_scene(Snake* snake)
{
    clear();
    refresh();
    int i, j;
    for(i = 0; i < snake->y_max; ++i)
    {
        for(j = 0; j < snake->x_max; ++j)
        {
            mvaddch(i, j, '*');
        }   
    }
    refresh();
    usleep(500*1000);
}