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