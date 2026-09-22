# 🌳 Trees — Complete MCQ Revision Notes

> [!abstract] Exam Focus  
> This note contains the definitions, formulas, traversals, BST, AVL, Heap, complexity, C-code concepts, and common MCQ traps needed for Tree questions.

---

# 1. Tree Basics

## Tree

A **tree** is a non-linear, hierarchical data structure.

Properties:

- Connected
    
- Acyclic
    
- Contains nodes connected by edges
    

For a tree with `n` nodes:

E=n−1\boxed{E=n-1}

### Example

```text
        A
       / \
      B   C
     / \
    D   E
```

- Nodes = 5
    
- Edges = 4
    

---

## Root

The topmost node.

```text
        A ← Root
       / \
      B   C
```

- Root has no parent.
    

---

## Parent

The node directly above another node.

```text
A
|
B
```

`A` is the parent of `B`.

---

## Child

The node directly below another node.

`B` is a child of `A`.

---

## Siblings

Nodes having the same parent.

```text
      A
     / \
    B   C
```

`B` and `C` are siblings.

---

## Leaf / External Node

A node with **0 children**.

```text
      A
     / \
    B   C
```

`B` and `C` are leaves.

---

## Internal Node

A node having at least one child.

In the above tree:

- `A` → internal
    
- `B`, `C` → leaves
    

---

## Degree of a Node

Number of children of that node.

```text
        A
       / \
      B   C
     /
    D
```

- Degree(A) = 2
    
- Degree(B) = 1
    
- Degree(C) = 0
    
- Degree(D) = 0
    

### Degree of a Tree

Maximum degree of any node.

For a binary tree:

Maximum Degree=2\boxed{Maximum\ Degree=2}

---

## Path

A sequence of connected nodes.

```text
A → B → D
```

is a path.

### Path Length

**Number of edges**, NOT number of nodes.

```text
A → B → D
```

- Nodes = 3
    
- Edges = 2
    
- Path length = 2
    

> [!warning] MCQ Trap  
> **Path length counts edges.**

---

## Ancestor

A node above another node.

```text
A
|
B
|
D
```

`A` and `B` are ancestors of `D`.

---

## Descendant

A node below another node.

`D` is a descendant of `A` and `B`.

---

## Subtree

A node together with all of its descendants.

```text
        A
       / \
      B   C
     / \
    D   E
```

Subtree rooted at `B`:

```text
      B
     / \
    D   E
```

---

# 2. Depth and Height

## Depth

Number of edges from the root to the node.

```text
        A       depth = 0
       /
      B         depth = 1
     /
    C           depth = 2
```

Depth(root)=0\boxed{Depth(root)=0}

---

## Height

Number of edges on the longest path from a node to a leaf.

```text
        A
       /
      B
     /
    C
```

- Height(C) = 0
    
- Height(B) = 1
    
- Height(A) = 2
    

Therefore:

Height(tree)=Height(root)\boxed{Height(tree)=Height(root)}

> [!warning] Height Convention  
> Some textbooks define height using number of **nodes** instead of edges. Check the convention used by your course.

---

# 3. Binary Tree

A **binary tree** is a tree where every node has **at most two children**.

Children are:

- Left child
    
- Right child
    

A node can have:

- 0 children
    
- 1 child
    
- 2 children
    

It cannot have 3 children.

---

# 4. Binary Tree Formulas

Assume root is at **level 0**.

## Maximum Nodes at Level `L`

2L\boxed{2^L}

|Level|Maximum Nodes|
|--:|--:|
|0|1|
|1|2|
|2|4|
|3|8|
|4|16|

---

## Maximum Nodes in Binary Tree of Height `h`

2h+1−1\boxed{2^{h+1}-1}

Example:

For `h = 3`:

24−1=152^4-1=15

---

## Minimum Nodes for Height `h`

A completely skewed tree:

```text
1
 \
  2
   \
    3
     \
      4
```

has:

h+1\boxed{h+1}

nodes.

---

## Maximum Height with `n` Nodes

A completely skewed tree gives maximum height:

hmax=n−1\boxed{h_{max}=n-1}

---

## Minimum Height with `n` Nodes

hmin=⌈log⁡2(n+1)⌉−1\boxed{h_{min}=\lceil\log_2(n+1)\rceil-1}

---

# 5. Types of Binary Trees

## Full Binary Tree

Every node has either:

```text
0 children
OR
2 children
```

No node has exactly one child.

### Important Formulas

Let:

- `I` = internal nodes
    
- `L` = leaf nodes
    

Then:

L=I+1\boxed{L=I+1}

