#ifndef SNAKE_STACK_H
#define SNAKE_STACK_H
typedef enum 
{
    Fail=0, 
    Success=1
}Status;

typedef enum 
{
    False=0, 
    True=1
}Bool;

typedef struct 
{
    int x;
    int y;
}Element;

typedef struct Node_tag
{
    Element pos;
    struct Node_tag *prev;
    struct Node_tag *next;
}Node;

typedef struct Dual_queue_tag
{
    //member data
    int len;
    Node* head;
    Node* tail;

    //member function declaration
    Status (*push_head)(struct Dual_queue_tag *queue, int x, int y);
    Status (*pop_tail)(struct Dual_queue_tag *queue, Element *val);
    Status (*get_head)(struct Dual_queue_tag *queue, Element *val);    
    Bool (*is_empty)(const struct Dual_queue_tag *queue);
    
}Dual_queue;


Dual_queue* init_dq(void);
void destroy_dq(Dual_queue *queue);

//function which implements the corresponding member function
Status push_head_func(Dual_queue *queue, int x, int y);
Bool is_empty_func(const Dual_queue *queue);
Status pop_tail_func(Dual_queue *queue, Element *val);
Status get_head_func(Dual_queue *queue, Element *val);

#endif