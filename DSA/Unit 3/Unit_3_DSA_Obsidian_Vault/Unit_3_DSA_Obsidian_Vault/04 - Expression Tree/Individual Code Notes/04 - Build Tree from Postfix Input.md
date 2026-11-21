---
title: 04 — Build Tree from Postfix Input
tags: [unit-3, c, code]
---

# 04 — Build Tree from Postfix Input

**Source file:** `Original Source Files/client.c`  
**Purpose:** Scans the postfix expression and builds/evaluates its expression tree using the stack API.

## Explanation

The supplied client uses `gets()`, which is unsafe and removed from modern C. The original is preserved; use `fgets()` in new code.

## Code

```c
int main()
{
	char str[MAXSIZE + 1]; char *s = str;
	gets(str);
	printf("input expression : :%s:\n", s);
	stack_t mystack;
	init_stack(&mystack);
	
	char ch;
	while( (ch = *s++) != '\0')
	{
		if(ch == '+' || ch == '-' || ch == '*' || ch == '/')
		{
			push_operator(&mystack, ch);
		}
		else if(ch >= '0' && ch <= '9')
		{
			push_operand(&mystack, ch);
		}
	}
	printf("result : %d\n", eval(&mystack));
	deinit_stack(&mystack);
	
}
```

## Related files

- [[04 - Expression Tree/00 - Complete Walkthrough]]
- [[04 - Expression Tree/Original Source Files/README - Source Files]]
