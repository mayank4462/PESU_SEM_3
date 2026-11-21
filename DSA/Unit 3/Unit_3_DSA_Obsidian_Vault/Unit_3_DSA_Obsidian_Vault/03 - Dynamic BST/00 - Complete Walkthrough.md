---
title: Dynamic Binary Search Tree
tags: [unit-3, c, bst, pointers, dynamic-memory, deletion]
---

# Dynamic Binary Search Tree (Pointer Representation)

## 1. Representation

Unlike the array implementation, each node is allocated dynamically and connected using pointers:

- `key`: node value.
- `left`: pointer to left child.
- `right`: pointer to right child.
- `root`: pointer to the root node.

`NULL` indicates an empty tree or missing child. `init()` sets `root = NULL`.

## 2. Insertion

`insert()` allocates a node using `malloc()`, stores the key and initializes both child pointers to `NULL`. It then calls `insert_recursive()`.

`insert_recursive()`:

- Returns the new node when the current subtree root is `NULL`.
- Recurses left when `temp->key < root->key`.
- Otherwise recurses right.
- Returns the current root so the parent can reconnect the modified subtree.

Duplicates are inserted into the right subtree.

## 3. Traversals

Three recursive functions implement the standard orders:

- Inorder: Left → Root → Right (`LNR`). For a BST, the output is sorted.
- Preorder: Root → Left → Right (`NLR`).
- Postorder: Left → Right → Root (`LRN`).

In each function, the base case is a `NULL` pointer.

## 4. Counting nodes and leaves

- Node count: `1 + count(left) + count(right)`.
- Leaf count: return `1` when both child pointers are `NULL`; otherwise add the leaf counts of left and right.
- A `NULL` subtree contributes zero to either count.

## 5. Deletion — four situations in this implementation

The search part follows the BST property: move left for a smaller key and right for a larger key. When the target is found:

### Case 1: Leaf node

Both children are `NULL`. Free the node and return `NULL`, so the parent's corresponding link becomes empty.

### Case 2: No left child (only right child)

Save `root->right`, free the target node, and return the saved right child. The child takes the target's position.

### Case 3: No right child (only left child)

Save `root->left`, free the target node, and return the saved left child.

### Case 4: Two children

1. Find the **inorder successor**, the leftmost node in the target's right subtree.
2. Copy the successor's key into the target node.
3. Delete the original successor from the right subtree using a recursive call.

The assignment `root->right = delete_key_recursive(root->right, temp->key)` is essential: it reconnects the right subtree after removing the duplicate successor node.

## 6. Copying and comparing

### Copy

`copy_recursive()` creates a new node for every source node, copies its key, then recursively copies the left and right subtrees. `copy()` assigns the returned root to the destination. This is a **deep copy**: source and destination do not share tree nodes.

### Compare

`are_same_recursive()` checks:

1. both pointers are `NULL` → same at this position;
2. exactly one pointer is `NULL` → different;
3. otherwise, keys must match and both corresponding subtrees must match.

It compares shape and values, not merely the sorted sequences.

## 7. Deallocation

`deinit_recursive()` uses postorder (Left, Right, Root): it frees both subtrees before freeing the current node. This avoids losing access to child pointers before their nodes are released. Finally, `deinit()` sets the public root pointer to `NULL`.

## 8. Source code

### `tree.h`

```c
#ifndef TREE_H
#define TREE_H

struct node
{
    int key;
    struct node *left;
    struct node *right;
};

struct tree
{
    struct node *root;
};

void init(struct tree *ptr_tree);
void deinit(struct tree *ptr_tree);

void disp(struct tree *ptr_tree);
void disp_preorder(struct tree *ptr_tree);
void disp_postorder(struct tree *ptr_tree);
void disp_inorder(struct tree *ptr_tree);

void insert(struct tree *ptr_tree, int key);

int number_of_nodes(struct tree *ptr_tree);
int number_of_leaves(struct tree *ptr_tree);

void copy(struct tree *ptr_src, struct tree *ptr_dst);
int are_same(struct tree *ptr_tree1, struct tree *ptr_tree2);

void delete_key(struct tree *ptr_tree, int key);

#endif
```
### `tree.c`

