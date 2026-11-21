---
title: 03 — Count Nodes and Leaves
tags: [unit-3, c, code]
---

# 03 — Count Nodes and Leaves

**Source file:** `Original Source Files/tree.c`  
**Purpose:** Counts total nodes and leaves in a dynamically allocated BST.

## Explanation

Base cases return zero for a null pointer; recursive calls process both subtrees.

## Code

```c
int number_of_nodes_recursive(struct node *temp)
{
    if(temp == NULL)
    {
        return 0;
    }
    else
    {
        return 1 + number_of_nodes_recursive(temp->left)
                 + number_of_nodes_recursive(temp->right);
    }
}

int number_of_leaves_recursive(struct node *temp)
{
    if(temp == NULL)
    {
        return 0;
    }
    else if(temp->left == NULL && temp->right == NULL)
    {
        return 1;
    }
    else
    {
        return number_of_leaves_recursive(temp->left)
             + number_of_leaves_recursive(temp->right);
    }
}

int number_of_nodes(struct tree *ptr_tree)
{
    return number_of_nodes_recursive(ptr_tree->root);
}

int number_of_leaves(struct tree *ptr_tree)
{
    return number_of_leaves_recursive(ptr_tree->root);
}
```

## Related files

- [[03 - Dynamic BST/00 - Complete Walkthrough]]
- [[03 - Dynamic BST/Original Source Files/README - Source Files]]
