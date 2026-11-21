---
title: Threaded Binary Search Tree
tags: [unit-3, c, threaded-bst, inorder, tree-traversal]
---

# Threaded Binary Search Tree

## 1. Why a threaded BST?

In an ordinary binary tree, a node's missing left and right child pointers are `NULL`. A threaded BST reuses these otherwise-empty links to store inorder predecessor and successor information.

The node contains:

- `key`: node value.
- `left`, `right`: child pointers or thread pointers.
- `lthread`, `rthread`: flags that identify what each pointer represents.

### Thread flag meaning

| Flag | Value | Meaning |
|---|---:|---|
| `lthread` | `0` | `left` points to an actual left child |
| `lthread` | `1` | `left` points to inorder predecessor (thread) |
| `rthread` | `0` | `right` points to an actual right child |
| `rthread` | `1` | `right` points to inorder successor (thread) |

This code uses `1` to mean thread and `0` to mean child link. Do not reverse these meanings.

## 2. Node creation

`create_node()` allocates one node, stores the key and initially sets both pointers to `NULL`. It sets both thread flags to `1` because a newly inserted node initially has no child links; both pointers represent its predecessor/successor threads.

## 3. Insertion

`insert_key()` searches from the root:

- To move left, it must have `lthread == 0`; otherwise the left pointer is a thread and the insertion location has been reached.
- To move right, it must have `rthread == 0`; otherwise the right pointer is a thread and the insertion location has been reached.

### Insert as left child

For a new node inserted to the left of `prev`:

- New node's left thread inherits `prev`'s old left thread (the predecessor).
- New node's right thread points to `prev` (its inorder successor).
- `prev->left` becomes the new node.
- `prev->lthread` becomes `0`, marking an actual left-child link.

### Insert as right child

For a new node inserted to the right of `prev`:

- New node's right thread inherits `prev`'s old right thread (the successor).
- New node's left thread points to `prev` (its inorder predecessor).
- `prev->right` becomes the new node.
- `prev->rthread` becomes `0`, marking an actual right-child link.

Duplicates go to the right because the search uses `if (key < curr->key)` for left and the `else` branch for right.

## 4. Inorder traversal without recursion or stack

`leftmost()` follows actual left-child links (`lthread == 0`) until it reaches the first inorder node.

`inorder()` then repeats:

1. Print the current node.
2. If `rthread == 1`, follow the right pointer directly to the inorder successor.
3. Otherwise, the right pointer is a real child; move to the leftmost node in that right subtree.
4. Stop when the pointer becomes `NULL`.

Thus, the threaded links allow inorder traversal without recursion and without an auxiliary stack.

## 5. Deallocation

`deinit()` follows the same inorder-successor navigation, but it saves the next node before freeing the current one. This order is essential: once a node is freed, its pointers must not be accessed.

For each node:

- If its right pointer is a thread, the successor is `temp->right`.
- Otherwise, its successor is the leftmost node in its right subtree.
- Save that successor, then free the current node.

After all nodes are freed, `root` is set to `NULL`.

## 6. Client test cases

The client builds three trees:

1. `{40, 20, 60, 10, 70, 30, 50}` — a mixed shape.
2. `{10, 20, 30, 40, 50, 60, 70}` — an increasingly inserted sequence, producing a right-skewed BST.
3. An empty tree.

Each case is displayed using threaded inorder traversal and then deallocated.

## 7. Source code

### `tree.h`

```c
#ifndef TREE_H
#define TREE_H

struct node
{
    int key;
    struct node *left;
    struct node *right;
    int lthread;
    int rthread;
};

struct tree
{
    struct node *root;
};

void init(struct tree *ptr_tree);
void deinit(struct tree *ptr_tree);
void disp(struct tree *ptr_tree);
void insert(struct tree *ptr_tree, int key);

#endif
```
### `tree.c`

