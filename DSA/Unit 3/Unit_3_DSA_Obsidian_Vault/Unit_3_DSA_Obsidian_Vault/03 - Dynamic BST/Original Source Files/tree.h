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