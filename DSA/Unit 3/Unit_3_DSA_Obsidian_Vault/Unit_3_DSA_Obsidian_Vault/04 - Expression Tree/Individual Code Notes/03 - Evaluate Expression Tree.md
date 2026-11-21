---
title: 03 — Evaluate Expression Tree
tags: [unit-3, c, code]
---

# 03 — Evaluate Expression Tree

**Source file:** `Original Source Files/stack.c`  
**Purpose:** Recursively evaluates an expression tree created from a postfix expression.

## Explanation

A leaf is an operand. An internal node applies its operator to the recursively evaluated left and right subtrees.

## Code

```c
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

## Related files

- [[04 - Expression Tree/00 - Complete Walkthrough]]
- [[04 - Expression Tree/Original Source Files/README - Source Files]]
