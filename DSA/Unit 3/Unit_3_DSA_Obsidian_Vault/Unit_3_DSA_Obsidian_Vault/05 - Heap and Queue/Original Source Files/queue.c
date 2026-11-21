#include <stdio.h>
#include "queue.h"

void init_queue(struct queue *ptr_queue)
{
    init(&ptr_queue->q);
}

void deinit_queue(struct queue *ptr_queue)
{
    deinit(&ptr_queue->q);
}

void enqueue(struct queue *ptr_queue, int key)
{
    insert(&ptr_queue->q, key);
}

int dequeue(struct queue *ptr_queue)
{
    return delete_max(&ptr_queue->q);
}

int is_empty(struct queue *ptr_queue)
{
    return ptr_queue->q.n == 0;
}

int is_full(struct queue *ptr_queue)
{
    return ptr_queue->q.n == MAXSIZE;
}

void disp_queue(struct queue *ptr_queue)
{
    disp(&ptr_queue->q);
}