Total nodes:

n=2I+1\boxed{n=2I+1}

Also:

n=2L−1\boxed{n=2L-1}

Therefore:

> [!important] MCQ  
> A full binary tree always has an **odd number of nodes**.

---

## Complete Binary Tree

Rules:

1. Every level except possibly the last is completely filled.
    
2. The last level is filled **from left to right**.
    

Example:

```text
        1
       / \
      2   3
     / \  /
    4  5 6
```

### Important

```text
Complete ≠ Full
```

A complete binary tree may contain a node with only one child.

---

## Perfect Binary Tree

A binary tree where:

1. Every internal node has exactly 2 children.
    
2. All leaves are at the same level.
    

Example:

```text
        1
       / \
      2   3
     / \ / \
    4  5 6  7
```

For height `h`:

Nodes=2h+1−1\boxed{Nodes=2^{h+1}-1} Leaves=2h\boxed{Leaves=2^h} Internal=2h−1\boxed{Internal=2^h-1}

A perfect binary tree is:

Full+Complete\boxed{Full+Complete}

---

## Skewed / Degenerate Tree

Every node has at most one child.

```text
1
 \
  2
   \
    3
     \
      4
```

Behaves similarly to a linked list.

For `n` nodes:

Height=n−1\boxed{Height=n-1}

---

# 6. Full vs Complete vs Perfect

|Type|Definition|
|---|---|
|**Full**|Every node has 0 or 2 children|
|**Complete**|Last level filled from left to right|
|**Perfect**|Full + all leaves at same level|

> [!important] Remember  
> **Full → children condition**
> 
> **Complete → level-filling condition**
> 
> **Perfect → both**

---

# 7. Tree Traversals

There are four important traversals:

1. Preorder
    
2. Inorder
    
3. Postorder
    
4. Level-order
    

---

## Preorder

Order:

Root→Left→Right\boxed{Root\rightarrow Left\rightarrow Right}

Example:

```text
        A
       / \
      B   C
     / \
    D   E
```

Preorder:

```text
A B D E C
```

> [!tip]  
> **Pre = Root first**

---

## Inorder

Order:

Left→Root→Right\boxed{Left\rightarrow Root\rightarrow Right}

Same tree:

```text
D B E A C
```

> [!important] BST  
> Inorder traversal of a standard BST produces **sorted order**.

---

## Postorder

Order:

Left→Right→Root\boxed{Left\rightarrow Right\rightarrow Root}

Same tree:

```text
D E B C A
```

> [!tip]  
> **Post = Root last**

Useful for deleting/freeing a tree because children are processed before the parent.

---

## Level Order

Visits nodes level by level.

```text
        A
       / \
      B   C
     / \
    D   E
```

Level order:

```text
A B C D E
```

Uses:

Queue\boxed{Queue}

Level order is a form of **BFS**.

---

# 8. Traversal Cheat Sheet

|Traversal|Order|
|---|---|
|**Preorder**|Root → Left → Right|
|**Inorder**|Left → Root → Right|
|**Postorder**|Left → Right → Root|
|**Level-order**|Level by level|

### Memory Trick

```text
PRE  → Root first
IN   → Root middle
POST → Root last
```

---

# 9. Binary Search Tree — BST

A BST is a binary tree satisfying:

Left<Root<Right\boxed{Left<Root<Right}

for every node.

Example:

```text
        40
       /  \
     20    60
    / \    / \
   10 30  50 70
```

Properties:

- All values in left subtree are smaller.
    
- All values in right subtree are larger.
    

---

# 10. BST Search

To search for a value:

```text
data < root
    ↓
go LEFT

data > root
    ↓
go RIGHT

data == root
    ↓
FOUND
```

### Complexity

General:

O(h)\boxed{O(h)}

where `h` = tree height.

### Best Case

Data is at root:

O(1)\boxed{O(1)}

### Balanced BST

O(log⁡n)\boxed{O(\log n)}

### Worst Case

Skewed BST:

O(n)\boxed{O(n)}

> [!danger] Major MCQ Trap  
> **BST search is NOT always O(log n).**
> 
> Correct general complexity:
> 
> O(h)O(h)

---

# 11. BST Insertion

Insertion follows one root-to-leaf path.

Complexity:

O(h)\boxed{O(h)}

Balanced:

O(log⁡n)O(\log n)

Worst case:

O(n)O(n)

---

## Sorted Insertion

If values are inserted in ascending order:

```text
10 20 30 40 50
```

BST becomes:

```text
10
  \
   20
     \
      30
        \
         40
           \
            50
```

This is a **right-skewed BST**.

Height:

n−1\boxed{n-1}

Search:

O(n)\boxed{O(n)}

---

# 12. BST Deletion

There are three cases.

## Case 1 — Leaf

```text
    40
   /
  20
```

Delete `20`.

Simply remove it.

---

## Case 2 — One Child

```text
    40
   /
  20
    \
     30
```

Delete `20`.

`30` replaces `20`.

---

## Case 3 — Two Children

Example:

```text
        40
       /  \
     20    60
          /  \
         50   70
```

To delete `60`, replace it with either:

### Inorder Successor

Smallest value in right subtree.

```text
70
```

OR

### Inorder Predecessor

Largest value in left subtree.

```text
50
```

Both are valid.

---

# 13. Inorder Successor

The **smallest value greater than** the current node.

Example:

```text
        40
       /  \
     20    60
          /  \
         50   70
```

Successor of `40`:

50\boxed{50}

Successor of `60`:

70\boxed{70}

---

# 14. Inorder Predecessor

The **largest value smaller than** the current node.

Predecessor of `40`:

20\boxed{20}

Predecessor of `60`:

50\boxed{50}

---

# 15. BST Critical Property

For a standard BST:

Inorder=Sorted Order\boxed{Inorder=Sorted\ Order}

Example:

```text
10 20 30 40 50 60 70
```

If the inorder sequence is not sorted, it cannot represent a standard BST.

---

# 16. AVL Tree

AVL = **self-balancing Binary Search Tree**.

Balance factor:

BF=Height(Left)−Height(Right)\boxed{BF=Height(Left)-Height(Right)}

Allowed values:

−1, 0, +1\boxed{-1,\ 0,\ +1}

If:

```text
BF = +2
```

or

```text
BF = -2
```

the node is unbalanced.

---

# 17. AVL Rotations

|Case|Rotation|
|---|---|
|**LL**|Right rotation|
|**RR**|Left rotation|
|**LR**|Left → Right|
|**RL**|Right → Left|

### Memory Trick

```text
LL → Right
RR → Left

LR → Left + Right
RL → Right + Left
```

AVL operations maintain:

O(log⁡n)\boxed{O(\log n)}

for search, insertion and deletion.

---

# 18. Heap

A binary heap is a:

Complete Binary Tree\boxed{Complete\ Binary\ Tree}

---

## Max Heap

Parent is greater than or equal to children.

Parent≥Children\boxed{Parent\ge Children}

Root contains maximum element.

---

## Min Heap

Parent is less than or equal to children.

Parent≤Children\boxed{Parent\le Children}

Root contains minimum element.

---

# 19. Heap vs BST

|BST|Heap|
|---|---|
|Left < Root < Right|Parent-child ordering|
|Need not be complete|Must be complete|
|Inorder is sorted|Inorder not necessarily sorted|
|Balanced search can be O(log n)|Arbitrary search is O(n)|
|Used for searching|Used for priority queues|

> [!danger] MCQ Trap  
> A heap is **not** a BST.

---

# 20. Heap Array Representation

For **0-based indexing**:

### Left child

2i+1\boxed{2i+1}

### Right child

2i+2\boxed{2i+2}

### Parent

⌊i−12⌋\boxed{\left\lfloor\frac{i-1}{2}\right\rfloor}

---

# 21. Heap Complexity

|Operation|Complexity|
|---|--:|
|Peek root|`O(1)`|
|Insert|`O(log n)`|
|Delete root|`O(log n)`|
|Build heap|`O(n)`|

> [!important] Common MCQ  
> **Build Heap = O(n)**, not `O(n log n)`.

---

# 22. Tree Representation

A binary tree can be represented using linked nodes.

Typical node:

```c
struct node
{
    int data;
    struct node *left;
    struct node *right;
};
```

Each node stores:

- Data
    
- Pointer to left child
    
- Pointer to right child
    

---

# 23. Recursive Tree Complexity

For a tree with `n` nodes:

### Traversal Time

Every node is visited once:

O(n)\boxed{O(n)}

### Recursive Space

Depends on height:

O(h)\boxed{O(h)}

Balanced tree:

O(log⁡n)O(\log n)

Skewed tree:

O(n)O(n)

---

# 24. YOUR BST C CODE — IMPORTANT MCQs

Your insertion logic is essentially:

```text
if data < root data
    insert LEFT
else
    insert RIGHT
```

Therefore:

data<root→LEFT\boxed{data<root\rightarrow LEFT} data≥root→RIGHT\boxed{data\ge root\rightarrow RIGHT}

### Duplicate values

Because equality goes to the `else` case:

