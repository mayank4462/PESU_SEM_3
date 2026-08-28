## 1. `main.c`

```c
#include <stdio.h>
#include "stack.h"

// check well formed brackets {[(
int main()
{
	char str[MAX + 1]; 
	char *s = str;
	int is_valid = 1;
	char top_ch;
	
	scanf("%s", str);
	printf("input expression : :%s:\n", s);
	
	struct stack mystack;
	init_stack(&mystack);
	
	char ch;
	while(is_valid && (ch = *s++) != '\0')
	{
		// 1. Push all opening brackets to the stack
		if(ch == '(' || ch == '[' || ch == '{')
		{
			push(&mystack, ch);
		}

		// 2. Process closing brackets
		else if(ch == ')' || ch == ']' || ch == '}')
		{
			// If stack is empty when a closer arrives, it's invalid
			if(is_empty(&mystack))
			{
				is_valid = 0;
			}
			else
			{
				top_ch = pop(&mystack); 
				
				// Validate if the popped bracket matches the closer
				if ((ch == ')' && top_ch != '(') ||
				    (ch == ']' && top_ch != '[') ||
				    (ch == '}' && top_ch != '{'))
				{
					is_valid = 0; 
				}
			}
		}
		
	}
	if(is_valid && is_empty(&mystack))
	{
		printf("Well formed expression\n");
	}
	else 
	{
		printf("not well formed expression\n");
	}
	
	deinit_stack(&mystack);
	return 0;
}
```

## 2. `stack.c`

```c
#include <stdio.h>
#include "stack.h"

void init_stack(struct stack *ptr_stack)
{
	ptr_stack->top = -1;
}

void deinit_stack(struct stack *ptr_stack)
{
	ptr_stack->top = -1;
}

void push(struct stack *ptr_stack, char ch)
{
	if(! is_full(ptr_stack))
	{
		ptr_stack->key[++ptr_stack->top] = ch;
	}
	else 
	{
		printf("stack full; cannot push\n");
	}
}
char pop(struct stack *ptr_stack)
{
	if(! is_empty(ptr_stack))
	{
		return ptr_stack->key[ptr_stack->top--];
	}
	else 
	{
		printf("stack empty; cannot pop\n");
		return '\0';
	}
}
int is_empty(struct stack *ptr_stack)
{
	return ptr_stack->top == -1;
}
int is_full(struct stack *ptr_stack)
{
	return ptr_stack->top == MAX - 1;
}
```

## 3. `stack.h`

```c
#ifndef STACK_H
#define STACK_H

#define MAX 100

struct stack
{
    char key[MAX];
    int top;
};

void init_stack(struct stack *ptr_stack);
void deinit_stack(struct stack *ptr_stack);
void push(struct stack *ptr_stack, char ch);
char pop(struct stack *ptr_stack);
int is_empty(struct stack *ptr_stack);
int is_full(struct stack *ptr_stack);

#endif
```

---

## Key Points

- **Array-based stack**: `struct stack` uses a fixed `char key[MAX]` array with a `top` index (initialized to `-1` for empty) instead of a linked structure — O(1) push/pop, but capacity capped at `MAX` (100).
- **`is_empty`/`is_full`** are the only two conditions guarding `push`/`pop` — `push` refuses silently (just prints) when `top == MAX - 1`, and `pop` returns `'\0'` and prints when `top == -1`, so callers must check before trusting the return value in general use (this program handles it via the `is_empty` check right before `pop`).
- **Core algorithm**: every opening bracket (`(`, `[`, `{`) is pushed; every closing bracket pops the top and checks it matches the corresponding opener — a mismatch or a pop-from-empty immediately sets `is_valid = 0`.
- **Final validity check has two parts**: `is_valid` must still be `1` **and** the stack must be empty at the end — this second check catches unclosed openers (e.g. `"(("` would keep `is_valid == 1` throughout since no closer ever triggers a mismatch, but leaves `top != -1`).
- **Short-circuit loop**: `while(is_valid && (ch = *s++) != '\0')` stops scanning as soon as invalidity is detected, avoiding wasted work on the rest of the string — but note the loop condition still evaluates `is_valid` _before_ checking `is_full`/`is_empty` state changes inside, so it exits on the _next_ iteration after the flag is set, not mid-body.
- **`init_stack`/`deinit_stack`** are both trivial here (`top = -1`) since the array is stack-allocated (not heap), so there's no `malloc`/`free` pairing needed — `deinit_stack` exists mainly for API symmetry/future-proofing.
- **Non-bracket characters** (letters, digits, etc.) are simply ignored by the `if`/`else if` chain — only bracket characters affect `is_valid` or the stack.
- **`MAX + 1`** in `char str[MAX + 1]` reserves room for the null terminator alongside up to `MAX` real characters read by `scanf("%s", ...)`.