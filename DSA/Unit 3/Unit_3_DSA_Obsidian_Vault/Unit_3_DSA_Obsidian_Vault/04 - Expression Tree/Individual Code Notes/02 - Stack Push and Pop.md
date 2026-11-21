---
title: 02 — Stack Push and Pop
tags: [unit-3, c, code]
---

# 02 — Stack Push and Pop

**Source file:** `Original Source Files/stack.c`  
**Purpose:** Implements a stack of expression-tree node pointers.

## Explanation

Operands are pushed as leaf nodes. For an operator, the right subtree is popped first, then the left subtree.

## Code

```c
void init_stack(stack_t *ptr_stack)
{
	ptr_stack->top_ = -1;
}

void deinit_stack(stack_t *ptr_stack)
{
	ptr_stack->top_ = -1;
}

void push_operand(stack_t *ptr_stack, char ch)
{
	if(! is_full(ptr_stack))
	{
		node_t* temp = create_node(ch);
		ptr_stack->key_[++ptr_stack->top_] = temp;
	}
	else 
	{
		printf("stack full; cannot push\n");
	}
}

void push_operator(stack_t *ptr_stack, char ch)
{
	if(! is_full(ptr_stack))
	{
		node_t* temp = create_node(ch);
		temp->right_ = pop(ptr_stack);
		temp->left_ = pop(ptr_stack);
		ptr_stack->key_[++ptr_stack->top_] = temp;
	}
}

node_t* pop(stack_t *ptr_stack)
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

int is_empty(stack_t *ptr_stack)
{
	return ptr_stack->top_ == -1;
}

int is_full(stack_t *ptr_stack)
{
	return ptr_stack->top_ == MAXSIZE - 1;
}
```

## Related files

- [[04 - Expression Tree/00 - Complete Walkthrough]]
- [[04 - Expression Tree/Original Source Files/README - Source Files]]
