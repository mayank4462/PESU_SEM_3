This program converts an infix expression to postfix using the **operator precedence parsing** technique, with two precedence functions:

- **`F(symbol)`** — precedence of the operator _when it's already on the stack_
- **`G(symbol)`** — precedence of the _incoming_ symbol from the input string

The core rule: while the stack-top operator's `F` value is greater than the incoming symbol's `G` value, pop and output. Operator Precedence Parsing — Infix toOtherwise, push the incoming symbol (or, in the case of matching parentheses, discard both).

Project layout:

```c
.
├── main.c      # driver + precedence tables + conversion logic
├── stack.c     # stack implementation
└── stack.h     # stack struct + function declarations
```

---

## `stack.h`

Defines the stack structure (a fixed-size char array with a `top_` index) and declares every stack operation used elsewhere.

```c
#ifndef STACK_H
#define STACK_H 

#define MAXSIZE 100
struct stack 
{
	char key_[MAXSIZE];
	int top_;
};

void init_stack(struct stack *ptr_stack);
void deinit_stack();
void push(struct stack *ptr_stack, char ch);
char pop(struct stack *ptr_stack);
char peek(struct stack *ptr_stack);
int is_empty(struct stack *ptr_stack);
int is_full(struct stack *ptr_stack);


#endif
```

---

## `stack.c`

Standard array-based stack: `init_stack`/`deinit_stack` reset `top_` to `-1`, `push`/`pop` check bounds before touching `key_`, and `is_empty`/`is_full` are simple index checks.

```c
#include <stdio.h>
#include "stack.h"

void init_stack(struct stack *ptr_stack)
{
	ptr_stack->top_ = -1;
}

void deinit_stack(struct stack *ptr_stack)
{
	ptr_stack->top_ = -1;
}

void push(struct stack *ptr_stack, char ch)
{
	if(! is_full(ptr_stack))
	{
		ptr_stack->key_[++ptr_stack->top_] = ch;
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
		return ptr_stack->key_[ptr_stack->top_--];
	}
	else 
	{
		printf("stack empty; cannot pop\n");
		return '\0';
	}
}
char peek(struct stack *ptr_stack)
{
	if(! is_empty(ptr_stack))
	{
		return ptr_stack->key_[ptr_stack->top_];
	}
	else 
	{
		printf("stack empty; cannot peek\n");
		return '\0';
	}	
}

int is_empty(struct stack *ptr_stack)
{
	return ptr_stack->top_ == -1;
}
int is_full(struct stack *ptr_stack)
{
	return ptr_stack->top_ + 1 == MAXSIZE;
}
```

---

## `main.c`

Reads an infix expression, then scans it left to right. `G()` gives the incoming symbol's precedence, `F()` gives the stack-top's precedence:

- While `F(top) > G(symbol)` → pop into the postfix output.
- If `F(top) != G(symbol)` → push the incoming symbol.
- Otherwise (`F(top) == G(symbol)`, e.g. `(` on stack meets `)` in input) → discard both by decrementing `top_` (no output written).

Any leftover stack contents are popped out at the end.

```c
#include <stdio.h>
#include<string.h>
#include "stack.h"

int G(char symbol) //Input string precedence Function G
{
	int prec;
	switch(symbol)
	{	
		case '+' :
		case '-' : prec = 1; break;
		case '*' :
		case '/' : prec = 3; break;
		case '$' :
		case '^' : prec = 6; break;
		case '(' : prec = 9; break;
		case ')' : prec = 0; break;
		default : prec =7; 
		
	}
	return prec;
}
int F(char symbol) //Stack precedence Function F
{
	int prec;
	switch(symbol)
	{	
		case '+' :
		case '-' : prec = 2; break;
		case '*' :
		case '/' : prec = 4; break;
		case '$' :
		case '^' : prec = 5; break;
		case '(' : prec = 0; break;
		case '#' : prec = -1; break;
		default : prec =8; 
		
	}
	return prec;
}
int main()
{
	char infix[MAXSIZE + 1]; char *s = infix;
	char postfix[MAXSIZE + 1]; char *p = postfix; 
	
	printf("enter the valid infix expression\n");
	scanf("%s",infix);
	printf("input expression : :%s:\n", s);

	struct stack mystack;
	init_stack(&mystack);
	int i;
	char symbol;
	//char ch1;
	for (i=0;i<strlen(infix); i++)
	{
		symbol=infix[i];
		while(!is_empty(&mystack) && F(peek(&mystack)) > G(symbol))
				{
					//*p++ = ' ';
					*p++ = pop(&mystack);
					
				}
		if(F(peek(&mystack))!=G(symbol))
				push(&mystack, symbol);
		else //if(ch == ')')
				mystack.top_--;
				
	}
	
	while(! is_empty(&mystack))
	{
		
		*p++ = pop(&mystack);
	}
	*p = '\0';
	
	printf("infix : %s\n", infix);
	printf("postfix : %s\n", postfix);
	deinit_stack(&mystack);
	
}
```

---

## Compiling

```bash
gcc main.c stack.c -o infix2postfix
./infix2postfix
```

## Notes

- `peek()` is called unconditionally on an empty stack right after the `while` loop (`if(F(peek(&mystack))!=G(symbol))`). This works in practice because `peek()` returns `'\0'` on an empty stack and `F('\0')` falls into the `default` case (`8`), which never equals any `G()` value — so the symbol still gets pushed correctly — but it does print a `"stack empty; cannot peek"` message each time. Harmless, just noisy.
- Only single-character operands are supported (multi-digit numbers or multi-letter identifiers will be split into separate symbols).
- No validation of malformed input (e.g. mismatched parentheses).