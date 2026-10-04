# BST — Unit 3 Obsidian Notes

## 1. What is a Binary Search Tree?

A **Binary Search Tree (BST)** is a binary tree with an ordering property.

For every node:

- Values in the **left subtree** are smaller than the node's value.
- Values in the **right subtree** are greater than or equal to the node's value, according to the insertion rule used in the source.

Example:

```text
          50
        /    \
      30      70
     /  \    /  \
   20   40  60   80
```

This property allows efficient searching because we do not need to examine both subtrees.

### Searching for 60

```text
Start at 50

60 > 50
    ↓
go right

60 < 70
    ↓
go left

60 == 60
    ↓
FOUND
```

---

# 2. Basic BST Structures

## Node structure

```c
struct node
{
    int key_;
    struct node *left_;
    struct node *right_;
};

typedef struct node node_t;
```

Each node contains:

```text
+---------+---------+---------+
| key_    | left_   | right_  |
+---------+---------+---------+
```

- `key_` stores the data.
- `left_` points to the left child.
- `right_` points to the right child.

## Tree structure

```c
struct tree
{
    node_t *root_;
};

typedef struct tree tree_t;
```

The tree object only needs a pointer to its root.

```text
tree
 |
 root
  |
  ↓
 node
```

---

# 3. Complete Pointer-Based BST Code

