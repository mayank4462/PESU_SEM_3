#include <stdio.h>
#include <stdlib.h>
#include "tree_array.h"

void init(struct tree *ptr_tree)
{
    ptr_tree->root = -1;
    ptr_tree->count = 0;
}

// insertion functions

static int insert_recursive(struct node elem[], int root, int temp)
{
    if(root == -1)
    {
        root = temp;
    }
    else if(elem[temp].key < elem[root].key)
    {
        elem[root].left = insert_recursive(elem, elem[root].left, temp);
    }
    else
    {
        elem[root].right = insert_recursive(elem, elem[root].right, temp);
    }
    return root;
}

void insert(struct tree *ptr_tree, int key)
{
    if(ptr_tree->count == MAXSIZE)
    {
        printf("tree full\n");
        exit(1);
    }

    // count tells us the next available index in the array, temp tells us the index of the new node to be inserted

    int temp = ptr_tree->count++; 
    ptr_tree->elem[temp].key = key;
    ptr_tree->elem[temp].left = ptr_tree->elem[temp].right = -1;

    ptr_tree->root = insert_recursive(ptr_tree->elem, ptr_tree->root, temp);
}


// Other Functions

static int number_of_nodes_recursive(struct node elem[], int temp)
{
    if(temp == -1)
    {
        return 0;
    }
    else
    {
        return 1 + number_of_nodes_recursive(elem, elem[temp].left)
                 + number_of_nodes_recursive(elem, elem[temp].right);
    }
}

int number_of_nodes(struct tree *ptr_tree)
{
    return number_of_nodes_recursive(ptr_tree->elem, ptr_tree->root);
}

static int number_of_leaves_recursive(struct node elem[], int temp)
{
    if(temp == -1)
    {
        return 0;
    }
    else if(elem[temp].left == -1 && elem[temp].right == -1)
    {
        return 1;
    }
    else
    {
        return number_of_leaves_recursive(elem, elem[temp].left)
             + number_of_leaves_recursive(elem, elem[temp].right);
    }
}

int number_of_leaves(struct tree *ptr_tree)
{
    return number_of_leaves_recursive(ptr_tree->elem, ptr_tree->root);
}

static int are_same_recursive(struct node elem1[], int root1,
                              struct node elem2[], int root2)
{
    if(root1 == -1 && root2 == -1)
    {
        return 1;
    }
    else if(root1 == -1 || root2 == -1)
    {
        return 0;
    }
    else
    {
        return elem1[root1].key == elem2[root2].key &&
               are_same_recursive(elem1, elem1[root1].left,
                                  elem2, elem2[root2].left) &&
               are_same_recursive(elem1, elem1[root1].right,
                                  elem2, elem2[root2].right);
    }
}

int are_same(struct tree *ptr_tree1, struct tree *ptr_tree2)
{
    return are_same_recursive(ptr_tree1->elem, ptr_tree1->root,
                              ptr_tree2->elem, ptr_tree2->root);
}

void copy(struct tree *ptr_src, struct tree *ptr_dst)
{
    ptr_dst->root = ptr_src->root;
    ptr_dst->count = ptr_src->count;

    for(int i = 0; i < ptr_src->count; ++i)
    {
        ptr_dst->elem[i] = ptr_src->elem[i];
    }
}


// Display functions

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

void disp(struct tree *ptr_tree)
{
    printf("Tree : ");
    disp_inorder_recursive(ptr_tree->elem, ptr_tree->root);
    printf("\n");
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

void deinit(struct tree *ptr_tree)
{
    ptr_tree->root = -1;
    ptr_tree->count = 0;
}

/*

init
- root = -1 (indicates an empty tree)
- count = 0 (next available index in the array)

deinit
- No need to free individual nodes since everything is stored in a static array
- Simply reset root = -1 and count = 0

insert
- Arguments: tree pointer and key
- First check whether the array is full (count == MAXSIZE)
- count tells us the next available index in the array
- Store this index in temp and increment count
- Insert the key at elem[temp].key
- Initialize its left and right as -1
- Call insert_recursive by passing the array, root index and temp index
- Update tree->root with the returned root index

insert_recursive
- Receives the array, root index and temp (new node's index)
- If root == -1, the position is empty, so return temp
- If temp's key < root's key:
    - Insert into the left subtree
    - root's left = recursive call passing root's left index
- Else:
    - Insert into the right subtree
    - root's right = recursive call passing root's right index
- Return root index

IMPORTANT:
- Here we don't use actual pointers to connect nodes
- Left and right store array indices
- -1 represents NULL
- root also stores an array index, not an address

number_of_nodes
- Receives the tree pointer
- Calls recursive function by passing the array and root index

number_of_nodes_recursive
- Receives the array and current node index (temp)
- If temp == -1, return 0
- Otherwise:
    - Return 1 + recursive call on left subtree + recursive call on right subtree
    - 1 accounts for the current node

number_of_leaves
- Receives the tree pointer
- Calls recursive function by passing the array and root index

number_of_leaves_recursive
- Receives the array and current node index (temp)
- If temp == -1, return 0
- If both left and right indices are -1, it is a leaf node, so return 1
- Otherwise:
    - Recursive call on left subtree + recursive call on right subtree

are_same
- Receives two tree pointers
- Calls recursive function by passing both arrays and their respective root indices

are_same_recursive
- Receives two arrays and their respective root indices
- If both root indices are -1, both trees are empty at this position, so SAME
- If either root index is -1, NOT SAME
- Otherwise:
    - Check whether both corresponding node keys match
    - Recursively compare both left subtrees
    - Recursively compare both right subtrees
- All three conditions must be TRUE
- Checks both structure and corresponding keys, not just the values

copy
- Receives source tree pointer and destination tree pointer
- First copies the source root index into destination root
- Copies the source count into destination count
- Then copies all populated array elements (0 to count-1)
- Since the nodes are stored in an array, no recursive copying is required
- The destination gets an independent copy of the array contents

disp (inorder, preorder, postorder)
- Receives the tree pointer
- Calls the respective recursive function by passing the array and root index

disp_inorder_recursive
- If temp != -1:
    - Recursively visit left subtree
    - Print current node's key
    - Recursively visit right subtree
- Order: LEFT ROOT RIGHT (LROOTR)

disp_preorder_recursive
- If temp != -1:
    - Print current node's key
    - Recursively visit left subtree
    - Recursively visit right subtree
- Order: ROOT LEFT RIGHT (RL R)

disp_postorder_recursive
- If temp != -1:
    - Recursively visit left subtree
    - Recursively visit right subtree
    - Print current node's key
- Order: LEFT RIGHT ROOT (LRROOT)

*/