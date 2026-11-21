---
title: 02 — Dynamic BST Traversals
tags: [unit-3, c, code]
---

# 02 — Dynamic BST Traversals

**Source file:** `Original Source Files/tree.c`  
**Purpose:** Implements inorder, preorder and postorder traversal using recursive pointer navigation.

## Explanation

Inorder traversal of a BST displays values in ascending order.

## Code

```c
static void disp_recursive(struct node *temp)
{
    if(temp)
    {
        disp_recursive(temp->left);
        printf("%d ", temp->key);
        disp_recursive(temp->right);
    }
}

static void disp_recursive_preorder(struct node *temp)
{
    if(temp)
    {
        printf("%d ", temp->key);
        disp_recursive_preorder(temp->left);
        disp_recursive_preorder(temp->right);
    }
}

static void disp_recursive_postorder(struct node *temp)
{
    if(temp)
    {
        disp_recursive_postorder(temp->left);
        disp_recursive_postorder(temp->right);
        printf("%d ", temp->key);
    }
}

void disp(struct tree *ptr_tree)
{
    printf("Tree : ");
    disp_recursive(ptr_tree->root);
    printf("\n");
}

void disp_preorder(struct tree *ptr_tree)
{
    printf("Tree preorder: ");
    disp_recursive_preorder(ptr_tree->root);
    printf("\n");
}

void disp_postorder(struct tree *ptr_tree)
{
    printf("Tree postorder: ");
    disp_recursive_postorder(ptr_tree->root);
    printf("\n");
}

void disp_inorder(struct tree *ptr_tree)
{
    printf("Tree inorder: ");
    disp_recursive(ptr_tree->root);
    printf("\n");
}
```

## Related files

- [[03 - Dynamic BST/00 - Complete Walkthrough]]
- [[03 - Dynamic BST/Original Source Files/README - Source Files]]