```c
#include <stdio.h>
#include <stdlib.h>

struct node
{
    int key_;
    struct node *left_;
    struct node *right_;
};

typedef struct node node_t;

struct tree
{
    node_t *root_;
};

typedef struct tree tree_t;

void init(tree_t *ptr_tree);
void deinit(tree_t *ptr_tree);
void disp_inorder(tree_t *ptr_tree);
void disp_preorder(tree_t *ptr_tree);
void disp_postorder(tree_t *ptr_tree);
void insert(tree_t *ptr_tree, int key);
int number_of_nodes(tree_t *ptr_tree);
int number_of_leaves(tree_t *ptr_tree);
int are_same(tree_t *ptr_tree1, tree_t *ptr_tree2);
void copy(tree_t *ptr_src, tree_t *ptr_dst);
node_t *search(tree_t *ptr_tree, int key);
void delete_key(tree_t *ptr_tree, int key);

static node_t *insert_recursive(node_t *root, node_t *temp);
static void deinit_recursive(node_t *root);
static void disp_inorder_recursive(node_t *root);
static void disp_preorder_recursive(node_t *root);
static void disp_postorder_recursive(node_t *root);
static int number_of_nodes_recursive(node_t *root);
static int number_of_leaves_recursive(node_t *root);
static int are_same_recursive(node_t *root1, node_t *root2);
static node_t *copy_recursive(node_t *root);
static node_t *search_recursive(node_t *root, int key);
static node_t *inorder_successor(node_t *root);
static node_t *delete_key_recursive(node_t *root, int key);

void init(tree_t *ptr_tree)
{
    ptr_tree->root_ = NULL;
}

void insert(tree_t *ptr_tree, int key)
{
    node_t *temp = (node_t *)malloc(sizeof(node_t));

    temp->key_ = key;
    temp->left_ = temp->right_ = NULL;

    ptr_tree->root_ = insert_recursive(ptr_tree->root_, temp);
}

static node_t *insert_recursive(node_t *root, node_t *temp)
{
    if(root == NULL)
    {
        root = temp;
    }
    else if(root->key_ > temp->key_)
    {
        root->left_ = insert_recursive(root->left_, temp);
    }
    else
    {
        root->right_ = insert_recursive(root->right_, temp);
    }

    return root;
}

void disp_inorder(tree_t *ptr_tree)
{
    printf("Tree inorder: ");
    disp_inorder_recursive(ptr_tree->root_);
    printf("\n");
}

static void disp_inorder_recursive(node_t *root)
{
    if(root)
    {
        disp_inorder_recursive(root->left_);
        printf("%d ", root->key_);
        disp_inorder_recursive(root->right_);
    }
}

void disp_preorder(tree_t *ptr_tree)
{
    printf("Tree preorder: ");
    disp_preorder_recursive(ptr_tree->root_);
    printf("\n");
}

static void disp_preorder_recursive(node_t *root)
{
    if(root)
    {
        printf("%d ", root->key_);
        disp_preorder_recursive(root->left_);
        disp_preorder_recursive(root->right_);
    }
}

void disp_postorder(tree_t *ptr_tree)
{
    printf("Tree postorder: ");
    disp_postorder_recursive(ptr_tree->root_);
    printf("\n");
}

static void disp_postorder_recursive(node_t *root)
{
    if(root)
    {
        disp_postorder_recursive(root->left_);
        disp_postorder_recursive(root->right_);
        printf("%d ", root->key_);
    }
}

void deinit(tree_t *ptr_tree)
{
    deinit_recursive(ptr_tree->root_);
    ptr_tree->root_ = NULL;
}

static void deinit_recursive(node_t *root)
{
    if(root)
    {
        deinit_recursive(root->left_);
        deinit_recursive(root->right_);
        free(root);
    }
}

int number_of_nodes(tree_t *ptr_tree)
{
    return number_of_nodes_recursive(ptr_tree->root_);
}

static int number_of_nodes_recursive(node_t *root)
{
    if(root == NULL)
    {
        return 0;
    }

    return 1 + number_of_nodes_recursive(root->left_)
             + number_of_nodes_recursive(root->right_);
}

int number_of_leaves(tree_t *ptr_tree)
{
    return number_of_leaves_recursive(ptr_tree->root_);
}

static int number_of_leaves_recursive(node_t *root)
{
    if(root == NULL)
    {
        return 0;
    }

    if(root->left_ == NULL && root->right_ == NULL)
    {
        return 1;
    }

    return number_of_leaves_recursive(root->left_)
         + number_of_leaves_recursive(root->right_);
}

int are_same(tree_t *ptr_tree1, tree_t *ptr_tree2)
{
    return are_same_recursive(ptr_tree1->root_, ptr_tree2->root_);
}

static int are_same_recursive(node_t *root1, node_t *root2)
{
    if(root1 == NULL && root2 == NULL)
    {
        return 1;
    }

    if(root1 == NULL || root2 == NULL)
    {
        return 0;
    }

    return root1->key_ == root2->key_
        && are_same_recursive(root1->left_, root2->left_)
        && are_same_recursive(root1->right_, root2->right_);
}

void copy(tree_t *ptr_src, tree_t *ptr_dst)
{
    ptr_dst->root_ = copy_recursive(ptr_src->root_);
}

static node_t *copy_recursive(node_t *root)
{
    node_t *temp;

    if(root != NULL)
    {
        temp = (node_t *)malloc(sizeof(node_t));

        temp->key_ = root->key_;
        temp->left_ = copy_recursive(root->left_);
        temp->right_ = copy_recursive(root->right_);

        return temp;
    }

    return NULL;
}

node_t *search(tree_t *ptr_tree, int key)
{
    return search_recursive(ptr_tree->root_, key);
}

static node_t *search_recursive(node_t *root, int key)
{
    if(root == NULL)
    {
        return NULL;
    }

    if(key == root->key_)
    {
        return root;
    }

    if(key < root->key_)
    {
        return search_recursive(root->left_, key);
    }

    return search_recursive(root->right_, key);
}

static node_t *inorder_successor(node_t *root)
{
    node_t *temp = root;

    while(temp->left_ != NULL)
    {
        temp = temp->left_;
    }

    return temp;
}

void delete_key(tree_t *ptr_tree, int key)
{
    ptr_tree->root_ = delete_key_recursive(ptr_tree->root_, key);
}

static node_t *delete_key_recursive(node_t *root, int key)
{
    if(root == NULL)
    {
        return NULL;
    }

    if(key < root->key_)
    {
        root->left_ = delete_key_recursive(root->left_, key);
    }
    else if(key > root->key_)
    {
        root->right_ = delete_key_recursive(root->right_, key);
    }
    else
    {
        if(root->left_ == NULL && root->right_ == NULL)
        {
            free(root);
            return NULL;
        }

        if(root->left_ == NULL)
        {
            node_t *temp = root->right_;
            free(root);
            return temp;
        }

        if(root->right_ == NULL)
        {
            node_t *temp = root->left_;
            free(root);
            return temp;
        }

        node_t *temp = inorder_successor(root->right_);

        root->key_ = temp->key_;

        root->right_ =
            delete_key_recursive(root->right_, temp->key_);
    }

    return root;
}
```

---

# 4. `init()`

```c
void init(tree_t *ptr_tree)
{
    ptr_tree->root_ = NULL;
}
```

Initially:

```text
tree
 |
 root
  |
 NULL
```

The tree is empty.

---

