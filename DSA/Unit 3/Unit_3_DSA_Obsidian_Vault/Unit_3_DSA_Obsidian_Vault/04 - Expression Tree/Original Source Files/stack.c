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