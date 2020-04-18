#include "stack.h"
#include <malloc.h>
#include <stdlib.h>

//Debug Marco definition
#ifdef DEBUG
#define debug_info(format, args...)\
    do{\
        fprintf(stderr, "%s:%d - ", __FILE__, __LINE__);\
        fprintf(stderr, format, ##args);\
    }while(0)
#else
    #define debug_info(format, args...) 
#endif

Dual_queue* init_dq(void)
{
    Dual_queue* queue;
    if(NULL == (queue=(Dual_queue*)malloc(sizeof(Dual_queue))))
    {
        debug_info("Failed to malloc memory for queue!\n");
        exit(EXIT_FAILURE);
    }
    queue->head = NULL;
    queue->tail = NULL;
    queue->push_head = push_head_func;
    queue->is_empty  = is_empty_func;
    queue->pop_tail  = pop_tail_func;
    queue->get_head  = get_head_func;
    queue->len  = 0;    
    return queue;
}

void destroy_dq(Dual_queue* queue)
{
    while(queue->head)
    {   
        Node* ptr = queue->head;
        queue->head = queue->head->next;
        free(ptr);
    }
    free(queue);
}

Status push_head_func(Dual_queue *queue, int x, int y)
{
    Node *tmp = (Node*) malloc(sizeof(Node));
    if(NULL == tmp)
    {
        fprintf(stderr, "Can't malloc memory for new node!\n");
        exit(EXIT_FAILURE);
    }
    tmp->pos.x = x;
    tmp->pos.y = y;
    if(0 == queue->len)
    {
        tmp->prev = tmp->next = NULL;
        queue->head = queue->tail = tmp;
    }
    else
    {
        tmp->prev = NULL;
        tmp->next = queue->head;
        queue->head->prev = tmp;
        queue->head = tmp;
    }
    queue->len += 1;
    return Success;
}

Bool is_empty_func(const Dual_queue *queue)
{
    return queue->len == 0 ? True : False;
}

Status pop_tail_func(Dual_queue *queue, Element *val)
{
    if(True == queue->is_empty(queue))
    {
        return Fail;
    }
    else
    {
        val->x = queue->tail->pos.x;
        val->y = queue->tail->pos.y;
        queue->len -= 1;
        Node* tmp = queue->tail;
        if(0 == queue->len)
        {
            queue->head = queue->tail = NULL;
        }
        else
        {
            queue->tail = queue->tail->prev;
            queue->tail->next = NULL;
        }
        free(tmp);
        return Success;
    }
}

Status get_head_func(Dual_queue *queue, Element *val)
{
    if(True == queue->is_empty(queue))
    {
        return Fail;
    }
    else
    {
        val->x = queue->head->pos.x;
        val->y = queue->head->pos.y;       
        return Success;
    }
}