# 5. BST Insertion

Insertion is divided into two functions:

```text
insert()
    ↓
insert_recursive()
```

The tree-level function handles the tree object.

The recursive function handles the current subtree.

## Code

```c
void insert(tree_t *ptr_tree, int key)
{
    node_t *temp = (node_t *)malloc(sizeof(node_t));

    temp->key_ = key;
    temp->left_ = temp->right_ = NULL;

    ptr_tree->root_ = insert_recursive(ptr_tree->root_, temp);
}
```

A new node is dynamically allocated.

For example:

```c
insert(&tree, 50);
```

creates:

```text
key_   = 50
left_  = NULL
right_ = NULL
```

Then the node is passed to the recursive insertion function.

## Recursive insertion

```c
static node_t *insert_recursive(node_t *root, node_t *temp)
{
    if(root == NULL)
    {
        root = temp;
    }
    else if(root->key_ > temp->key_)
    {
        root->left_ = insert_recursive(root->left_, temp);
    }
    else
    {
        root->right_ = insert_recursive(root->right_, temp);
    }

    return root;
}
```

### Case 1: Empty subtree

```c
if(root == NULL)
{
    root = temp;
}
```

The new node becomes the root of that subtree.

### Case 2: New data is smaller

```c
else if(root->key_ > temp->key_)
{
    root->left_ = insert_recursive(root->left_, temp);
}
```

Move to the left subtree.

### Case 3: New data is greater or equal

```c
else
{
    root->right_ = insert_recursive(root->right_, temp);
}
```

Move to the right subtree.

### Why assign the recursive result?

This is critical:

```c
root->left_ = insert_recursive(root->left_, temp);
```

and:

```c
root->right_ = insert_recursive(root->right_, temp);
```

The recursive function returns the new root of the modified subtree. The parent must reconnect its child pointer to that returned node.

---

# 6. Example of Insertion

Insert:

```text
50, 30, 70, 20, 40
```

Final tree:

```text
          50
        /    \
      30      70
     /  \
   20   40
```

For inserting `20`:

```text
20 < 50
    ↓
go left

20 < 30
    ↓
go left

NULL
    ↓
insert 20
```

---

# 7. Tree Traversals

There are three important traversals.

## Inorder

Order:

```text
LEFT → ROOT → RIGHT
```

```c
static void disp_inorder_recursive(node_t *root)
{
    if(root)
    {
        disp_inorder_recursive(root->left_);
        printf("%d ", root->key_);
        disp_inorder_recursive(root->right_);
    }
}
```

For:

```text
          50
        /    \
      30      70
     /  \
   20   40
```

Output:

```text
20 30 40 50 70
```

### Important BST property

**Inorder traversal of a BST gives sorted order.**

---

## Preorder

Order:

```text
ROOT → LEFT → RIGHT
```

```c
static void disp_preorder_recursive(node_t *root)
{
    if(root)
    {
        printf("%d ", root->key_);
        disp_preorder_recursive(root->left_);
        disp_preorder_recursive(root->right_);
    }
}
```

Output:

```text
50 30 20 40 70
```

---

## Postorder

Order:

```text
LEFT → RIGHT → ROOT
```

```c
static void disp_postorder_recursive(node_t *root)
{
    if(root)
    {
        disp_postorder_recursive(root->left_);
        disp_postorder_recursive(root->right_);
        printf("%d ", root->key_);
    }
}
```

Output:

```text
20 40 30 70 50
```

### Memorize

```text
Inorder    = L R? More precisely: L → ROOT → R
Preorder   = ROOT → L → R
Postorder  = L → R → ROOT
```

---

# 8. Deinitializing the Entire Tree

```c
void deinit(tree_t *ptr_tree)
{
    deinit_recursive(ptr_tree->root_);
    ptr_tree->root_ = NULL;
}
```

Recursive function:

```c
static void deinit_recursive(node_t *root)
{
    if(root)
    {
        deinit_recursive(root->left_);
        deinit_recursive(root->right_);
        free(root);
    }
}
```

The order is:

```text
LEFT
RIGHT
ROOT
```

This is postorder.

Why?

The children must be processed before the parent is freed.

---

# 9. Counting Number of Nodes

Logic:

```text
Empty tree → 0

Otherwise:

1
+ nodes in left subtree
+ nodes in right subtree
```

Code:

