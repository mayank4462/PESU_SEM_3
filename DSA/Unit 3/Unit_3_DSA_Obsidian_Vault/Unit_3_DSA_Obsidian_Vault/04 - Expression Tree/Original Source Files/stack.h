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