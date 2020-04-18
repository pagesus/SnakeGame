#ifndef SNAKE_H
#define SNAKE_H
#include <deque>
#include <map>
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

struct Element
{
public:
    Element()=default;
    Element(int a, int b): x(a), y(b){}
    int x;
    int y;
};

class Snake
{
public:
    Snake();
    void draw();
    void crawl();
    void set_direct(Direct new_direct) {direct = new_direct;}
    Direct get_direct() {return direct;}
    LifeStatus get_lifestatus() {return status;}
    void print_score();
    void start_scene();
    void end_scene();
    int get_score(){return score;}
private:
    std::deque<Element> body;
    Direct direct;
    Element egg;
    LifeStatus status;
    int y_max;
    int x_max;
    std::map<int, int> record;
    int record_size;
    int score;

    LifeStatus is_bite_self(Element val);
};

#endif