---
title: 01 — Create Expression Tree Nodes
tags: [unit-3, c, code]
---

# 01 — Create Expression Tree Nodes

**Source file:** `Original Source Files/stack.c`  
**Purpose:** Allocates and initialises an expression-tree node.

## Explanation

Each node stores a character and pointers to its left and right expression subtrees.

## Code

```c
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
```

## Related files

- [[04 - Expression Tree/00 - Complete Walkthrough]]
- [[04 - Expression Tree/Original Source Files/README - Source Files]]
