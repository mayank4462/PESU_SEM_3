## 1. `main.c`

```c
#include <stdio.h>
#include "stack.h"

// evaluate expression in postfix
int main()
{
	char str[MAXSIZE + 1]; char *s = str;
	gets(str);
	printf("input expression : :%s:\n", s);
	struct stack mystack;
	init_stack(&mystack);
	
	char ch;
	int val1; int val2;
	int val = 0;
	int is_num = 0;
	while( (ch = *s++) != '\0')
	{
		if(ch == '+' || ch == '-' || ch == '*' || ch == '/')
		{
			val2 = pop(&mystack);
			val1 = pop(&mystack);
			switch(ch)
			{
				case '+' : push(&mystack,val1 + val2);
					   break;
				case '-' : push(&mystack,val1 - val2);
					   break;
				case '*' : push(&mystack,val1 * val2);
					   break;
				case '/' : push(&mystack,val1 / val2);
					   break;
			}
		}
		else if(ch >= '0' && ch <= '9')
		{
			is_num = 1;
			val = val * 10 + ch - '0';//converts the character stream into a numerical integer value
		}
		else if (ch == ' ' && is_num)
		{
			printf("pushing %d\n", val);
			push(&mystack, val);
			val = 0;
			is_num = 0;
		}
	}
	printf("result : %d\n", pop(&mystack));
	deinit_stack(&mystack);
	
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

void push(struct stack *ptr_stack, int key)
{
	if(! is_full(ptr_stack))
	{
		ptr_stack->key[++ptr_stack->top] = key;
	}
	else 
	{
		printf("stack full; cannot push\n");
	}
}
int pop(struct stack *ptr_stack)
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
	return ptr_stack->top + 1 == MAXSIZE;
}
```

## 3. `stack.h`

```c
#ifndef STACK_H
#define STACK_H 

#define MAXSIZE 100
struct stack 
{
	int key[MAXSIZE];
	int top;
};

void init_stack(struct stack *ptr_stack);
void deinit_stack(struct stack *ptr_stack);
void push(struct stack *ptr_stack, int key);
int pop(struct stack *ptr_stack);
int is_empty(struct stack *ptr_stack);
int is_full(struct stack *ptr_stack);


#endif
```

---

## Key Points

- **`int` stack, not `char`**: unlike the two bracket-checking programs, this stack stores `int key[MAXSIZE]` — it holds actual numeric operands, not bracket characters, since postfix evaluation needs arithmetic on real values.
- **Digit accumulation**: `val = val * 10 + ch - '0'` builds a multi-digit number one character at a time as digits stream in (e.g. `'2'` then `'3'` builds `23`), using the classic "shift left, add new digit" pattern.
- **Space-delimited tokens**: a number is only pushed once a trailing space is hit (`ch == ' ' && is_num`) — so the input format requires operands separated by spaces (e.g. `"2 3 +"`), and there's **no handling for a number at the very end of the string with no trailing space** — if the input doesn't end in a space, the last operand is silently dropped (`is_num` stays `1` but the loop exits on `'\0'` without ever pushing it).
- **Evaluation order matters**: `val2 = pop()` then `val1 = pop()` — the second-popped value is `val1` (the left operand), preserving correct order for non-commutative ops like `-` and `/` (top of stack = most recently pushed = right operand).
- **`gets(str)` is unsafe and removed from the C standard (C11)**: it has no bounds checking and can overflow `str[MAXSIZE + 1]` on long input — this is a real bug worth fixing (e.g. `fgets(str, sizeof(str), stdin)`), not just a style choice.
- **No division-by-zero or stack-underflow guard**: `val1 / val2` will crash/UB if `val2` is `0`, and `pop()` on an empty stack (malformed expression) prints a warning but still returns `'\0'` (0), silently continuing with a wrong result rather than aborting.
- **`pop`'s error return doubles as valid data**: `pop` returns `'\0'` (i.e. `0`) both as its "stack empty" error signal and as a value indistinguishable from a legitimately pushed `0` — there's no way for the caller to tell an error apart from a real zero operand.
- **Same array-based stack mechanics** (`is_empty`, `is_full`, `top` index starting at `-1`) as the previous stack programs, just retyped from `char` to `int`.