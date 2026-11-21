---
title: Expression Tree from Postfix Expression
tags: [unit-3, c, expression-tree, postfix, stack, recursion]
---

# Constructing and Evaluating an Expression Tree from Postfix

## 1. Main idea

The program reads a postfix expression and constructs an expression tree using a stack of node pointers. Each tree node contains:

- `ch_`: one operand digit or one operator (`+`, `-`, `*`, `/`).
- `left_`: left child.
- `right_`: right child.

Operands become leaf nodes. Operators become internal nodes.

## 2. Postfix construction algorithm

Scan the expression from left to right:

1. If the current character is a digit, create a node and push its pointer onto the stack.
2. If the current character is an operator:
   - create an operator node;
   - pop the **right** subtree first;
   - pop the **left** subtree second;
   - attach them to the operator node;
   - push the operator node back.
3. After a valid postfix expression has been processed, the stack should contain exactly one pointer: the root of the expression tree.

Why right first? A postfix expression places the left operand before the right operand. Because a stack is LIFO, the right operand is on top and must be popped first.

### Example: `23+5*`

Construction:

- Push leaf `2`.
- Push leaf `3`.
- Read `+`: pop `3` as right child and `2` as left child; push the `+` node.
- Push leaf `5`.
- Read `*`: pop `5` as right child and the `+` subtree as left child.

Resulting tree:

```text
        *
       / \
      +   5
     / \
    2   3
```

The expression is `(2 + 3) * 5`, so the evaluated result is `25`.

## 3. Stack implementation

The stack array stores `node_t *` values, not integers or characters. `top_ = -1` means empty. Push increments `top_` and stores the pointer. Pop returns the pointer at the current top and then decrements `top_`.

- `push_operand()` creates and pushes a leaf node.
- `push_operator()` creates an operator node, pops two subtrees and pushes the new combined tree.
- `is_empty()` checks `top_ == -1`.
- `is_full()` checks `top_ == MAXSIZE - 1`.

## 4. Evaluation

`eval()` pops the root and passes it to `eval_recursive()`.

`eval_recursive()`:

1. If both children are `NULL`, the node is an operand digit. Convert its character to an integer using `root->ch_ - '0'`.
2. Otherwise recursively evaluate the left and right subtrees.
3. Apply the operator stored at the current node and return the result.

The tree is evaluated bottom-up: operands are evaluated before the operator that combines them.

**Current implementation limit:** operands are single decimal digits. A sequence such as `12+` is treated as two separate operands `1` and `2`, not as the number twelve. Division uses integer division because the return type is `int`.

## 5. Important problems in the supplied source

These are source-level issues, not changes to the algorithm:

1. **Prototype clarity:** `stack.h` declares `void deinit_stack();` with no parameter information, while `stack.c` defines it with a `stack_t *` parameter. Update the header to `void deinit_stack(stack_t *ptr_stack);` so callers and the compiler see the intended function signature.
2. **Unsafe input:** `client.c` calls `gets(str)`. `gets()` cannot limit input length and is removed from modern C standards. Use `fgets(str, sizeof str, stdin)` in a corrected implementation and remove the trailing newline if needed.
3. **Memory leak:** `deinit_stack()` only resets `top_`; it does not free the expression tree nodes. Also, `eval()` pops the root, so resetting the stack cannot release the tree. A complete cleanup should recursively free the final expression tree after evaluation.
4. **Malformed postfix input:** `push_operator()` assumes that two operands/subtrees are available. An invalid expression can cause `NULL` children and later a dereference during evaluation. Validate stack size before popping.
5. **Single final tree assumption:** a valid postfix expression should leave exactly one tree pointer on the stack. The supplied client does not verify this.

## 6. Source code

### `stack.h`

```c
#ifndef STACK_H
#define STACK_H 

#define MAXSIZE 100
struct node
{
	char ch_;
	struct node *left_;
	struct node *right_;
};
typedef struct node node_t;

struct stack 
{
	node_t* key_[MAXSIZE];
	int top_;
};
typedef struct stack stack_t;

void init_stack(stack_t *ptr_stack);
void deinit_stack();
void push_operand(stack_t *ptr_stack, char ch);
void push_operator(stack_t *ptr_stack, char ch);
node_t* pop(stack_t *ptr_stack);
int is_empty(stack_t *ptr_stack);
int is_full(stack_t *ptr_stack);

int eval(stack_t *ptr_stack);
#endif
```
### `stack.c`

```c
#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

void init_stack(stack_t *ptr_stack)
{
	ptr_stack->top_ = -1;
}

void deinit_stack(stack_t *ptr_stack)
{
	ptr_stack->top_ = -1;
}

static node_t* create_node(char ch)
{
	node_t *temp = (node_t*)malloc(sizeof(node_t));
	if(temp == NULL)
	{
		printf("memory full\n"); exit(1);
	}
	temp->ch_ = ch;
	temp->left_ = temp->right_ = NULL;
	return temp;
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
// Postfix order is left right operator, so pop right first and then left.
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
// Evaluate an expression tree bottom-up: evaluate children before applying root operator.
int eval_recursive(node_t* root)
{
	
    int left, right;

    if (root->left_ == NULL && root->right_ == NULL)
        return root->ch_ - '0';

    left = eval_recursive(root->left_);
    right = eval_recursive(root->right_);

    switch (root->ch_)
    {
        case '+': return left + right;
        case '-': return left - right;
        case '*': return left * right;
        case '/': return left / right;
    }

    return 0;
}

int eval(stack_t *ptr_stack)
{
	return eval_recursive(pop(ptr_stack));
}
```
### `client.c`

```c
#include <stdio.h>
#include "stack.h"

// make an expression tree given postfix expression 
// evaluate it
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
### `makefile`

```makefile
client.exe : client.o stack.o
	gcc client.o stack.o -o client.exe
stack.o : stack.c stack.h 
	gcc -Wall -c stack.c 
client.o : client.c stack.h
	gcc -Wall -c client.c
```
## 7. Minimal compile correction

At minimum, make the prototype in `stack.h` match the implementation:

```c
void deinit_stack(stack_t *ptr_stack);
```

For safe input in `client.c`, replace the `gets(str);` line with:

```c
if (fgets(str, sizeof str, stdin) == NULL)
    return 1;
str[strcspn(str, "\n")] = '\0';
```

This replacement also requires `#include <string.h>`. The memory-management issue still requires a recursive tree-freeing function; the prototype/input changes alone do not solve the leak.
