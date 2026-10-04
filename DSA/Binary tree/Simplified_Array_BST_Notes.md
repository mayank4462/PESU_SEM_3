# Simplified Array-Based BST

## 1. Basic Idea

In a normal BST, `left` and `right` are pointers.

In an array BST, `left` and `right` store **array indices**.

Example:

```text
index     data     left     right
  0        50       1        2
  1        30      -1       -1
  2        70      -1       -1
```

This represents:

```text
       50
      /  \
    30    70
```

`-1` means **no child**.

---

# 2. Simplest Array BST Code

```c
#include <stdio.h>

#define MAX 10

struct node
{
    int data;
    int left;
    int right;
};

struct node tree[MAX];

int root = -1;
int count = 0;

void insert(int data)
{
    int new_node = count++;

    tree[new_node].data = data;
    tree[new_node].left = -1;
    tree[new_node].right = -1;

    if(root == -1)
    {
        root = new_node;
        return;
    }

    int current = root;

    while(1)
    {
        if(data < tree[current].data)
        {
            if(tree[current].left == -1)
            {
                tree[current].left = new_node;
                return;
            }

            current = tree[current].left;
        }
        else
        {
            if(tree[current].right == -1)
            {
                tree[current].right = new_node;
                return;
            }

            current = tree[current].right;
        }
    }
}

void inorder(int current)
{
    if(current == -1)
        return;

    inorder(tree[current].left);

    printf("%d ", tree[current].data);

    inorder(tree[current].right);
}

int main()
{
    insert(50);
    insert(30);
    insert(70);
    insert(20);
    insert(40);

    inorder(root);

    return 0;
}
```

Output:

```text
20 30 40 50 70
```

---

# 3. Node Structure

```c
struct node
{
    int data;
    int left;
    int right;
};
```

There are three things:

```text
data  → value stored in the node
left  → index of left child
right → index of right child
```

Unlike pointer BST:

```c
struct node *left;
struct node *right;
```

we simply use:

```c
int left;
int right;
```

---

# 4. The Array

```c
struct node tree[MAX];
```

This creates an array containing `MAX` nodes.

For example:

```text
tree[0]
tree[1]
tree[2]
tree[3]
...
```

Each position can hold one BST node.

---

# 5. Root

```c
int root = -1;
```

`root` stores the **index** of the root node.

Initially:

```text
root = -1
```

means:

```text
Tree is empty
```

After inserting `50`:

```text
root = 0
```

So:

```text
root
 ↓
 0
 ↓
tree[0]
 ↓
50
```

---

# 6. Count

```c
int count = 0;
```

`count` tells us where to place the next new node.

Example:

```text
Insert 50 → index 0
Insert 30 → index 1
Insert 70 → index 2
Insert 20 → index 3
Insert 40 → index 4
```

---

# 7. Creating a New Node

Inside `insert()`:

```c
int new_node = count++;

tree[new_node].data = data;
tree[new_node].left = -1;
tree[new_node].right = -1;
```

Suppose:

```c
insert(50);
```

Then:

```text
new_node = 0
```

and:

```text
tree[0].data = 50
tree[0].left = -1
tree[0].right = -1
```

---

# 8. First Node

```c
if(root == -1)
{
    root = new_node;
    return;
}
```

If the tree is empty, the new node becomes the root.

For the first insertion:

```text
root = 0
```

Tree:

```text
50
```

---

# 9. Finding the Correct Position

```c
int current = root;
```

`current` tells us which node we are currently looking at.

Then:

```c
while(1)
{
    if(data < tree[current].data)
```

If the new data is smaller:

```text
go LEFT
```

Otherwise:

```text
go RIGHT
```

This is exactly the normal BST rule.

---

# 10. Going Left

```c
if(data < tree[current].data)
{
    if(tree[current].left == -1)
    {
        tree[current].left = new_node;
        return;
    }

    current = tree[current].left;
}
```

Suppose:

```text
current node = 50
new data = 30
```

Since:

```text
30 < 50
```

go left.

If:

```text
tree[current].left == -1
```

there is no left child, so attach the new node there.

Otherwise:

```c
current = tree[current].left;
```

move to the existing left child and continue.

---

# 11. Going Right

```c
else
{
    if(tree[current].right == -1)
    {
        tree[current].right = new_node;
        return;
    }

    current = tree[current].right;
}
```

If the new data is greater than or equal to the current data:

```text
go RIGHT
```

If there is no right child, insert there.

Otherwise move to the right child and continue.

---

# 12. Example

Insert:

```text
50
30
70
20
40
```

Final array:

```text
index     data     left     right
-----------------------------------
  0        50       1        2
  1        30       3        4
  2        70      -1       -1
  3        20      -1       -1
  4        40      -1       -1
```

Tree:

```text
          50
         /  \
       30    70
      /  \
    20    40
```

---

# 13. Inorder Traversal

```c
void inorder(int current)
{
    if(current == -1)
        return;

    inorder(tree[current].left);

    printf("%d ", tree[current].data);

    inorder(tree[current].right);
}
```

The order is:

```text
LEFT → ROOT → RIGHT
```

For the example:

```text
20 30 40 50 70
```

Because this is a BST, inorder gives sorted order.

---

# 14. Pointer BST vs Array BST

### Pointer BST

```c
root->left
root->right
```

These are pointers.

### Array BST

```c
tree[current].left
tree[current].right
```

These are indices.

### Empty child

```text
Pointer BST → NULL
Array BST   → -1
```

---

# 15. The Main Concept

The BST logic does **not** change.

Only the storage changes.

```text
Normal BST:

left/right → memory addresses


Array BST:

left/right → array indices
```

For example:

```text
tree[0].left = 1
```

means:

```text
Node at index 0 has the node at index 1 as its left child.
```

---

# 16. What to Memorize for the Exam

### Structure

```c
struct node
{
    int data;
    int left;
    int right;
};
```

### Root

```c
int root = -1;
```

### Empty child

```text
-1
```

### BST rule

```text
smaller → LEFT
greater/equal → RIGHT
```

### Inorder

```text
LEFT → ROOT → RIGHT
```

### Most important difference

```text
Pointer BST:
left/right are pointers

Array BST:
left/right are indices
```

---

# 17. One-Line Mental Model

> **Array BST = normal BST logic + array indices instead of pointers.**