```c
#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

static struct node *leftmost(struct node *temp)
{
    if(temp == NULL)
        return NULL;

    while(temp->lthread == 0)
        temp = temp->left;

    return temp;
}

// Follow successor threads to traverse inorder without recursion or a stack.
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

// tree functions

void init(struct tree *ptr_tree)
{
    ptr_tree->root = NULL;
}

void deinit(struct tree *ptr_tree)
{
    struct node *temp = leftmost(ptr_tree->root);
    struct node *curr;

    while(temp != NULL)
    {
        curr = temp;

        if(temp->rthread == 1)
            temp = temp->right;
        else
            temp = leftmost(temp->right);

        free(curr);
    }

    ptr_tree->root = NULL;
}

void disp(struct tree *ptr_tree)
{
    printf("Tree : ");
    inorder(ptr_tree->root);
    printf("\n");
}

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

// When inserting, preserve predecessor/successor threads around the new node.
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

/*

init
- root = NULL (indicates an empty tree)

create_node
- Receives the key
- Dynamically allocates memory for a new node
- If memory allocation fails, exit
- Initializes:
    - temp->key = key
    - temp->left = NULL
    - temp->right = NULL
    - temp->lthread = 1
    - temp->rthread = 1
- Both thread flags are initialized to 1 because initially both left and right pointers are threads, not child links
- Returns the newly created node

IMPORTANT:
- lthread = 0 means left points to an actual child
- lthread = 1 means left points to inorder predecessor (thread)
- rthread = 0 means right points to an actual child
- rthread = 1 means right points to inorder successor (thread)

insert
- Receives tree pointer and key
- Calls insert_key by passing the current root and key
- Updates tree->root with the returned root

insert_key
- Receives root and key
- First creates a new node using create_node
- If root == NULL:
    - Tree is empty
    - Return the newly created node as root

- Otherwise:
    - Initialize prev = NULL and curr = root
    - Traverse the tree to find the appropriate insertion position

- While curr != NULL:
    - prev = curr (stores the parent of the current node)

    - If key < curr->key:
        - Check whether lthread == 0
        - If yes, left is an actual child, so move curr to curr->left
        - Otherwise, left is a thread, so the insertion position is found, break

    - Else:
        - Check whether rthread == 0
        - If yes, right is an actual child, so move curr to curr->right
        - Otherwise, right is a thread, so the insertion position is found, break

- Once the insertion position is found, insert the new node

CASE 1: INSERT AS LEFT CHILD
- If key < prev->key:
    - temp->left = prev->left
        - New node's left thread points to the parent's previous inorder predecessor
    - temp->right = prev
        - New node's right thread points to its inorder successor (parent)
    - prev->left = temp
        - Parent's left now points to the new node
    - prev->lthread = 0
        - Indicates that parent's left is now an actual child link

CASE 2: INSERT AS RIGHT CHILD
- Else:
    - temp->right = prev->right
        - New node's right thread points to the parent's previous inorder successor
    - temp->left = prev
        - New node's left thread points to its inorder predecessor (parent)
    - prev->right = temp
        - Parent's right now points to the new node
    - prev->rthread = 0
        - Indicates that parent's right is now an actual child link

- Finally, return root

IMPORTANT:
- Unlike a normal BST, we must maintain predecessor and successor threads during insertion
- A newly inserted node initially has both pointers as threads
- Only the parent's corresponding thread is converted into an actual child link
- Duplicate keys go into the right subtree because the condition is key < curr->key, otherwise right

leftmost
- Receives a node pointer
- If temp == NULL, return NULL
- Otherwise:
    - Keep moving towards the left child while lthread == 0
    - Stop when lthread == 1
- Returns the leftmost node
- This is the first node in inorder traversal

inorder
- Receives the root pointer
- First finds the leftmost node using leftmost(root)
- Store it in temp
- While temp != NULL:
    - Print temp->key
    - If rthread == 1:
        - Right pointer is a thread
        - Move directly to the inorder successor using temp->right
    - Else:
        - Right pointer is an actual child
        - Move to the leftmost node of the right subtree using leftmost(temp->right)
    
- Continues until temp == NULL

IMPORTANT:
- In a normal BST, inorder traversal generally uses recursion or a stack
- In a threaded BST, we use the threads to move directly to the inorder successor
- No recursion or auxiliary stack is required
- Inorder sequence: LEFT ROOT RIGHT (LROOTR)

disp
- Receives tree pointer
- Prints "Tree : "
- Calls inorder by passing tree->root
- Prints newline

deinit
- Receives tree pointer
- First finds the leftmost node using leftmost(root)
- Store it in temp

- While temp != NULL:
    - Store current node in curr
    - Find the next node BEFORE freeing the current node

    - If rthread == 1:
        - Right pointer directly points to the inorder successor
        - Move temp to temp->right

    - Else:
        - Right pointer is an actual child
        - Find the leftmost node of the right subtree
        - Move temp to leftmost(temp->right)

    - Free curr

- After all nodes are freed:
    - Set tree->root = NULL

IMPORTANT:
- Deallocation follows inorder traversal
- We calculate the next node before freeing the current node
- This is important because we should not access a node after freeing it
- No recursion or stack is required for deallocation either

*/
```
### `client.c`

```c
#include <stdio.h>
#include "tree.h"

int main()
{
    // case 1
    struct tree t1;
    init(&t1);

    int a1[] = {40, 20, 60, 10, 70, 30, 50};
    int n1 = 7;

    for(int i = 0; i < n1; ++i)
    {
        insert(&t1, a1[i]);
    }

    disp(&t1);
    deinit(&t1);

    // case 2
    struct tree t2;
    init(&t2);

    int a2[] = {10, 20, 30, 40, 50, 60, 70};
    int n2 = 7;

    for(int i = 0; i < n2; ++i)
    {
        insert(&t2, a2[i]);
    }

    disp(&t2);
    deinit(&t2);

    // case 3
    struct tree t3;
    init(&t3);

    disp(&t3);
    deinit(&t3);

    return 0;
}
```
## 8. Complexity and exam checkpoints

- Inorder traversal visits each node once: `O(n)` time and `O(1)` auxiliary space for the iterative traversal shown.
- Insertion takes `O(h)` search time, where `h` is the BST height.
- A skewed BST can still have height `O(n)`; threading removes the traversal stack requirement, not the BST's worst-case height.
- Threads are not child edges. Always inspect the corresponding flag before following a pointer as a child.
- During deallocation, compute the successor before calling `free()`.