Duplicates go RIGHT\boxed{Duplicates\ go\ RIGHT}

---

# 25. `disp_recursive()`

Logic:

```text
left
root
right
```

Therefore:

INORDER\boxed{INORDER}

For insertion sequence:

```text
40 20 60 10 70 30 50
```

BST:

```text
        40
       /  \
     20    60
    / \    / \
   10 30  50 70
```

Output:

```text
10 20 30 40 50 60 70
```

---

# 26. `number_of_nodes_recursive()`

Logic:

```text
1 + nodes in left subtree + nodes in right subtree
```

If:

```text
root == NULL
```

returns:

```text
0
```

Therefore:

NULL tree has 0 nodes\boxed{NULL\ tree\ has\ 0\ nodes}

---

# 27. `number_of_leaves_recursive()`

Leaf condition:

```text
left == NULL && right == NULL
```

Leaf returns:

```text
1
```

NULL returns:

```text
0
```

For:

```text
        40
       /  \
     20    60
    / \    / \
   10 30  50 70
```

Leaves:

```text
10, 30, 50, 70
```

Number of leaves:

4\boxed{4}

---

# 28. `deinit_recursive()`

Logic:

```text
deinit(left)
deinit(right)
free(root)
```

Order:

Left→Right→Root\boxed{Left\rightarrow Right\rightarrow Root}

Therefore:

POSTORDER\boxed{POSTORDER}

### Why?

Children must be freed before the parent.

After `deinit()`:

```text
root = NULL
```

---

# 29. `malloc()` in BST

A new node is dynamically allocated:

```c
node_t *temp = (node_t*)malloc(sizeof(node_t));
```

Then:

```text
temp->data = data
temp->left = NULL
temp->right = NULL
```

The newly created node initially has no children.

---

# 30. Why Recursive Insert Returns a Node Pointer

Conceptually:

```c
root->left = insert_recursive(root->left, temp);
```

The recursive function:

1. Goes into the required subtree.
    
2. Inserts the new node.
    
3. Returns the updated subtree root.
    
4. Parent reconnects that subtree using the returned pointer.
    

This is why recursive insertion returns a node pointer.

---

# 31. First BST Insertion

Code conceptually does:

```c
tree.root = insert_recursive(tree.root, temp);
```

Initially:

```text
root = NULL
```

The new node becomes the root.

Therefore:

> [!important]  
> **The first inserted value becomes the BST root.**

---

# 32. Empty Tree

If:

```text
root = NULL
```

then:

```text
Number of nodes = 0
Number of leaves = 0
```

Traversal visits nothing.

---

# 33. Complexity Master Table

|Structure / Operation|Complexity|
|---|--:|
|Tree traversal|`O(n)`|
|BST search|`O(h)`|
|Balanced BST search|`O(log n)`|
|Worst BST search|`O(n)`|
|BST insertion|`O(h)`|
|BST deletion|`O(h)`|
|AVL search|`O(log n)`|
|AVL insertion|`O(log n)`|
|AVL deletion|`O(log n)`|
|Heap insert|`O(log n)`|
|Heap delete root|`O(log n)`|
|Heap peek|`O(1)`|
|Build heap|`O(n)`|

---

# 34. 🔥 HIGH-YIELD FORMULAS

## Tree

E=n−1\boxed{E=n-1}

## Maximum nodes at level `L`

2L\boxed{2^L}

## Maximum nodes at height `h`

2h+1−1\boxed{2^{h+1}-1}

## Minimum nodes for height `h`

h+1\boxed{h+1}

## Maximum height with `n` nodes

n−1\boxed{n-1}

## Full Binary Tree

L=I+1\boxed{L=I+1} n=2I+1\boxed{n=2I+1} n=2L−1\boxed{n=2L-1}

## Perfect Binary Tree

L=2h\boxed{L=2^h} n=2h+1−1\boxed{n=2^{h+1}-1}

## AVL

BF=HL−HR\boxed{BF=H_L-H_R} ∣BF∣≤1\boxed{|BF|\le1}

---

# 35. ⚠️ MCQ TRAPS

> [!danger] Trap 1  
> **BST ≠ Balanced BST**
> 
> A normal BST can become skewed.

---

> [!danger] Trap 2  
> **BST search is not always O(log n)**
> 
> General:
> 
> O(h)O(h)
> 
> Worst:
> 
> O(n)O(n)

---

> [!danger] Trap 3  
> **Full ≠ Complete**
> 
> Full → 0 or 2 children.
> 
> Complete → last level left-to-right.

---

> [!danger] Trap 4  
> **Complete ≠ Perfect**
> 
> Perfect requires all leaves at the same level.

