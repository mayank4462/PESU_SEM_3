```c
#include <stdio.h>

#define MAXSIZE 5

struct queue
{
    int data[MAXSIZE];
    int front;
    int rear;
};

typedef struct queue queue_t;

void init_queue(queue_t *q)
{
    q->front = 0;
    q->rear = 0;
}

int is_empty(queue_t *q)
{
    return q->front == q->rear;
}

int is_full(queue_t *q)
{
    return q->front == q->rear;
}

void enqueue(queue_t *q, int data)
{
    int temp;

    temp = ++q->rear % MAXSIZE;
    q->rear = temp;

    if (!is_full(q))
    {
        q->data[temp] = data;
    }
    else
    {
        q->rear = (q->rear - 1 + MAXSIZE) % MAXSIZE;
        printf("Queue full; cannot enqueue\n");
    }
}

int dequeue(queue_t *q)
{
    if (!is_empty(q))
    {
        int data;
        int temp = ++q->front % MAXSIZE;

        data = q->data[temp];
        q->front = temp;

        return data;
    }
    else
    {
        printf("Queue empty; cannot dequeue\n");
        return -1;
    }
}

void display(queue_t *q)
{
    if (is_empty(q))
    {
        printf("Queue is empty\n");
        return;
    }

    int i = q->front;

    printf("Queue: ");

    while (i != q->rear)
    {
        i = (i + 1) % MAXSIZE;
        printf("%d ", q->data[i]);
    }

    printf("\n");
}

int main()
{
    queue_t q;

    init_queue(&q);

    enqueue(&q, 10);
    enqueue(&q, 20);
    enqueue(&q, 30);
    enqueue(&q, 40);

    display(&q);

    printf("Deleted: %d\n", dequeue(&q));
    printf("Deleted: %d\n", dequeue(&q));

    display(&q);

    enqueue(&q, 50);
    enqueue(&q, 60);

    display(&q);

    return 0;
}
```