---
title: 04 — Delete a Key from Dynamic BST
tags: [unit-3, c, code]
---

# 04 — Delete a Key from Dynamic BST

**Source file:** `Original Source Files/tree.c`  
**Purpose:** Deletes a value while preserving the BST property.

## Explanation

The two-child case replaces the node value with its inorder successor and removes that successor from the right subtree.

## Code

```c
struct node *inorder_successor(struct node *root) // leftmost of the right subtree
{
    struct node *temp = root;
    while(temp->left != NULL)
        temp = temp->left;
    return temp;
}

struct node *delete_key_recursive(struct node *root, int key)
{
    // search for the key
    if(root == NULL)
    {
        return root;
    }
    else if(key < root->key)
    {
        root->left = delete_key_recursive(root->left, key);
    }
    else if(key > root->key)
    {
        root->right = delete_key_recursive(root->right, key);
    }
    else // key matches
    {
        // 1. leaf
        if(root->left == NULL && root->right == NULL)
        {
            free(root);
            return NULL;
        }

        // 2. no left
        else if(root->left == NULL)
        {
            struct node *temp = root->right;
            free(root);
            return temp;
        }

        // 3. no right
        else if(root->right == NULL)
        {
            struct node *temp = root->left;
            free(root);
            return temp;
        }

        // 4. has both left and right
        // find the inorder successor
        else
        {
            struct node *temp = inorder_successor(root->right);
            root->key = temp->key;
            root->right = delete_key_recursive(root->right, temp->key);
        }
    }
    return root;
}

void delete_key(struct tree *ptr_tree, int key)
{
    ptr_tree->root = delete_key_recursive(ptr_tree->root, key);
}
```

## Related files

- [[03 - Dynamic BST/00 - Complete Walkthrough]]
- [[03 - Dynamic BST/Original Source Files/README - Source Files]]
