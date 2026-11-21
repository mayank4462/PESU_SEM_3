---
title: 02 — BST Traversals
tags: [unit-3, c, code]
---

# 02 — BST Traversals

**Source file:** `Original Source Files/tree_array.c`  
**Purpose:** Implements inorder, preorder and postorder traversal for an array-based BST.

## Explanation

The traversal order changes only in the position of the root visit: LNR, NLR or LRN.

## Code

```c
static void disp_inorder_recursive(struct node elem[], int temp)
{
    if(temp != -1)
    {
        disp_inorder_recursive(elem, elem[temp].left);
        printf("%d ", elem[temp].key);
        disp_inorder_recursive(elem, elem[temp].right);
    }
}

static void disp_preorder_recursive(struct node elem[], int temp)
{
    if(temp != -1)
    {
        printf("%d ", elem[temp].key);
        disp_preorder_recursive(elem, elem[temp].left);
        disp_preorder_recursive(elem, elem[temp].right);
    }
}

static void disp_postorder_recursive(struct node elem[], int temp)
{
    if(temp != -1)
    {
        disp_postorder_recursive(elem, elem[temp].left);
        disp_postorder_recursive(elem, elem[temp].right);
        printf("%d ", elem[temp].key);
    }
}

void disp_inorder(struct tree *ptr_tree)
{
    printf("Tree inorder: ");
    disp_inorder_recursive(ptr_tree->elem, ptr_tree->root);
    printf("\n");
}

void disp_preorder(struct tree *ptr_tree)
{
    printf("Tree preorder : ");
    disp_preorder_recursive(ptr_tree->elem, ptr_tree->root);
    printf("\n");
}

void disp_postorder(struct tree *ptr_tree)
{
    printf("Tree postorder: ");
    disp_postorder_recursive(ptr_tree->elem, ptr_tree->root);
    printf("\n");
}

void disp(struct tree *ptr_tree)
{
    printf("Tree : ");
    disp_inorder_recursive(ptr_tree->elem, ptr_tree->root);
    printf("\n");
}
```

## Related files

- [[02 - Array Representation of BST/00 - Complete Walkthrough]]
- [[02 - Array Representation of BST/Original Source Files/README - Source Files]]
