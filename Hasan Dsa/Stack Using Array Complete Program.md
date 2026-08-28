# Stack Using Array — Complete Program

## `stack.h`

```c
#ifndef STACK_H
#define STACK_H

#include <stdbool.h>

#define MAX 100

struct stack
{
    int arr[MAX];
    int top;
};

void push(struct stack *s, int value);
int pop(struct stack *s);
void display(struct stack *s);
bool isFull(struct stack *s);
bool isEmpty(struct stack *s);

#endif
```

## `stack.c`

```c
#include <stdio.h>
#include "stack.h"

void push(struct stack *s, int value)
{
    if(!isFull(s))
    {
        s->arr[++s->top] = value;
    }
}

int pop(struct stack *s)
{
    if(!isEmpty(s))
    {
        return s->arr[s->top--];
    }

    return -1;
}

void display(struct stack *s)
{
    for(int i = s->top; i >= 0; i--)
    {
        printf("%d ", s->arr[i]);
    }
}

bool isFull(struct stack *s)
{
    return (s->top == MAX - 1);
}

bool isEmpty(struct stack *s)
{
    return (s->top == -1);
}
```

## `client.c`

```c
#include <stdio.h>
#include "stack.h"

int main()
{
    int n = 0, value = 0;
    char choice;

    struct stack s;
    s.top = -1;

    printf("Enter number of elements: ");
    scanf("%d", &n);

    if(n > MAX)
    {
        printf("Stack Overflow\n");
        return 0;
    }

    for(int i = 0; i < n; i++)
    {
        printf("\nEnter value: ");
        scanf("%d", &value);
        push(&s, value);
    }

    printf("\nDo you wish to pop elements? (Y/N): ");
    scanf(" %c", &choice);

    if(choice == 'Y' || choice == 'y')
    {
        while(!isEmpty(&s))
        {
            int popped_element = pop(&s);

            printf("\nPopped element: %d", popped_element);

            if(isEmpty(&s))
                break;

            printf("\nDo you want to pop more? (y/n): ");
            scanf(" %c", &choice);

            if(choice == 'n' || choice == 'N')
                break;
        }
    }

    printf("\n\nDisplaying stack now:\n");
    display(&s);

    return 0;
}
```

### Key Points

- `top = -1` → Stack is empty
- `push()` → Increment `top`, then insert
- `pop()` → Return top element, then decrement `top`
- `isFull()` → `top == MAX - 1`
- `isEmpty()` → `top == -1`