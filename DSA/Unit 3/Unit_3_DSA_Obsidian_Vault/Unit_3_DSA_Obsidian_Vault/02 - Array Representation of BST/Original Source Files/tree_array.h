#ifndef TREE_ARRAY_H
#define TREE_ARRAY_H

#define MAXSIZE 200

struct node
{
    int left;
    int key;
    int right;
};

struct tree
{
    int root;
    int count;
    struct node elem[MAXSIZE];
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

#endif