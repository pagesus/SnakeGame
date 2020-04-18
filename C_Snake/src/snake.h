#ifndef SNAKE_H
#define SNAKE_H
#include "stack.h"

typedef enum
{
    Left=0,
    Up,
    Right,
    Down
} Direct;
typedef enum 
{
    DEAD=0,
    ALIVE
}LifeStatus;

typedef struct Snake_tag
{
    //member data
    Dual_queue *body;
    Direct direct;
    Element egg;
    LifeStatus status;
    int y_max;
    int x_max;
    int *record;
    int record_size;
    int score;

    //member function
    void (*draw)(struct Snake_tag *snake);    
    void (*crawl)(struct Snake_tag *snake);
    void (*set_direct)(struct Snake_tag *snake, Direct new_direct);    
    Direct (*get_direct)(struct Snake_tag *snake);
    LifeStatus (*get_lifestatus)(struct Snake_tag *snake);    
}Snake;

//member function implementation
void draw_func(Snake *snake);
void crawl_func(Snake *snake);
void set_direct_func(Snake *snake, Direct new_direct);
Direct get_direct_func(Snake *snake);
LifeStatus get_lifestatus_func(Snake *snake);

//Related function
Snake* init_snake(void);
void delete_snake(Snake *snake);
void print_score(Snake* snake);
void start_scene(Snake* snake);
void end_scene(Snake* snake);

#endif