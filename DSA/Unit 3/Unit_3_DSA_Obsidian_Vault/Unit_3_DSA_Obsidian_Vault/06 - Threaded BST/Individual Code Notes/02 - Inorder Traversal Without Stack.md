---
title: 02 — Inorder Traversal Without Stack
tags: [unit-3, c, code]
---

# 02 — Inorder Traversal Without Stack

**Source file:** `Original Source Files/tree.c`  
**Purpose:** Traverses a threaded BST in inorder without recursion or an auxiliary stack.

## Explanation

When a right link is a thread, it points directly to the inorder successor. Otherwise, move to the leftmost node of the right subtree.

## Code

```c
static struct node *leftmost(struct node *temp)
{
    if(temp == NULL)
        return NULL;

    while(temp->lthread == 0)
        temp = temp->left;

    return temp;
}

static void inorder(struct node *root)
{
    struct node *temp = leftmost(root);

    while(temp != NULL)
    {
        printf("%d ", temp->key);

        if(temp->rthread == 1)
            temp = temp->right;
        else
            temp = leftmost(temp->right);
    }
}

void disp(struct tree *ptr_tree)
{
    printf("Tree : ");
    inorder(ptr_tree->root);
    printf("\n");
}
```

## Related files

- [[06 - Threaded BST/00 - Complete Walkthrough]]
- [[06 - Threaded BST/Original Source Files/README - Source Files]]
