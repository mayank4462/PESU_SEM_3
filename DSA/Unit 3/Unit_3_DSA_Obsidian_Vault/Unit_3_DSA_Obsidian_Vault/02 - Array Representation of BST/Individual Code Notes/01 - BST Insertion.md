---
title: 01 — BST Insertion
tags: [unit-3, c, code]
---

# 01 — BST Insertion

**Source file:** `Original Source Files/tree_array.c`  
**Purpose:** Inserts a key into the array-based BST using array indices as child links.

## Explanation

Index `-1` represents a missing child. Duplicate keys follow the right branch in this implementation.

## Code

```c
static int insert_recursive(struct node elem[], int root, int temp)
{
    if(root == -1)
    {
        root = temp;
    }
    else if(elem[temp].key < elem[root].key)
    {
        elem[root].left = insert_recursive(elem, elem[root].left, temp);
    }
    else
    {
        elem[root].right = insert_recursive(elem, elem[root].right, temp);
    }
    return root;
}

void insert(struct tree *ptr_tree, int key)
{
    if(ptr_tree->count == MAXSIZE)
    {
        printf("tree full\n");
        exit(1);
    }

    // count tells us the next available index in the array, temp tells us the index of the new node to be inserted

    int temp = ptr_tree->count++; 
    ptr_tree->elem[temp].key = key;
    ptr_tree->elem[temp].left = ptr_tree->elem[temp].right = -1;

    ptr_tree->root = insert_recursive(ptr_tree->elem, ptr_tree->root, temp);
}
```

## Related files

- [[02 - Array Representation of BST/00 - Complete Walkthrough]]
- [[02 - Array Representation of BST/Original Source Files/README - Source Files]]
