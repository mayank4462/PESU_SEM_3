---
title: 05 — Copy and Compare Dynamic BST
tags: [unit-3, c, code]
---

# 05 — Copy and Compare Dynamic BST

**Source file:** `Original Source Files/tree.c`  
**Purpose:** Creates a deep copy and checks whether two trees have identical structure and values.

## Explanation

A deep copy allocates separate nodes; it does not merely copy pointer addresses.

## Code

```c
struct node *copy_recursive(struct node *ptr_src)
{
    struct node *temp;
    if(ptr_src != NULL)
    {
        temp = (struct node*)malloc(sizeof(struct node));
        temp->key = ptr_src->key;
        temp->left = copy_recursive(ptr_src->left);
        temp->right = copy_recursive(ptr_src->right);
        return temp;
    }
    else
    {
        return NULL;
    }
}

void copy(struct tree *ptr_src, struct tree *ptr_dst)
{
    ptr_dst->root = copy_recursive(ptr_src->root);
}

int are_same_recursive(struct node *root1, struct node *root2)
{
    if(root1 == NULL && root2 == NULL)
        return 1;
    else if(root1 == NULL || root2 == NULL)
        return 0;
    else
        return root1->key == root2->key &&
               are_same_recursive(root1->left, root2->left) &&
               are_same_recursive(root1->right, root2->right);
}

int are_same(struct tree *ptr_tree1, struct tree *ptr_tree2)
{
    return are_same_recursive(ptr_tree1->root, ptr_tree2->root);
}
```

## Related files

- [[03 - Dynamic BST/00 - Complete Walkthrough]]
- [[03 - Dynamic BST/Original Source Files/README - Source Files]]