```c
#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

void init(struct tree *ptr_tree)
{
    ptr_tree->root = NULL;
}

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

// Link the dynamically allocated node into its BST position.
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
        // Replace with the inorder successor, then delete its old node.
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

// finding the number of nodes and leaves

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

// copying a tree

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

// comparing two trees

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

static void deinit_recursive(struct node *root)
{
    if(root)
    {
        deinit_recursive(root->left);
        deinit_recursive(root->right);
        free(root);
    }
}

void deinit(struct tree *ptr_tree)
{
    deinit_recursive(ptr_tree->root);
    ptr_tree->root = NULL;
}

/*
init
- root = NULL

deinit 
- free nodes in postorder format (LEFT RIGHT ROOT)
- recursive call

disp (preorder, postorder, inorder)
- accordingly ROOTLR, LRROOT, LROOTR

insert
- arguments accepts tree pointer and the key
- tree->root - call the recursive function by passing it the root, key

insert_recursive
- it receives the root, temp node
- if root is NULL, root is temp
- if temp's key < root's key, root->left - recursive call by passing root->left, temp
- else root->right - recursive call by passing root->right, temp

delete
- receives tree pointer, key
- calls tree->root - recursive call by passing the root, key to be deleted

delete_recursive
- receives the root, key
- if root is NULL, return NULL
- first search using same pattern root == NULL, temp->key < root->key and temp->key > root->key
- when key is found, we have 3 cases
- 1st Case: Leaf Node
    - if root's right and left is NULL, its a leaf node, so directly free the root and return NULL

- 2nd Case: Either Left Child or Right Child
    - If Left Child Exists
        - Make the root->left as successor
    - If Right Child Exists
        - Make the root->right as successor

- 3rd Case: Both Left and Right Child Exists
    - We find inorder successor by passing the found root's->right
    - Inorder Successor receives the root->right as temp, and then goes leftward until NULL which gives the leftmost of the right subtree
    - Then we copy the temp->key to root->key
    - Then importantly, we call root->right = delete_recursive(root->right, temp->key) 
        - In this we are starting from one down, because after copying we want the old one to be deleted

number_of_nodes
- if root is NULL, then 0
- recursive call of 1 + func(left) + func(right)

number_of_leaves
- if root is NULL, then 0
- if root's left and right are NULL, then return 1
- recursive call of func(left) + func(right)

are_same
- it passes the tree1->root, tree2->root to the recursive function

are_same_recurisve
- it receievs the root1, root2
- first it checks if both are NULL, then SAME
- then check, if either of them is NULL, NOT SAME
- then check if both keys match && recursive call passing both's left && recursive call passing both's right

copy_tree
- receives the source tree pointer and destination tree pointer
- calls recursive function by ptr_dest->root = copy_tree_recursive(ptr_tree->root)

copy_tree_recursive
- it receieves the source root
- if ptr_src->root is NOT NULL
    - creates a temp node
    - copies the key
    - copies the left pointer recursively passing the src's left
    - copies the right pointer recursively passing the src's right


*/
```
### `client.c`

```c
#include <stdio.h>
#include "tree.h"

int main()
{
    // create and insert
    struct tree t1;
    init(&t1);

    int a1[] = {40, 20, 60, 10, 70, 30, 50};
    int n1 = 7;
    for(int i = 0; i < n1; ++i)
    {
        insert(&t1, a1[i]);
    }

    disp(&t1);

    // number of nodes and leaves
    printf("# of nodes : %d\n", number_of_nodes(&t1));
    printf("# of leaves : %d\n", number_of_leaves(&t1));

    // traversals
    disp_preorder(&t1);
    disp_postorder(&t1);
    disp_inorder(&t1);

    // copy and compare trees
    struct tree t2;
    init(&t2);
    copy(&t1, &t2);
    disp(&t2);
    printf("same : %d\n", are_same(&t1, &t2));

    struct tree t3;
    init(&t3);
    int a3[] = {10, 20, 30, 40, 50, 60, 70};
    int n3 = 7;
    for(int i = 0; i < n3; ++i)
    {
        insert(&t3, a3[i]);
    }
    printf("same : %d\n", are_same(&t1, &t3));

    // delete a leaf
    delete_key(&t1, 10);
    printf("after deleting 10 : ");
    disp_inorder(&t1);

    // delete a node with no left subtree
    delete_key(&t1, 20);
    printf("after deleting 20 : ");
    disp_inorder(&t1);

    // delete a node with both left and right subtrees
    delete_key(&t1, 60);
    printf("after deleting 60 : ");
    disp_inorder(&t1);

    // delete the root node
    delete_key(&t1, 40);
    printf("after deleting 40 : ");
    disp_inorder(&t1);

    delete_key(&t1, 70);
    printf("after deleting 70 : ");
    disp_inorder(&t1);

    delete_key(&t1, 50);
    printf("after deleting 50 : ");
    disp_inorder(&t1);

    delete_key(&t1, 30);
    printf("after deleting 30 : ");
    disp_inorder(&t1);

    // empty tree
    struct tree t4;
    init(&t4);
    disp(&t4);
    printf("# of nodes : %d\n", number_of_nodes(&t4));
    printf("# of leaves : %d\n", number_of_leaves(&t4));

    deinit(&t1);
    deinit(&t2);
    deinit(&t3);
    deinit(&t4);

    return 0;
}
```
## 9. Source-code cautions

- `insert()` does not check whether `malloc()` returned `NULL` before dereferencing the result.
- `copy_recursive()` also does not check allocation failure.
- `copy()` assumes the destination is empty or has already been deallocated. Copying over a non-empty destination without first freeing it would leak the old destination nodes.
- `inorder_successor()` is defined in `tree.c` but is only used by deletion in that file.

## 10. Complexity and exam checkpoints

- Search/insertion/deletion: `O(h)`; worst case `O(n)` in a skewed tree.
- Traversals, node count, leaf count, copy, compare and deallocation: `O(n)`.
- Dynamic storage grows node by node; unlike the array version, it does not have a fixed `MAXSIZE` limit in the data structure.
- For deletion with two children, use the inorder successor (or predecessor in another valid implementation), copy its key, and remove the successor node from its old position.
