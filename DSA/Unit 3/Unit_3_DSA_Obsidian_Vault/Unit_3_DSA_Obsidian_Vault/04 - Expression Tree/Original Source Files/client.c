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