```c
static int number_of_nodes_recursive(node_t *root)
{
    if(root == NULL)
    {
        return 0;
    }

    return 1
         + number_of_nodes_recursive(root->left_)
         + number_of_nodes_recursive(root->right_);
}
```

Example:

```text
       50
      /  \
    30    70
   / \
 20  40
```

Count:

```text
50 → 1
30 → 1
70 → 1
20 → 1
40 → 1
```

Total:

```text
5
```

---

# 10. Counting Leaves

A leaf is a node with:

```text
left == NULL
AND
right == NULL
```

Code:

```c
static int number_of_leaves_recursive(node_t *root)
{
    if(root == NULL)
    {
        return 0;
    }

    if(root->left_ == NULL && root->right_ == NULL)
    {
        return 1;
    }

    return number_of_leaves_recursive(root->left_)
         + number_of_leaves_recursive(root->right_);
}
```

For:

```text
          50
        /    \
      30      70
     /  \
   20   40
```

Leaves:

```text
20
40
70
```

Therefore:

```text
3 leaves
```

---

# 11. Comparing Two Trees

Two trees are the same only when:

1. Their structures are the same.
2. Corresponding nodes contain the same data.

```c
static int are_same_recursive(node_t *root1, node_t *root2)
{
    if(root1 == NULL && root2 == NULL)
    {
        return 1;
    }

    if(root1 == NULL || root2 == NULL)
    {
        return 0;
    }

    return root1->key_ == root2->key_
        && are_same_recursive(root1->left_, root2->left_)
        && are_same_recursive(root1->right_, root2->right_);
}
```

### Case 1: Both are empty

```c
root1 == NULL && root2 == NULL
```

Return:

```text
1
```

### Case 2: Only one is empty

Return:

```text
0
```

### Case 3: Both exist

Check:

```text
data is same
AND
left subtree is same
AND
right subtree is same
```

---

# 12. Copying a Tree

The copy must create separate nodes.

```c
static node_t *copy_recursive(node_t *root)
{
    node_t *temp;

    if(root != NULL)
    {
        temp = (node_t *)malloc(sizeof(node_t));

        temp->key_ = root->key_;
        temp->left_ = copy_recursive(root->left_);
        temp->right_ = copy_recursive(root->right_);

        return temp;
    }

    return NULL;
}
```

The process is:

```text
1. Create a new node.
2. Copy the current node's data.
3. Recursively copy the left subtree.
4. Recursively copy the right subtree.
5. Return the new node.
```

The original and copy have the same structure and data, but they contain different dynamically allocated nodes.

---

# 13. Searching a BST

Search is one of the main reasons for using a BST.

```c
node_t *search(tree_t *ptr_tree, int key)
{
    return search_recursive(ptr_tree->root_, key);
}
```

Recursive function:

```c
static node_t *search_recursive(node_t *root, int key)
{
    if(root == NULL)
    {
        return NULL;
    }

    if(key == root->key_)
    {
        return root;
    }

    if(key < root->key_)
    {
        return search_recursive(root->left_, key);
    }

    return search_recursive(root->right_, key);
}
```

### Search logic

```text
key == root data
    → FOUND

key < root data
    → go LEFT

key > root data
    → go RIGHT
```

There is no need to search both subtrees because the BST property tells us where the value can be.

---

# 14. What Does Search Return?

Search returns:

```c
node_t *
```

If found:

```text
pointer to the node
```

If not found:

```text
NULL
```

Example:

```c
node_t *result = search(&tree, 40);
```

If `40` exists:

```text
result → node containing 40
```

---

# 15. Inorder Successor

The inorder successor is the **smallest value in the right subtree**.

Code:

```c
static node_t *inorder_successor(node_t *root)
{
    node_t *temp = root;

    while(temp->left_ != NULL)
    {
        temp = temp->left_;
    }

    return temp;
}
```

If the right subtree is:

```text
       70
      /  \
    60    80
   /
 55
```

The smallest value is:

```text
55
```

So `55` is the inorder successor.

### Shortcut

To find the inorder successor in a right subtree:

```text
Start at right subtree
        ↓
Keep going LEFT
        ↓
Stop at the leftmost node
```

---

# 16. BST Deletion

Deletion is the most important and most complicated BST operation.

There are four situations:

```text
1. Node not found
2. Node is a leaf
3. Node has one child
4. Node has two children
```

The public function:

```c
void delete_key(tree_t *ptr_tree, int key)
{
    ptr_tree->root_ =
        delete_key_recursive(ptr_tree->root_, key);
}
```

