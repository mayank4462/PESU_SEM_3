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