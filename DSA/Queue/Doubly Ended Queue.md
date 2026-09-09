```c
#include <stdio.h>

#define MAXSIZE 5

struct deque
{
    int data[MAXSIZE];
    int front;
    int rear;
};

typedef struct deque deque_t;


// Initialize
void init_deque(deque_t *dq)
{
    dq->front = -1;
    dq->rear = -1;
}


// Check Empty
int is_empty(deque_t *dq)
{
    return dq->front == -1;
}


// Check Full
int is_full(deque_t *dq)
{
    return (dq->front == 0 && dq->rear == MAXSIZE - 1)
           || (dq->front == dq->rear + 1);
}


// Insert at Rear
void insert_rear(deque_t *dq, int data)
{
    if (is_full(dq))
    {
        printf("Deque is full\n");
        return;
    }

    if (is_empty(dq))
    {
        dq->front = dq->rear = 0;
    }
    else if (dq->rear == MAXSIZE - 1)
    {
        dq->rear = 0;
    }
    else
    {
        dq->rear++;
    }

    dq->data[dq->rear] = data;
}


// Insert at Front
void insert_front(deque_t *dq, int data)
{
    if (is_full(dq))
    {
        printf("Deque is full\n");
        return;
    }

    if (is_empty(dq))
    {
        dq->front = dq->rear = 0;
    }
    else if (dq->front == 0)
    {
        dq->front = MAXSIZE - 1;
    }
    else
    {
        dq->front--;
    }

    dq->data[dq->front] = data;
}


// Delete from Front
int delete_front(deque_t *dq)
{
    if (is_empty(dq))
    {
        printf("Deque is empty\n");
        return -1;
    }

    int data = dq->data[dq->front];

    if (dq->front == dq->rear)
    {
        dq->front = dq->rear = -1;
    }
    else if (dq->front == MAXSIZE - 1)
    {
        dq->front = 0;
    }
    else
    {
        dq->front++;
    }

    return data;
}


// Delete from Rear
int delete_rear(deque_t *dq)
{
    if (is_empty(dq))
    {
        printf("Deque is empty\n");
        return -1;
    }

    int data = dq->data[dq->rear];

    if (dq->front == dq->rear)
    {
        dq->front = dq->rear = -1;
    }
    else if (dq->rear == 0)
    {
        dq->rear = MAXSIZE - 1;
    }
    else
    {
        dq->rear--;
    }

    return data;
}


// Display
void display(deque_t *dq)
{
    if (is_empty(dq))
    {
        printf("Deque is empty\n");
        return;
    }

    int i = dq->front;

    printf("Deque: ");

    while (1)
    {
        printf("%d ", dq->data[i]);

        if (i == dq->rear)
        {
            break;
        }

        i = (i + 1) % MAXSIZE;
    }

    printf("\n");
}


// Main
int main()
{
    deque_t dq;

    init_deque(&dq);

    insert_rear(&dq, 10);
    insert_rear(&dq, 20);
    insert_rear(&dq, 30);

    insert_front(&dq, 5);

    display(&dq);

    printf("Deleted from front: %d\n", delete_front(&dq));

    printf("Deleted from rear: %d\n", delete_rear(&dq));

    display(&dq);

    return 0;
}
```