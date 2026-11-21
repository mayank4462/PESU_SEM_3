---
title: 01 — Dynamic BST Insertion
tags: [unit-3, c, code]
---

# 01 — Dynamic BST Insertion

**Source file:** `Original Source Files/tree.c`  
**Purpose:** Allocates a node dynamically and inserts it into the pointer-based BST.

## Explanation

Left is chosen for a smaller key; the source routes duplicates to the right.

## Code

```c
static struct node *insert_recursive(struct node *root, struct node *temp)
{
    if(root == NULL)
    {
        root = temp;
    }
    else if(temp->key < root->key)
    {
        root->left = insert_recursive(root->left, temp);
    }
    else
    {
        root->right = insert_recursive(root->right, temp);
    }
    return root;
}

void insert(struct tree *ptr_tree, int key)
{
    struct node *temp = (struct node*)malloc(sizeof(struct node ));
    temp->key = key;
    temp->left = temp->right = NULL;
    ptr_tree->root = insert_recursive(ptr_tree->root, temp);
}
```

## Related files

- [[03 - Dynamic BST/00 - Complete Walkthrough]]
- [[03 - Dynamic BST/Original Source Files/README - Source Files]]
