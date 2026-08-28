## `stack.h`

```c
#ifndef STACK_H
#define STACK_H

#include <stdbool.h>

struct node
{
    int value;
    struct node *next;
};

struct stack
{
    struct node *top;
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
#include <stdlib.h>
#include "stack.h"

void push(struct stack *s, int value)
{
    if(!isFull(s))
    {
        struct node *newNode = malloc(sizeof(struct node));

        newNode->value = value;
        newNode->next = s->top;
        s->top = newNode;
    }
}

int pop(struct stack *s)
{
    if(!isEmpty(s))
    {
        struct node *temp = s->top;
        int value = temp->value;

        s->top = temp->next;
        free(temp);

        return value;
    }

    return -1;
}

void display(struct stack *s)
{
    struct node *curr = s->top;

    while(curr != NULL)
    {
        printf("%d -> ", curr->value);
        curr = curr->next;
    }
}

bool isFull(struct stack *s)
{
    struct node *temp = malloc(sizeof(struct node));

    if(temp == NULL)
        return true;

    free(temp);
    return false;
}

bool isEmpty(struct stack *s)
{
    return (s->top == NULL);
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
    s.top = NULL;

    printf("Enter number of elements: ");
    scanf("%d", &n);

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

## Key Points

- `top = NULL` → Stack is empty
- `push()` → Create node and insert it at `top`
- `pop()` → Remove and free the top node
- `isEmpty()` → `top == NULL`
- `isFull()` → `malloc()` failure
- No `MAX` limit is required
- `curr` is used in `display()` so that `top` is not modified