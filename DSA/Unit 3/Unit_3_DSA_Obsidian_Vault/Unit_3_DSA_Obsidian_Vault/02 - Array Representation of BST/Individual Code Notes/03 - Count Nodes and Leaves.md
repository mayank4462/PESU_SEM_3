---
title: 03 — Count Nodes and Leaves
tags: [unit-3, c, code]
---

# 03 — Count Nodes and Leaves

**Source file:** `Original Source Files/tree_array.c`  
**Purpose:** Counts all nodes and leaf nodes recursively.

## Explanation

A leaf has both child indices equal to `-1`.

## Code

```c
static int number_of_nodes_recursive(struct node elem[], int temp)
{
    if(temp == -1)
    {
        return 0;
    }
    else
    {
        return 1 + number_of_nodes_recursive(elem, elem[temp].left)
                 + number_of_nodes_recursive(elem, elem[temp].right);
    }
}

int number_of_nodes(struct tree *ptr_tree)
{
    return number_of_nodes_recursive(ptr_tree->elem, ptr_tree->root);
}

static int number_of_leaves_recursive(struct node elem[], int temp)
{
    if(temp == -1)
    {
        return 0;
    }
    else if(elem[temp].left == -1 && elem[temp].right == -1)
    {
        return 1;
    }
    else
    {
        return number_of_leaves_recursive(elem, elem[temp].left)
             + number_of_leaves_recursive(elem, elem[temp].right);
    }
}

int number_of_leaves(struct tree *ptr_tree)
{
    return number_of_leaves_recursive(ptr_tree->elem, ptr_tree->root);
}
```

## Related files

- [[02 - Array Representation of BST/00 - Complete Walkthrough]]
- [[02 - Array Representation of BST/Original Source Files/README - Source Files]]
