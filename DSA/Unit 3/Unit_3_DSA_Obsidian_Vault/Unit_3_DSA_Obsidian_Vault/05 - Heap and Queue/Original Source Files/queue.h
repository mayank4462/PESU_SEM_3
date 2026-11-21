#ifndef QUEUE_H
#define QUEUE_H

#include "heap.h"

struct queue
{
    struct heap q;
};

void init_queue(struct queue *ptr_queue);
void deinit_queue(struct queue *ptr_queue);

void enqueue(struct queue *ptr_queue, int key);
int dequeue(struct queue *ptr_queue);

int is_empty(struct queue *ptr_queue);
int is_full(struct queue *ptr_queue);

void disp_queue(struct queue *ptr_queue);

#endif