Recursive function:

```c
static node_t *delete_key_recursive(node_t *root, int key)
{
    if(root == NULL)
    {
        return NULL;
    }

    if(key < root->key_)
    {
        root->left_ = delete_key_recursive(root->left_, key);
    }
    else if(key > root->key_)
    {
        root->right_ = delete_key_recursive(root->right_, key);
    }
    else
    {
        ...
    }

    return root;
}
```

---

# 17. Delete Case 1 — Leaf Node

Example:

```text
      50
     /
   30
```

Delete `30`.

`30` has:

```text
left = NULL
right = NULL
```

Code:

```c
if(root->left_ == NULL && root->right_ == NULL)
{
    free(root);
    return NULL;
}
```

The node is freed.

The parent reconnects its pointer to `NULL`.

Before:

```text
50
 |
30
```

After:

```text
50
 |
NULL
```

---

# 18. Delete Case 2 — Only Right Child

Example:

```text
      50
     /
   30
     \
      40
```

Delete `30`.

Code:

```c
if(root->left_ == NULL)
{
    node_t *temp = root->right_;
    free(root);
    return temp;
}
```

The node is freed and its right child is returned.

Result:

```text
      50
     /
    40
```

The parent connects directly to `40`.

---

# 19. Delete Case 3 — Only Left Child

Example:

```text
      50
     /
   30
   /
 20
```

Delete `30`.

Code:

```c
if(root->right_ == NULL)
{
    node_t *temp = root->left_;
    free(root);
    return temp;
}
```

Result:

```text
      50
     /
    20
```

---

# 20. Delete Case 4 — Two Children

This is the most important deletion case.

Example:

```text
          50
        /    \
      30      70
             /  \
           60    80
```

Delete `50`.

The node has two children:

```text
left  = 30
right = 70
```

We find the inorder successor.

Right subtree:

```text
       70
      /  \
    60    80
```

Smallest value:

```text
60
```

Code:

```c
node_t *temp = inorder_successor(root->right_);
```

Then copy the successor's data:

```c
root->key_ = temp->key_;
```

The tree temporarily becomes:

```text
          60
        /    \
      30      70
             /  \
           60    80
```

There are now two `60`s.

So delete the original successor:

```c
root->right_ =
    delete_key_recursive(root->right_, temp->key_);
```

Final:

```text
          60
        /    \
      30      70
                \
                 80
```

### Two-child deletion algorithm

```text
Find inorder successor
        ↓
Copy successor data into current node
        ↓
Delete successor from right subtree
```

---

# 21. Why Does Recursive Delete Return `node_t *`?

This is a central idea.

Suppose:

```text
50
 |
30
```

Delete `30`.

The subtree rooted at `30` becomes:

```text
NULL
```

So the recursive function returns:

```text
NULL
```

Then:

```c
root->left_ =
    delete_key_recursive(root->left_, key);
```

changes the parent's pointer.

Similarly, if:

```text
50
 |
30
 |
20
```

and `30` is deleted, the recursive call returns `20`.

The parent reconnects:

```text
50
 |
20
```

### General pattern

```text
recursive operation
        ↓
returns new root of subtree
        ↓
parent reconnects its child pointer
```

This pattern is fundamental to recursive BST code.

---

# 22. Deletion Summary

```text
Search for node
      ↓
Not found?
      → return

Found?
      ↓
 ┌───────────────────────┐
 │                       │
Leaf                 One child
 │                       │
free                  return child
 │
return NULL
 │
 └───────────────┐
                 ↓
           Two children
                 ↓
      Find inorder successor
                 ↓
       Copy successor data
                 ↓
 Delete successor recursively
```

---

# 23. Array-Based BST

The U3 material also uses an array implementation of a BST.

Instead of dynamically allocating every node, the nodes are stored in an array.

Structure:

```c
#define MAXSIZE 200

struct node
{
    int key_;
    int left_;
    int right_;
};

typedef struct node node_t;

struct tree
{
    int root_;
    int count_;
    node_t elem_[MAXSIZE];
};

typedef struct tree tree_t;
```

Here:

```text
left_
right_
```

are **array indices**, not pointers.

---

# 24. Array BST Representation

Example:

```text
index    left    key    right

  0        1      50       2
  1        3      30       4
  2        5      70       6
  3       -1      20      -1
  4       -1      40      -1
  5       -1      60      -1
  6       -1      80      -1
```

