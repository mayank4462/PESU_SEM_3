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