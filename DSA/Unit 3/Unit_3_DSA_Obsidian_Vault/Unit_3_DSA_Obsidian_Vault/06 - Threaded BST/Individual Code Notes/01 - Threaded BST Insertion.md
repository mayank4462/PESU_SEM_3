---
title: 01 — Threaded BST Insertion
tags: [unit-3, c, code]
---

# 01 — Threaded BST Insertion

**Source file:** `Original Source Files/tree.c`  
**Purpose:** Inserts nodes while maintaining predecessor and successor threads.

## Explanation

`lthread` or `rthread` equal to 1 means the corresponding pointer is a thread; 0 means it is a child link.

## Code

```c
static struct node *create_node(int key)
{
    struct node *temp = (struct node*)malloc(sizeof(struct node));

    if(temp == NULL)
    {
        printf("no more memory\n");
        exit(1);
    }

    temp->key = key;
    temp->left = temp->right = NULL;
    temp->lthread = temp->rthread = 1;

    return temp;
}

static struct node *insert_key(struct node *root, int key)
{
    struct node *temp = create_node(key);

    if(root == NULL)
    {
        return temp;
    }

    struct node *prev = NULL;
    struct node *curr = root;

    while(curr != NULL)
    {
        prev = curr;

        if(key < curr->key)
        {
            if(curr->lthread == 0)
                curr = curr->left;
            else
                break;
        }
        else
        {
            if(curr->rthread == 0)
                curr = curr->right;
            else
                break;
        }
    }

    // Insert as left child
    if(key < prev->key)
    {
        temp->left = prev->left;
        temp->right = prev;

        prev->left = temp;
        prev->lthread = 0;
    }

    // Insert as right child
    else
    {
        temp->right = prev->right;
        temp->left = prev;

        prev->right = temp;
        prev->rthread = 0;
    }

    return root;
}

void insert(struct tree *ptr_tree, int key)
{
    ptr_tree->root = insert_key(ptr_tree->root, key);
}
```

## Related files

- [[06 - Threaded BST/00 - Complete Walkthrough]]
- [[06 - Threaded BST/Original Source Files/README - Source Files]]
