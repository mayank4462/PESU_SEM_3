---
title: 05 — Initialise and Deinitialise Array BST
tags: [unit-3, c, code]
---

# 05 — Initialise and Deinitialise Array BST

**Source file:** `Original Source Files/tree_array.c`  
**Purpose:** Initialises and resets the array-based tree.

## Explanation

The array storage is embedded in the struct, so it does not require per-node `malloc()` or `free()`.

## Code

```c
void init(struct tree *ptr_tree)
{
    ptr_tree->root = -1;
    ptr_tree->count = 0;
}

void deinit(struct tree *ptr_tree)
{
    ptr_tree->root = -1;
    ptr_tree->count = 0;
}
```

## Related files

- [[02 - Array Representation of BST/00 - Complete Walkthrough]]
- [[02 - Array Representation of BST/Original Source Files/README - Source Files]]
