---
title: 03 — Deallocate Threaded BST
tags: [unit-3, c, code]
---

# 03 — Deallocate Threaded BST

**Source file:** `Original Source Files/tree.c`  
**Purpose:** Frees nodes by following inorder successor navigation.

## Explanation

The next node is saved before the current node is freed.

## Code

```c
void deinit(struct tree *ptr_tree)
{
    struct node *temp = leftmost(ptr_tree->root);
    struct node *curr;

    while(temp != NULL)
    {
        curr = temp;

        if(temp->rthread == 1)
            temp = temp->right;
        else
            temp = leftmost(temp->right);

        free(curr);
    }

    ptr_tree->root = NULL;
}
```

## Related files

- [[06 - Threaded BST/00 - Complete Walkthrough]]
- [[06 - Threaded BST/Original Source Files/README - Source Files]]
