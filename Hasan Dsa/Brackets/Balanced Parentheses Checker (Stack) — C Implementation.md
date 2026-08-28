## 1. `main.c`

```c
#include <stdio.h>
#include "stack.h"

// check well formed simple parantheses ((()))
int main()
{
	char str[MAXSIZE + 1]; char *s = str;
	int is_valid = 1;
	scanf("%s", str);
	printf("input expression : :%s:\n", s);
	struct stack mystack;
	init_stack(&mystack);

	char ch,ch1;
	while( is_valid && (ch = *s++) != '\0')
	{
		if(ch == '(')
		{
			push(&mystack, '(');
		}
		else if(ch == ')')
		{
			if(! is_empty(&mystack))
			{	
			ch1 = pop(&mystack);
				if(ch1 == '(')
				{
					is_valid=1;
				}
			}
			else 
				is_valid = 0;
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
	
}
```

## 2. `stack.c`

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
int is_empty(struct stack *ptr_stack)
{
	return ptr_stack->top_ == -1;
}
int is_full(struct stack *ptr_stack)
{
	return ptr_stack->top_ + 1 == MAXSIZE;
}
```

## 3. `stack.h`

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
void deinit_stack(struct stack *ptr_stack);
void push(struct stack *ptr_stack, char ch);
char pop(struct stack *ptr_stack);
int is_empty(struct stack *ptr_stack);
int is_full(struct stack *ptr_stack);

#endif
```

---

## Key Points

- **Single-bracket-type simplification**: since only `(` is ever pushed, `pop()` returning anything other than `'('` from a non-empty stack is impossible — so the `if(ch1 == '(')` check inside the `)` branch is always true in practice, and there's no corresponding `else` to catch a hypothetical mismatch (unlike the multi-bracket version, this program can't actually detect a "wrong type" closer since there's only one type).
- **Redundant `is_valid = 1`**: setting `is_valid = 1` when a match is found is a no-op — it was already `1` (the `while` loop wouldn't have entered otherwise), so this line does nothing functionally.
- **Same final validity rule as the bracket version**: `is_valid` staying `1` only guarantees no premature `)` without a matching `(`; the stack must _also_ be empty at the end to catch unclosed `(` (e.g. `"((("` never sets `is_valid = 0` but leaves the stack non-empty).
- **`is_full`** uses `top_ + 1 == MAXSIZE` here instead of `top_ == MAX - 1` (as in the bracket-matching version) — algebraically identical, just written differently.
- **Trailing underscore naming (`key_`, `top_`)**: purely a style choice (common in some C++ conventions to mark struct/class members) — functionally identical to the non-underscore version from before.
- **Missing `return` in `main`**: the function is declared `int main()` but has no explicit `return` statement — this compiles (falls off the end, implicitly returning an unspecified value in C89 or `0` under C99+ rules) but is worth adding `return 0;` for clarity/portability.
- **Array-based stack**, fixed capacity `MAXSIZE` (100), same O(1) push/pop and same silent-failure behavior (`push`/`pop` just print and return `'\0'`/do nothing on full/empty) as the earlier stack implementation.
- **Non-`(`/`)` characters** are ignored entirely by the `if`/`else if` chain, same as the bracket version.