This represents:

```text
          50
        /    \
      30      70
     /  \    /  \
   20   40  60   80
```

---

# 25. Why Is `-1` Used?

Valid array indices are:

```text
0, 1, 2, ...
```

Therefore `-1` can represent:

```text
NO NODE
```

So:

```text
left_ = -1
```

means no left child.

```text
right_ = -1
```

means no right child.

A leaf has:

```text
left_  = -1
right_ = -1
```

---

# 26. Array BST Initialization

```c
void init(tree_t *ptr_tree)
{
    ptr_tree->root_ = -1;
    ptr_tree->count_ = 0;
}
```

Meaning:

```text
root_ = -1
    → tree is empty

count_ = 0
    → no array positions are being used
```

---

# 27. Array BST Insertion

```c
void insert(tree_t *ptr_tree, int key)
{
    if(ptr_tree->count_ == MAXSIZE)
    {
        printf("tree full\n");
        exit(1);
    }

    int temp = ptr_tree->count_++;

    ptr_tree->elem_[temp].key_ = key;
    ptr_tree->elem_[temp].left_ = -1;
    ptr_tree->elem_[temp].right_ = -1;

    ptr_tree->root_ =
        insert_recursive(ptr_tree->elem_,
                         ptr_tree->root_,
                         temp);
}
```

The next free array position is selected using `count_`.

For example:

```text
count_ = 0
```

Insert `50`.

```text
temp = 0
```

So:

```text
elem_[0].key_ = 50
```

Then:

```text
root_ = 0
count_ = 1
```

---

# 28. Array Recursive Insertion

```c
static int insert_recursive(node_t elem[], int root, int temp)
{
    if(root == -1)
    {
        root = temp;
    }
    else if(elem[root].key_ > elem[temp].key_)
    {
        elem[root].left_ =
            insert_recursive(elem,
                             elem[root].left_,
                             temp);
    }
    else
    {
        elem[root].right_ =
            insert_recursive(elem,
                             elem[root].right_,
                             temp);
    }

    return root;
}
```

The algorithm is almost identical to pointer-based BST insertion.

### Pointer BST

```c
root->left_
```

### Array BST

```c
elem[root].left_
```

### Pointer BST empty

```text
NULL
```

### Array BST empty

```text
-1
```

---

# 29. Array BST Traversal

Inorder:

```c
static void disp_inorder_recursive(node_t elem[], int temp)
{
    if(temp != -1)
    {
        disp_inorder_recursive(elem, elem[temp].left_);
        printf("%d ", elem[temp].key_);
        disp_inorder_recursive(elem, elem[temp].right_);
    }
}
```

The algorithm is still:

```text
LEFT → ROOT → RIGHT
```

The difference is that `temp` is an array index.

---

# 30. Array BST Count Nodes

```c
int number_of_nodes_recursive(node_t elem[], int temp)
{
    if(temp == -1)
    {
        return 0;
    }

    return 1
         + number_of_nodes_recursive(elem, elem[temp].left_)
         + number_of_nodes_recursive(elem, elem[temp].right_);
}
```

Same recursive logic as the pointer implementation.

Only the representation changes.

---

# 31. Array BST Count Leaves

```c
int number_of_leaves_recursive(node_t elem[], int temp)
{
    if(temp == -1)
    {
        return 0;
    }

    if(elem[temp].left_ == -1 &&
       elem[temp].right_ == -1)
    {
        return 1;
    }

    return number_of_leaves_recursive(
               elem,
               elem[temp].left_)
         + number_of_leaves_recursive(
               elem,
               elem[temp].right_);
}
```

Leaf condition:

```text
left == -1
AND
right == -1
```

---

# 32. Array BST Comparison

```c
int are_same_recursive(
    node_t elem1[],
    int root1,
    node_t elem2[],
    int root2)
{
    if(root1 == -1 && root2 == -1)
    {
        return 1;
    }

    if(root1 == -1 || root2 == -1)
    {
        return 0;
    }

    return elem1[root1].key_ == elem2[root2].key_
        && are_same_recursive(
               elem1,
               elem1[root1].left_,
               elem2,
               elem2[root2].left_)
        && are_same_recursive(
               elem1,
               elem1[root1].right_,
               elem2,
               elem2[root2].right_);
}
```

Same recursive idea as the pointer-based version.

---

# 33. Array BST Copy

Because the whole tree is stored in an array, copying can be done by copying the tree metadata and array elements.