---

> [!danger] Trap 5  
> **Path length = edges**
> 
> Not number of nodes.

---

> [!danger] Trap 6  
> **Heap ≠ BST**
> 
> Heap only guarantees parent-child ordering.

---

> [!danger] Trap 7  
> **Inorder is sorted only for BST**
> 
> An arbitrary binary tree's inorder traversal is not necessarily sorted.

---

> [!danger] Trap 8  
> **Build Heap = O(n)**
> 
> Not O(n log n).

---

> [!danger] Trap 9  
> **Level order uses Queue**
> 
> It is BFS.

---

> [!danger] Trap 10  
> **Postorder is useful for deleting a tree**
> 
> Children are processed before parent.

---

> [!danger] Trap 11 — YOUR CODE  
> Duplicate values go **RIGHT**.

```text
data < root → LEFT
data >= root → RIGHT
```

---

> [!danger] Trap 12  
> Empty tree:

```text
root = NULL
```

Therefore:

```text
nodes = 0
leaves = 0
```

---

# 36. 🔢 Numerical MCQs

### Q1. Tree has 50 nodes. Number of edges?

50−1=4950-1=\boxed{49}

### Q2. Maximum nodes at level 6?

26=642^6=\boxed{64}

### Q3. Maximum nodes in binary tree of height 5?

26−1=632^6-1=\boxed{63}

### Q4. Maximum height with 25 nodes?

25−1=2425-1=\boxed{24}

### Q5. Perfect binary tree of height 4?

Nodes:

25−1=312^5-1=\boxed{31}

Leaves:

24=162^4=\boxed{16}

### Q6. Full binary tree with 15 internal nodes?

Leaves:

15+1=1615+1=\boxed{16}

Total nodes:

2(15)+1=312(15)+1=\boxed{31}

### Q7. Full binary tree with 20 nodes?

Impossible\boxed{Impossible}

Full binary tree must have odd number of nodes.

### Q8. Ascending insertion of 10 values into BST?

Height:

9\boxed{9}

Worst-case search:

O(n)\boxed{O(n)}

### Q9. AVL valid balance factors?

−1,0,+1\boxed{-1,0,+1}

### Q10. Heap insertion?

O(log⁡n)\boxed{O(\log n)}

---

# 37. 🧠 FINAL 5-MINUTE REVISION

```text
TREE
n nodes → n-1 edges

BINARY TREE
≤ 2 children

LEVEL L
max nodes = 2^L

HEIGHT h
max nodes = 2^(h+1)-1

MAX HEIGHT
n-1

FULL
0 OR 2 children

COMPLETE
last level LEFT → RIGHT

PERFECT
FULL + all leaves same level

PREORDER
Root Left Right

INORDER
Left Root Right

POSTORDER
Left Right Root

LEVEL ORDER
BFS + Queue

BST
Left < Root < Right

BST INORDER
Sorted

BST Search
O(h)

Balanced BST
O(log n)

Skewed BST
O(n)

BST DELETE
0 child → remove
1 child → child replaces node
2 children → successor/predecessor

SUCCESSOR
Smallest greater

PREDECESSOR
Largest smaller

AVL
|BF| ≤ 1

LL → Right
RR → Left
LR → Left + Right
RL → Right + Left

HEAP
Complete binary tree

MAX HEAP
Parent ≥ children

MIN HEAP
Parent ≤ children

Heap Insert
O(log n)

Heap Delete
O(log n)

Heap Peek
O(1)

Build Heap
O(n)

0-based Heap:
Left = 2i+1
Right = 2i+2
Parent = floor((i-1)/2)

YOUR CODE:
< root → LEFT
>= root → RIGHT
duplicate → RIGHT
disp() → INORDER
deinit() → POSTORDER
NULL nodes → 0
leaf → left=NULL && right=NULL
```

# 🎯 MOST IMPORTANT 15 FACTS

1. `n nodes → n−1 edges`
    
2. `Full → 0 or 2 children`
    
3. `Complete → last level left-to-right`
    
4. `Perfect → full + leaves same level`
    
5. `Preorder = Root Left Right`
    
6. `Inorder = Left Root Right`
    
7. `Postorder = Left Right Root`
    
8. `Level order = BFS + Queue`
    
9. `BST inorder = sorted`
    
10. `BST complexity = O(h)`
    
11. `Worst-case BST = O(n)`
    
12. `AVL BF = -1, 0, +1`
    
13. `Heap = Complete Binary Tree`
    
14. `Build Heap = O(n)`
    
15. **Your BST duplicates go RIGHT**