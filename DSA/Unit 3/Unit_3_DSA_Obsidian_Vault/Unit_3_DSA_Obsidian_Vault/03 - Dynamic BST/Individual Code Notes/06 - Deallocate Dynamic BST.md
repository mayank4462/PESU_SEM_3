---
title: 06 — Deallocate Dynamic BST
tags: [unit-3, c, code]
---

# 06 — Deallocate Dynamic BST

**Source file:** `Original Source Files/tree.c`  
**Purpose:** Frees all dynamically allocated nodes and resets the root pointer.

## Explanation

Postorder freeing ensures child nodes are released before their parent.

## Code

```c
static void deinit_recursive(struct node *root)
{
    if(root)
    {
        deinit_recursive(root->left);
        deinit_recursive(root->right);
        free(root);
    }
}

void deinit(struct tree *ptr_tree)
{
    deinit_recursive(ptr_tree->root);
    ptr_tree->root = NULL;
}
```

## Related files

- [[03 - Dynamic BST/00 - Complete Walkthrough]]
- [[03 - Dynamic BST/Original Source Files/README - Source Files]]