```c
void copy(tree_t *ptr_src, tree_t *ptr_dst)
{
    ptr_dst->root_ = ptr_src->root_;
    ptr_dst->count_ = ptr_src->count_;

    for(int i = 0; i < MAXSIZE; ++i)
    {
        ptr_dst->elem_[i] = ptr_src->elem_[i];
    }
}
```

Difference:

```text
Dynamic BST
→ recursively allocate new nodes

Array BST
→ copy the array
```

---

# 34. Threaded BST

A threaded BST uses otherwise-unused child pointers as **threads**.

Normal BST:

```c
struct node
{
    int key_;
    struct node *left_;
    struct node *right_;
};
```

Many child pointers can be `NULL`.

A threaded tree uses these unused pointers to store links to inorder predecessor/successor nodes.

---

# 35. Threaded Node Structure

```c
struct node
{
    int key_;
    struct node *left_;
    struct node *right_;
    int lthread_;
    int rthread_;
};

typedef struct node node_t;
```

Meaning:

```text
lthread_ == 0
    → left_ is a real child

lthread_ == 1
    → left_ is a thread
```

Similarly:

```text
rthread_ == 0
    → right_ is a real child

rthread_ == 1
    → right_ is a thread
```

---

# 36. Creating a Threaded Node

```c
static node_t *create_node(int key)
{
    node_t *temp = (node_t *)malloc(sizeof(node_t));

    if(temp == NULL)
    {
        printf("no more memory\n");
        exit(1);
    }

    temp->key_ = key;
    temp->left_ = temp->right_ = NULL;
    temp->lthread_ = temp->rthread_ = 1;

    return temp;
}
```

Initially both pointers are treated as threads.

---

# 37. Threaded BST Insertion

The main difference is that while traversing we must check whether a pointer is a real child or a thread.

```c
while(curr != NULL)
{
    parent = curr;

    if(key < curr->key_)
    {
        if(curr->lthread_ == 0)
            curr = curr->left_;
        else
            break;
    }
    else
    {
        if(curr->rthread_ == 0)
            curr = curr->right_;
        else
            break;
    }
}
```

Important:

```text
lthread_ == 0
→ left pointer is a real subtree

lthread_ == 1
→ left pointer is a thread, so stop

rthread_ == 0
→ right pointer is a real subtree

rthread_ == 1
→ right pointer is a thread, so stop
```

---

# 38. Threaded BST — Inserting a Left Child

```c
if(key < parent->key_)
{
    temp->left_ = parent->left_;
    temp->right_ = parent;

    parent->left_ = temp;
    parent->lthread_ = 0;
}
```

The new node's right pointer becomes a thread to the parent.

The parent becomes the new node's inorder successor.

---

# 39. Threaded BST — Inserting a Right Child

```c
else
{
    temp->right_ = parent->right_;
    temp->left_ = parent;

    parent->right_ = temp;
    parent->rthread_ = 0;
}
```

The new node's left pointer becomes a thread to the parent.

The parent becomes the new node's inorder predecessor.

---

# 40. Threaded BST Inorder Traversal

First find the leftmost node:

```c
static node_t *leftmost(node_t *temp)
{
    if(temp == NULL)
        return NULL;

    while(temp->lthread_ == 0)
        temp = temp->left_;

    return temp;
}
```

Then traverse:

```c
static void inorder(node_t *root)
{
    node_t *temp = leftmost(root);

    while(temp != NULL)
    {
        printf("%d ", temp->key_);

        if(temp->rthread_ == 1)
            temp = temp->right_;
        else
            temp = leftmost(temp->right_);
    }
}
```

### Main advantage

A threaded BST can perform inorder traversal **without recursion** because the threads tell us where to go next.

---

# 41. Expression Tree

The U3 material also contains expression trees.

An expression tree represents an expression using a binary tree.

Example:

```text
2 + 3
```

Tree:

```text
      +
     / \
    2   3
```

For postfix:

```text
23+
```

the expression tree can be constructed using a stack of tree-node pointers.

---

# 42. Expression Tree Construction

For an operand:

```text
1. Create a node.
2. Push it onto the stack.
```

For an operator:

```text
1. Pop the right operand/tree.
2. Pop the left operand/tree.
3. Create an operator node.
4. Attach left and right.
5. Push the new tree back.
```

The order matters.

For:

```text
23-
```

the tree must be:

```text
    -
   / \
  2   3
```

not:

```text
    -
   / \
  3   2
```

because subtraction is not commutative.

The resulting tree can then be recursively evaluated.

---

# 43. Core BST Patterns to Memorize

## BST insertion

```c
if(root == NULL)
{
    root = temp;
}
else if(root->key_ > temp->key_)
{
    root->left_ =
        insert_recursive(root->left_, temp);
}
else
{
    root->right_ =
        insert_recursive(root->right_, temp);
}

return root;
```

---

## Inorder

```c
if(root)
{
    inorder(root->left_);
    printf("%d ", root->key_);
    inorder(root->right_);
}
```

Remember:

```text
LEFT → ROOT → RIGHT
```

---

## Search

```c
if(root == NULL)
    return NULL;

if(key == root->key_)
    return root;

if(key < root->key_)
    return search_recursive(root->left_, key);

return search_recursive(root->right_, key);
```

---

## Count nodes

```c
if(root == NULL)
    return 0;

return 1
     + count(root->left_)
     + count(root->right_);
```

---

## Count leaves

```c
if(root == NULL)
    return 0;

if(root->left_ == NULL &&
   root->right_ == NULL)
    return 1;

return leaves(root->left_)
     + leaves(root->right_);
```

---

# 44. BST Deletion — Exam Memory Version

```text
1. Leaf
   free(root)
   return NULL

2. Only right child
   save right child
   free(root)
   return right child

3. Only left child
   save left child
   free(root)
   return left child

4. Two children
   find inorder successor
   copy successor data
   delete successor from right subtree
```

---

# 45. The Central Recursive BST Pattern

Almost every recursive BST function follows:

```text
             TREE
              |
             root
              |
              ↓
       recursive function
              |
       ┌──────┴──────┐
       ↓             ↓
 left subtree    right subtree
```

The tree-level function handles the tree:

```c
void insert(...)
void delete_key(...)
node_t *search(...)
```

The recursive function handles a subtree:

```c
insert_recursive(...)
delete_key_recursive(...)
search_recursive(...)
```

The especially important pattern is:

```c
root->left_ =
    recursive(root->left_, ...);
```

or:

```c
root->right_ =
    recursive(root->right_, ...);
```

The recursive call returns the **new root of the subtree**, and the parent reconnects its pointer.

---

# 46. Pointer BST vs Array BST vs Threaded BST

| Feature | Normal BST | Array BST | Threaded BST |
|---|---|---|---|
| Node storage | `malloc` | Array | `malloc` |
| Child representation | Pointers | Indices | Pointers |
| Empty child | `NULL` | `-1` | Thread/NULL |
| Insertion | Recursive | Recursive | Special handling |
| Inorder | Recursive | Recursive | Non-recursive |
| Search | Recursive | Same basic logic | Possible |
| Delete | Covered | Not implemented in the source version | Not covered |
| Main advantage | Simple | No dynamic node allocation | Inorder without recursion |

The array-based material shown in the source is simplified and does not implement deletion; a complete implementation would need a mechanism for managing free array positions.

---

# 47. Recommended Study Order

Study the material in this order:

```text
1. Binary tree basics
        ↓
2. BST property
        ↓
3. Node + tree structures
        ↓
4. insert_recursive()
        ↓
5. inorder / preorder / postorder
        ↓
6. count nodes
        ↓
7. count leaves
        ↓
8. search
        ↓
9. inorder successor
        ↓
10. DELETE
        ↓
11. copy
        ↓
12. compare
        ↓
13. array implementation
        ↓
14. threaded BST
        ↓
15. expression tree
```

The highest-priority functions to be able to write without looking are:

```text
insert
inorder
search
inorder_successor
delete
```

# 48. Final Mental Model

Think of a BST as:

```text
             ROOT
              |
       ┌──────┴──────┐
       ↓             ↓
    smaller       greater/equal
       |             |
      LEFT          RIGHT
```

For recursive code:

```text
Current node
    ↓
Decide LEFT or RIGHT
    ↓
Recursive call
    ↓
Recursive call returns subtree root
    ↓
Reconnect subtree
```

For deletion:

```text
Leaf
→ remove it

One child
→ replace it with its child

Two children
→ find smallest node in right subtree
→ copy its data
→ delete that successor
```

For traversal:

```text
Inorder   = LEFT → ROOT → RIGHT
Preorder  = ROOT → LEFT → RIGHT
Postorder = LEFT → RIGHT → ROOT
```
