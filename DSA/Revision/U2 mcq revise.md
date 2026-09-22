# DSA MCQ MASTER NOTES

> **Purpose:** These notes are designed specifically for difficult/tricky MCQs.  
> Focus on **definitions + properties + edge cases + complexity + pointer behavior + formulas + common traps**.

---

# 1. LINKED LISTS

## 1.1 Doubly Linked List (DLL)

Each node contains:

```text
prev | data | next
```

- `prev` → previous node
    
- `next` → next node
    
- First node:
    
    ```text
    prev = NULL
    ```
    
- Last node:
    
    ```text
    next = NULL
    ```
    

---

## 1.2 DLL vs Singly Linked List

A DLL allows movement in both directions:

```text
A ⇄ B ⇄ C ⇄ D
```

A singly linked list:

```text
A → B → C → D
```

### Important MCQ fact

If you already have a **direct pointer to the node to be deleted**:

- DLL deletion → `O(1)`
    
- SLL deletion → generally `O(n)` if previous node must be found
    

Why?

DLL already gives:

```text
p->prev
p->next
```

So deletion can be:

```text
p->prev->next = p->next;
p->next->prev = p->prev;
```

No traversal is required.

---

# 2. DLL POINTER OPERATIONS

## 2.1 Insert before a node `p`

All four pointer relationships need to be handled:

```text
new->next = p;
new->prev = p->prev;
p->prev->next = new;
p->prev = new;
```

Conceptually:

```text
Before:

A ⇄ P

After:

A ⇄ NEW ⇄ P
```

### MCQ trap

If one of the pointer updates is missing, the list may become broken.

---

## 2.2 Delete a middle node `p`

```text
p->prev->next = p->next;
p->next->prev = p->prev;
free(p);
```

Result:

```text
A ⇄ P ⇄ C

becomes

A ⇄ C
```

---

## 2.3 `p->prev == NULL`

In a normal DLL:

```text
p->prev == NULL
```

means:

> `p` is the **first node**.

Similarly:

```text
p->next == NULL
```

means:

> `p` is the **last node**.

---

# 3. DLL EDGE CASES

## Empty DLL

If there is no head/tail sentinel:

```text
head == NULL
tail == NULL
```

---

## One-node DLL

```text
head == tail
```

and:

```text
head->prev == NULL
head->next == NULL
```

### Important

`head == tail` does **not necessarily mean empty**.

It can mean:

- one node
    

depending on the implementation.

---

# 4. DLL DELETING THE LAST NODE

Typical code:

```c
while (temp->next != NULL)
    temp = temp->next;

temp->prev->next = NULL;
free(temp);
```

The loop moves `temp` to the last node.

Then:

```text
temp->prev->next = NULL;
```

disconnects the last node.

Then:

```text
free(temp);
```

releases its memory.

### Critical edge case

If the DLL contains only **one node**:

```text
temp->prev == NULL
```

Therefore:

```c
temp->prev->next
```

dereferences `NULL`.

This causes **undefined behavior**.

A correct implementation must handle the one-node case separately.

---

# 5. CIRCULAR LINKED LIST

In a circular list, the last node points back to the first.

```text
A → B → C → D
↑           ↓
└───────────┘
```

Therefore:

```text
last->next == head
```

There is normally **no NULL at the end**.

---

# 6. CIRCULAR LIST TRAVERSAL

Normal linked list:

```c
while (p != NULL)
```

works because the list ends at `NULL`.

For a circular list, this can cause an infinite loop.

Use:

```c
p = head;

do {
    ...
    p = p->next;
} while (p != head);
```

### Why?

Eventually `p` returns to `head`.

So:

```text
p == head
```

is the termination condition.

---

## Critical MCQ trap

This is dangerous for a circular list:

```c
while (p != NULL)
```

because `p` may **never become NULL**.

---

# 7. EMPTY CIRCULAR LIST TRAP

Consider:

```c
do {
    ...
} while (temp != head);
```

If:

```text
head == NULL
temp == NULL
```

the body executes **before** the condition is checked.

If the body dereferences:

```c
temp->data
```

then it causes undefined behavior.

### Remember

A `do-while` always executes **at least once**.

---

# 8. ONE-NODE CIRCULAR LIST

For one node:

```text
head->next == head
```

Example:

```text
A
↑↓
```

The node points to itself.

---

# 9. CIRCULAR LIST POINTER QUESTIONS

Example:

```text
5 → 10 → 15 → 20 → 35
↑                   ↓
└───────────────────┘
```

If:

```text
p → 20
```

then:

```text
p->next
```

is `35`.

Therefore:

```text
p->next->next
```

is `5`.

So:

```c
p->next->next->data
```

gives:

```text
5
```

### MCQ technique

Trace one pointer movement at a time.

---

# 10. SLOW/FAST POINTER

Common pattern:

```text
p moves 1 step
q moves 2 steps
```

For example:

```c
p = p->next;
q = q->next->next;
```

Because `q` moves twice as fast, when the faster pointer completes a cycle, the slower pointer is approximately halfway around.

### MCQ trap

Always inspect whether:

- the list is linear or circular
    
- the pointers start at the same node
    
- the loop condition is checked before/after movement
    

These affect exact results.

---

# 11. POINTER MANIPULATION MCQ

Example list:

```text
10 ⇄ 20 ⇄ 30 ⇄ 40 ⇄ 50
```

Suppose:

```text
p → 30
```

Then:

```c
p = p->next;
```

makes:

```text
p → 40
```

Then:

```c
p->prev->next = p->next;
```

means:

```text
40->prev = 30
```

so:

```text
30->next = 50
```

Then:

```c
p->next->prev = p->prev;
```

makes:

```text
50->prev = 30
```

Final list:

```text
10 ⇄ 20 ⇄ 30 ⇄ 50
```

Node `40` has been bypassed.

---

# 12. CIRCULAR LIST INSERTION TRAP

Suppose:

```text
head → A → B → C → A
```

A new node is inserted after the old head.

Then:

```text
A → NEW → B → C → A
```

If afterwards:

```c
head = head->next;
```

then the new head becomes:

```text
NEW
```

### Important

Do not confuse:

> where a node is inserted

with:

> where `head` points after the insertion.

---

# 13. QUEUE

## Definition

A queue follows:

# FIFO

**First In, First Out**

Example:

```text
10 → 20 → 30
```

10 leaves first.

---

# 14. QUEUE TERMINOLOGY

```text
FRONT → 10 20 30 ← REAR
```

### Enqueue

Insert at:

```text
REAR
```

### Dequeue

Delete from:

```text
FRONT
```

---

# 15. BASIC QUEUE EXAMPLE

Start:

```text
empty
```

Enqueue 10:

```text
10
```

Enqueue 20:

```text
10 20
```

Enqueue 30:

```text
10 20 30
```

Dequeue:

```text
20 30
```

Dequeue:

```text
30
```

Therefore after:

```text
enqueue(10)
enqueue(20)
enqueue(30)
dequeue()
enqueue(40)
enqueue(50)
```

logical queue:

```text
20 30 40 50
```

---

# 16. LINKED QUEUE

Usually maintain:

```text
front
rear
```

Example:

```text
front
 ↓
10 → 20 → 30
          ↑
         rear
```

---

# 17. LINKED QUEUE COMPLEXITY

With both `front` and `rear`:

### Enqueue

Insert at rear:

```text
O(1)
```

### Dequeue

Delete from front:

```text
O(1)
```

Therefore:

```text
Enqueue = O(1)
Dequeue = O(1)
```

---

# 18. EMPTY LINKED QUEUE

Normally:

```text
front = NULL
rear = NULL
```

---

# 19. ONE-NODE LINKED QUEUE

For:

```text
10
```

we have:

```text
front == rear
```

This means:

> one node

not necessarily empty.

### Critical MCQ trap

Never assume:

```text
front == rear
```

means empty.

It can mean one-node queue.

---

# 20. DEQUEUE OF LAST NODE

Suppose:

```text
front = rear = 10
```

After deleting 10:

```text
front = NULL
rear = NULL
```

Both must be updated.

---

## Common bug

If code only does:

```c
front = front->next;
free(p);
```

then after deleting the last node:

```text
front == NULL
```

but:

```text
rear
```

may still point to freed memory.

That is an invalid/dangling state.

---

# 21. ENQUEUE INTO EMPTY LINKED QUEUE

If queue is empty:

```text
front = rear = NULL
```

and a new node is inserted:

```text
front = rear = new;
```

Both pointers must become the new node.

---

# 22. FALSE OVERFLOW IN LINEAR ARRAY QUEUE

Consider:

```text
[10][20][30][40][50]
 ↑               ↑
front           rear
```

Dequeue 10 and 20:

```text
[ ][ ][30][40][50]
       ↑       ↑
     front    rear
```

There are empty spaces at the beginning.

But if:

```text
rear == MAX - 1
```

is treated as overflow, the queue may report overflow.

This is called:

# False Overflow

because unused positions exist but cannot be reused.

---

# 23. CIRCULAR QUEUE

A circular queue connects the end of the array back to the beginning.

```text
0 → 1 → 2 → 3 → 4
↑               ↓
└───────────────┘
```

When rear reaches the end, it wraps around.

---

# 24. CIRCULAR QUEUE FORMULA

Next position:

```text
(rear + 1) % N
```

where `N` is the array size.

Example:

```text
N = 5
rear = 4
```

Then:

```text
(4 + 1) % 5 = 0
```

So rear wraps to index 0.

---

# 25. ONE-SLOT-EMPTY CIRCULAR QUEUE

A common convention is:

### Empty

```text
front == rear
```

### Full

```text
(rear + 1) % N == front
```

### Capacity

```text
N - 1
```

not `N`.

---

# 26. CIRCULAR QUEUE MCQ TRAPS

Do not automatically assume all circular queues use the same conventions.

Different implementations may define:

- `rear` as last occupied element
    
- `rear` as next insertion position
    

Therefore always inspect the question/code.

---

# 27. CIRCULAR QUEUE DELETION

When an element is dequeued:

```text
front
```

moves forward.

The remaining elements do **not** need to be shifted.

This is one of the main advantages over a simple linear array queue.

---

# 28. QUEUE USING TWO STACKS

To implement FIFO using two LIFO stacks:

```text
S1
S2
```

A common method:

### Enqueue

Push into `S1`.

### Dequeue

If `S2` is empty:

```text
move everything from S1 to S2
```

Then pop from `S2`.

Example:

```text
enqueue 1
enqueue 2
enqueue 3
enqueue 4
```

S1:

```text
4
3
2
1
```

Transfer to S2:

```text
1
2
3
4
```

Pop → `1`.

Next pop → `2`.

Therefore FIFO behavior is obtained.

---

# 29. APPLICATIONS OF QUEUES

Typical queue applications:

- Printer jobs
    
- CPU scheduling
    
- Waiting lines
    
- Network buffering
    
- BFS traversal
    
- Request processing
    

### MCQ trap

If people are served in arrival order:

```text
Queue
```

If someone can be pulled from the middle based on some condition:

```text
Not ordinary FIFO behavior
```

---

# 30. PRIORITY QUEUE

A priority queue removes elements according to:

# PRIORITY

not simply arrival time.

Example:

```text
A priority 5
B priority 2
C priority 9
```

If larger number = higher priority:

```text
C → A → B
```

---

# 31. PRIORITY DIRECTION IS IMPORTANT

Never assume:

```text
larger number = higher priority
```

or:

```text
smaller number = higher priority
```

The question must specify it.

Examples:

```text
Higher number = higher priority
```

means:

```text
9 > 5 > 2
```

But:

```text
Lower number = higher priority
```

means:

```text
2 > 5 > 9
```

in terms of service priority.

---

# 32. SAME PRIORITY

If two items have the same priority, the MCQs in this set use:

# FIFO

Example:

```text
A priority 4
B priority 4
```

A arrived first.

Therefore:

```text
A
B
```

must be removed in that order.

---

# 33. UNORDERED PRIORITY QUEUE

An unordered array/list does not maintain priority order.

Example:

```text
A(5)
B(2)
C(9)
D(4)
```

can be stored as:

```text
A B C D
```

without sorting.

---

## Complexity

### Insertion

Simply insert at a convenient location:

```text
O(1)
```

### Deletion

Must search for highest-priority element:

```text
O(n)
```

Therefore:

# Unordered PQ

```text
Insert = O(1)
Delete = O(n)
```

---

# 34. ORDERED PRIORITY QUEUE

Elements are maintained in priority order.

Example:

```text
Highest → A B C D → Lowest
```

### Insertion

Must find correct position:

```text
O(n)
```

### Deletion

If highest priority is maintained at an end:

```text
O(1)
```

Therefore:

# Ordered PQ

```text
Insert = O(n)
Delete = O(1)
```

---

# 35. MOST IMPORTANT PQ TABLE

|Implementation|Insert|Delete highest priority|
|---|--:|--:|
|Unordered array/list|O(1)|O(n)|
|Ordered array/list|O(n)|O(1)|

### Memorize

> **Unordered → Insert fast, Delete slow**

> **Ordered → Insert slow, Delete fast**

---

# 36. PRIORITY QUEUE SEARCH CODE

For an unordered list where larger priority is higher:

```c
max = head;

while (temp != NULL) {
    if (temp->priority > max->priority)
        max = temp;

    temp = temp->next;
}
```

At the end:

```text
max
```

points to the highest-priority node.

If smaller value is higher priority, reverse the comparison.

---

# 37. DYNAMIC PRIORITY

Sometimes priority changes with waiting time.

Example:

```text
effective priority = original priority - waiting time
```

Suppose:

```text
X: 10, waited 5
Y: 6, waited 0
Z: 8, waited 5
```

Calculate:

```text
X = 10 - 5 = 5
Y = 6 - 0 = 6
Z = 8 - 5 = 3
```

If **lower value = higher priority**:

```text
Z → X → Y
```

### MCQ rule

Never compare original priority if the question gives a dynamic-priority formula.

First calculate the **effective priority**.

---

# 38. ORDERED PQ + FIFO TIE BREAKING

Suppose highest priority is at the rear/end and deletion happens from that end.

For equal-priority jobs:

```text
Older job must remain closer to deletion end.
```

Therefore a newer equal-priority job may need to be inserted **before/to the left of** existing equal-priority jobs, depending on the exact representation.

### Example

Existing:

```text
A(5) B(5)
```

A is older.

If deletion occurs from the right:

```text
A B
```

would remove B first.

So a new equal-priority item must be positioned such that:

```text
older item
```

remains closer to the deletion end.

### Always inspect the physical representation.

---

# 39. DEQUE

Deque =

# Double-Ended Queue

Insertion and deletion can happen at **both ends**.

```text
FRONT ⇄ elements ⇄ REAR
```

Operations:

```text
insertFront
insertRear
deleteFront
deleteRear
```

---

# 40. DEQUE AS A QUEUE

To behave like a normal queue:

```text
insertRear
deleteFront
```

This gives:

```text
FIFO
```

---

# 41. DEQUE AS A STACK

To behave like a stack, use the same end for insertion and deletion.

For example:

```text
insertFront
deleteFront
```

or:

```text
insertRear
deleteRear
```

This gives:

```text
LIFO
```

---

# 42. DEQUE TRACE

Suppose a circular deque has size 5.

Operations:

```text
insertRear(10)
insertRear(20)
insertFront(30)
deleteRear()
insertFront(40)
deleteFront()
insertRear(50)
insertFront(60)
```

Trace:

```text
insertRear(10)
[10]

insertRear(20)
[10,20]

insertFront(30)
[30,10,20]

deleteRear()
[30,10]

insertFront(40)
[40,30,10]

deleteFront()
[30,10]

insertRear(50)
[30,10,50]

insertFront(60)
[60,30,10,50]
```

Final:

```text
FRONT → 60 30 10 50 ← REAR
```

Number of elements:

```text
4
```

So no overflow in a capacity-5 deque.

---

# 43. TREES

A tree is a connected acyclic graph.

Important properties:

```text
No cycles
Connected
```

---

# 44. NUMBER OF EDGES IN A TREE

For:

```text
n nodes
```

a tree always has:

```text
n - 1 edges
```

### Example

```text
5 nodes
```

means:

```text
4 edges
```

---

# 45. ADDING AN EDGE TO A TREE

A tree already has exactly one path between any two nodes.

Adding another edge creates another path.

Therefore:

# Adding an edge creates a cycle.

The resulting graph is:

```text
not a tree
```

---

# 46. REMOVING AN EDGE FROM A TREE

Removing any edge from a tree disconnects it.

Therefore:

# Removing an edge produces a forest.

For one removed edge:

```text
2 disconnected trees
```

---

# 47. UNIQUE PATH PROPERTY

Between any two nodes in a tree:

# Exactly one simple path exists.

This includes:

- root → leaf
    
- root → any node
    
- any node → any other node
    

---

# 48. ROOT

In a rooted tree:

- Root has no parent.
    
- Every other node has exactly one parent.
    

---

# 49. LEAF

A leaf is a node with:

```text
0 children
```

Example:

```text
      A
     / \
    B   C
```

B and C are leaves.

---

# 50. SIBLINGS

Two nodes are siblings if:

# They have the same parent.

Example:

```text
      A
    / | \
   B  C  D
```

B, C and D are siblings.

---

# 51. ANCESTOR

A node is an ancestor of another node if it occurs above it on the path from the root.

Example:

```text
      A
      |
      B
      |
      C
```

A is an ancestor of:

```text
B
C
```

B is an ancestor of:

```text
C
```

### MCQ convention

A node is generally **not considered its own ancestor**.

---

# 52. UNCLE

Example:

```text
        A
       / \
      B   C
     /
    D
```

D's:

- parent = B
    
- grandparent = A
    
- uncle = C
    

because C is the sibling of D's parent.

---

# 53. GENERAL TREE LEAF COUNT

For a tree with `n` nodes:

### Minimum possible leaves

```text
1
```

This occurs in a chain:

```text
A
|
B
|
C
|
D
```

Only D is a leaf.

---

### Maximum possible leaves

```text
n - 1
```

This occurs when the root has every other node as a child:

```text
        A
   / / / | \ \
  B C D E F G
```

If there are `n` nodes:

```text
n - 1 leaves
```

---

# 54. BINARY TREE

A binary tree is either:

```text
empty
```

or:

```text
root
├── left subtree
└── right subtree
```

Each node can have:

```text
0
1
2
```

children.

### Important

A binary tree does **not** require every node to have exactly two children.

---

# 55. BINARY TREE NODE POINTERS

Each binary-tree node normally has:

```text
left pointer
right pointer
```

Therefore with `n` nodes:

```text
2n pointer fields
```

---

# 56. NULL POINTERS IN A BINARY TREE

A binary tree with `n` nodes has:

```text
n - 1
```

actual edges.

But there are:

```text
2n
```

pointer fields.

Therefore NULL pointers:

```text
2n - (n - 1)
```

which equals:

# n + 1

So:

```text
Binary tree with n nodes
→ n + 1 NULL child pointers
```

### Very common MCQ.

---

# 57. BINARY TREE HEIGHT

In these MCQs, height is measured in:

# EDGES

Height = number of edges on the longest root-to-leaf path.

---

## Maximum height

For `n` nodes, maximum height occurs in a completely skewed tree:

```text
A
|
B
|
C
|
D
```

For 4 nodes:

```text
height = 3
```

Therefore:

# Maximum height = n - 1

---

## Example

15-node binary tree:

```text
maximum height = 15 - 1 = 14
```

---

# 58. COMPLETE BINARY TREE

A complete binary tree has:

- all levels completely filled except possibly the last
    
- last level filled from **left to right**
    

Example:

```text
        A
      /   \
     B     C
    / \   /
   D   E F
```

This is complete.

---

# 59. LEAVES IN A COMPLETE BINARY TREE

For `n` nodes:

```text
number of leaves = floor((n + 1) / 2)
```

Equivalent:

```text
ceil(n / 2)
```

### Example

For:

```text
n = 15
```

```text
floor((15 + 1)/2)
= 8
```

Therefore:

```text
8 leaves
```

---

# 60. TREE TRAVERSALS

There are three fundamental depth-first traversals:

```text
Preorder
Inorder
Postorder
```

---

# 61. PREORDER

Order:

# Root → Left → Right

Mnemonic:

```text
RLR
```

Example:

```text
      A
     / \
    B   C
```

Preorder:

```text
A B C
```

---

# 62. INORDER

Order:

# Left → Root → Right

Mnemonic:

```text
LRR
```

More clearly:

```text
Left
Root
Right
```

For:

```text
      A
     / \
    B   C
```

Inorder:

```text
B A C
```

---

# 63. POSTORDER

Order:

# Left → Right → Root

For:

```text
      A
     / \
    B   C
```

Postorder:

```text
B C A
```

---

# 64. TRAVERSAL MEMORY TRICK

```text
PRE  → Root first
IN   → Root middle
POST → Root last
```

So:

```text
Preorder  = Root Left Right
Inorder   = Left Root Right
Postorder = Left Right Root
```

---

# 65. POSTORDER TRAP

The **last node visited in postorder is always the root**.

Therefore if a question asks:

> Which node occurs last in postorder?

Answer:

```text
Root
```

### Do not confuse with:

> Rightmost node of a BST

which is the maximum-value node.

---

# 66. RECURSIVE TREE FUNCTION TRAP

Suppose:

```c
void disp_recursive(node *root)
{
    ...
    disp_recursive(root);
}
```

The recursive call uses the **same root**.

Therefore the function does not move to a smaller subtree.

Result:

# Infinite recursion

Correct recursive traversal normally changes the argument:

```c
disp_recursive(root->left);
disp_recursive(root->right);
```

or otherwise reaches a base case.

---

# 67. LEAF-CHECK FUNCTION

Suppose:

```c
int what(node *root)
{
    if (root == NULL)
        return 0;

    if (root->left == NULL &&
        root->right == NULL)
        return 1;

    return -1;
}
```

If called on the root:

### Empty tree

```text
return 0
```

### One-node tree

Root has no children:

```text
return 1
```

### Tree with more than one node

Root has at least one child:

```text
return -1
```

Therefore:

> `what(root) == 1` iff the tree consists of exactly one node.

---

# 68. BINARY SEARCH TREE (BST)

A BST is a binary tree with an ordering property.

Standard convention:

```text
left subtree < root < right subtree
```

Therefore:

```text
smaller values → left
larger values → right
```

---

# 69. BST SEARCH

To search for `x`:

If:

```text
x < root->data
```

go left.

If:

```text
x > root->data
```

go right.

If:

```text
x == root->data
```

found.

---

# 70. BST INORDER

One of the most important facts:

# Inorder traversal of a BST gives sorted order.

Example:

```text
       50
      /  \
    30    70
   / \    / \
 20  40  60 80
```

Inorder:

```text
20 30 40 50 60 70 80
```

Therefore if an MCQ gives a BST and asks for inorder:

# Think SORTED ORDER.

---

# 71. BST MINIMUM

The minimum value is found by repeatedly moving:

```text
LEFT
```

Therefore:

# Minimum = leftmost node

---

# 72. BST MAXIMUM

The maximum value is found by repeatedly moving:

```text
RIGHT
```

Therefore:

# Maximum = rightmost node

---

# 73. BST MAXIMUM NODE TRAP

The maximum node:

```text
has no right child
```

But it **can have a left child**.

Example:

```text
       80
      /
     70
```

80 is maximum, but it is not a leaf.

Therefore:

> "Maximum node is always a leaf"

is false.

Correct statement:

> Maximum node has no right child.

---

# 74. BST DUPLICATES

Duplicates are not universally handled the same way.

The insertion rule matters.

Example rule:

```text
if data <= root->data
    go left
else
    go right
```

Then duplicates go:

# LEFT

---

## Example

Insert:

```text
50
30
70
30
```

First 30:

```text
   50
  /
 30
```

Second 30:

Since:

```text
30 <= 30
```

go left:

```text
     50
    /  \
   30   70
  /
 30
```

Inorder:

```text
30(second)
30(first)
50
70
```

---

## Other possible convention

If:

```text
data < root->data
```

go left,

and:

```text
data >= root->data
```

go right,

then duplicates go:

# RIGHT

### MCQ rule

Always inspect the insertion condition before deciding where duplicates go.

---

# 75. LCRS REPRESENTATION

LCRS means:

# Left-Child Right-Sibling

It represents a general tree using binary-tree-like pointers.

---

# 76. LCRS POINTER MEANING

For a node:

```text
left pointer
```

means:

# First child

And:

```text
right pointer
```

means:

# Next sibling

This is extremely important.

---

# 77. LCRS EXAMPLE

General tree:

```text
       A
    /  |  \
   B   C   D
```

LCRS:

```text
A.left = B

B.right = C

C.right = D
```

So:

```text
A
|
B → C → D
```

The arrows from B to C to D represent sibling relationships.

---

# 78. LCRS WITH CHILDREN

Suppose:

```text
       A
    /  |  \
   B   C   D
      / \
     E   F
```

Then:

```text
A.left = B

B.right = C
C.right = D

C.left = E

E.right = F
```

---

# 79. COUNTING CHILDREN USING LCRS

To find number of children of node `X`:

### Step 1

Go to:

```text
X.left
```

This gives first child.

### Step 2

Move through:

```text
child->right
```

because right means next sibling.

### Example

```text
X.left = A
A.right = B
B.right = C
C.right = NULL
```

Children:

```text
A, B, C
```

Count:

```text
3
```

---

# 80. LCRS MCQ TRAP

In LCRS:

```text
right pointer ≠ right child
```

Instead:

```text
right pointer = next sibling
```

This is one of the most important traps.

---

# 81. TREE QUESTION — TRAVERSAL

Consider:

```text
        A
       / \
      B   C
     / \ / \
    D  E F  G
   /
  H
```

### Inorder

Treat it as a binary tree:

```text
D B H E A F C G
```

Therefore the node immediately after `E` is:

```text
A
```

### Important correction

Do not assume:

```text
H
```

comes after E.

The correct inorder sequence is:

```text
D → B → H → E → A → F → C → G
```

---

# 82. COMMON TREE MCQ TRAPS

## Trap 1

```text
n nodes → n edges
```

Wrong.

Correct:

```text
n nodes → n - 1 edges
```

---

## Trap 2

Adding an edge makes a larger tree.

Wrong.

Adding an edge to a tree:

```text
creates a cycle
```

so it is no longer a tree.

---

## Trap 3

Removing an edge keeps the graph connected.

Wrong.

Removing any tree edge:

```text
disconnects the tree
```

and produces a forest.

---

## Trap 4

Binary tree means every node has exactly two children.

Wrong.

A binary-tree node can have:

```text
0, 1, or 2 children
```

---

## Trap 5

Binary tree with n nodes has n NULL pointers.

Wrong.

Correct:

```text
n + 1 NULL pointers
```

---

## Trap 6

Maximum binary-tree height = n.

Wrong under edge-based height.

Correct:

```text
n - 1
```

---

## Trap 7

Inorder of any binary tree is sorted.

Wrong.

Only:

# BST inorder = sorted

---

## Trap 8

Maximum BST node must be a leaf.

Wrong.

It has:

```text
no right child
```

but may have a left child.

---

## Trap 9

LCRS right pointer means right child.

Wrong.

It means:

```text
next sibling
```

---

## Trap 10

`front == rear` means queue empty.

Not necessarily.

Depending on implementation, it can mean:

```text
one element
```

---

## Trap 11

Circular linked list traversal uses:

```c
p != NULL
```

Wrong.

Normally terminate when:

```c
p == head
```

after completing a traversal.

---

## Trap 12

Priority queue is FIFO.

Wrong.

Priority determines removal.

FIFO usually applies only when priorities are equal.

---

# 83. MASTER COMPLEXITY TABLE

|Data Structure / Operation|Complexity|
|---|--:|
|DLL delete given node|O(1)|
|DLL insert before given node|O(1)|
|Linked Queue enqueue|O(1)|
|Linked Queue dequeue|O(1)|
|Unordered PQ insert|O(1)|
|Unordered PQ delete highest priority|O(n)|
|Ordered PQ insert|O(n)|
|Ordered PQ delete highest priority|O(1)|

---

# 84. MASTER FORMULA SHEET

## Tree

```text
n nodes → n - 1 edges
```

---

## Binary tree

```text
Pointer fields = 2n
Actual edges = n - 1
NULL pointers = n + 1
```

---

## Maximum binary-tree height

```text
hmax = n - 1
```

---

## Complete binary-tree leaves

```text
leaves = floor((n + 1) / 2)
```

Equivalent:

```text
leaves = ceil(n / 2)
```

---

## General tree leaves

```text
minimum = 1
maximum = n - 1
```

---

## Circular queue

Next position:

```text
(rear + 1) % N
```

One-slot-empty convention:

```text
empty: front == rear

full: (rear + 1) % N == front

capacity: N - 1
```

---

# 85. MASTER TRAVERSAL TABLE

|Traversal|Order|
|---|---|
|Preorder|Root → Left → Right|
|Inorder|Left → Root → Right|
|Postorder|Left → Right → Root|

### Memory trick

```text
PRE  = Root first
IN   = Root middle
POST = Root last
```

---

# 86. MASTER BST TABLE

|Question|Answer|
|---|---|
|Smaller value|Left|
|Larger value|Right|
|Minimum|Leftmost|
|Maximum|Rightmost|
|Inorder|Sorted ascending|
|Maximum node|No right child|
|Minimum node|No left child|
|Duplicate placement|Depends on insertion rule|

---

# 87. MASTER QUEUE TABLE

|Property|Queue|
|---|---|
|Principle|FIFO|
|Insert|Rear|
|Delete|Front|
|Linked enqueue|O(1)|
|Linked dequeue|O(1)|
|Empty linked queue|front = rear = NULL|
|One-node queue|front = rear|
|Circular queue|Wrap-around|
|Circular next position|`(rear+1)%N`|

---

# 88. MASTER PRIORITY QUEUE TABLE

|Property|Priority Queue|
|---|---|
|Removal based on|Priority|
|Different priorities|Higher priority first|
|Same priority|FIFO in these MCQs|
|Unordered insert|O(1)|
|Unordered delete|O(n)|
|Ordered insert|O(n)|
|Ordered delete|O(1)|
|Priority direction|Must read question|

---

# 89. MASTER DEQUE TABLE

|Operation|Allowed?|
|---|---|
|Insert front|Yes|
|Insert rear|Yes|
|Delete front|Yes|
|Delete rear|Yes|

Deque as queue:

```text
insertRear + deleteFront
```

Deque as stack:

```text
insertFront + deleteFront
```

or:

```text
insertRear + deleteRear
```

---

# 90. HIGH-RISK MCQ DISTINCTIONS

Memorize these pairs.

### Queue vs Priority Queue

```text
Queue:
arrival order

Priority Queue:
priority order
```

---

### Linear Queue vs Circular Queue

```text
Linear:
may suffer false overflow

Circular:
reuses freed positions
```

---

### Queue vs Deque

```text
Queue:
insert rear
delete front

Deque:
insert/delete at both ends
```

---

### Binary Tree vs BST

```text
Binary Tree:
at most 2 children

BST:
binary tree + ordering property
```

---

### Tree vs Graph

```text
Tree:
connected + acyclic
```

---

### LCRS vs Normal Binary Tree

```text
LCRS left:
first child

LCRS right:
next sibling
```

---

### General Tree vs Binary Tree

General tree can have:

```text
0, 1, 2, 3, ... children
```

Binary tree:

```text
maximum 2 children
```

---

# 91. POINTER-TRACING STRATEGY FOR MCQs

When you see something like:

```c
p = p->next;
p->prev->next = p->next;
p->next->prev = p->prev;
```

Do NOT try to visualize the entire operation mentally at once.

Do this:

### Step 1

Write where `p` currently points.

### Step 2

Execute:

```text
p = p->next
```

and update `p`.

### Step 3

Evaluate:

```text
p->prev
```

### Step 4

Evaluate:

```text
p->next
```

### Step 5

Apply the pointer assignment.

### Step 6

Draw the affected links again.

This prevents most pointer-manipulation mistakes.

---

# 92. TRACING QUEUE/DEQUE QUESTIONS

For every operation, maintain:

```text
FRONT → [elements] ← REAR
```

After each operation:

1. Add/delete the correct element.
    
2. Update front/rear.
    
3. Check capacity.
    
4. Only then move to the next operation.
    

Do not try to calculate the final state directly.

---

# 93. TRACING PRIORITY QUEUE QUESTIONS

For every item write:

```text
Item | Priority | Arrival order
```

Then:

### Step 1

Determine whether:

```text
larger = higher
```

or:

```text
smaller = higher
```

### Step 2

If priorities differ:

```text
priority wins
```

### Step 3

If priorities are equal:

```text
FIFO
```

### Step 4

Repeat until all items are removed.

---

# 94. TRACING BST INSERTION

For every inserted value:

### Start at root.

If:

```text
new < current
```

go:

```text
LEFT
```

If:

```text
new > current
```

go:

```text
RIGHT
```

If duplicate:

# STOP AND READ THE DUPLICATE RULE.

For example:

```c
if (data <= root->data)
```

means duplicate goes left.

Whereas:

```c
if (data < root->data)
```

with an `else` going right means duplicate may go right.

---

# 95. TREE HEIGHT TRAP

Always check how height is defined.

If height is measured in **edges**:

```text
single-node tree → height 0
```

If a textbook/course defines height in **levels/nodes**, the answer can differ.

For these MCQs, the convention used was:

```text
height = number of edges
```

Therefore:

```text
n-node skewed binary tree → height n-1
```

---

# 96. FINAL 30-SECOND REVISION

Before entering the exam, remember:

```text
TREE
n nodes → n-1 edges
add edge → cycle
remove edge → forest
unique path between any two nodes

BINARY TREE
0/1/2 children
2n pointer fields
n-1 actual edges
n+1 NULL pointers
max height = n-1
complete-tree leaves = floor((n+1)/2)

TRAVERSALS
Pre  = Root Left Right
In   = Left Root Right
Post = Left Right Root

BST
left < root < right
inorder = sorted
minimum = leftmost
maximum = rightmost
maximum has no right child
duplicates depend on insertion rule

LCRS
left = first child
right = next sibling

GENERAL TREE
min leaves = 1
max leaves = n-1

QUEUE
FIFO
insert rear
delete front
linked enqueue/dequeue = O(1)
empty = front = rear = NULL
one node = front == rear

CIRCULAR QUEUE
next = (rear+1)%N
empty = front==rear
full = (rear+1)%N==front
capacity = N-1

PRIORITY QUEUE
priority beats arrival
same priority → FIFO
unordered: insert O(1), delete O(n)
ordered: insert O(n), delete O(1)

DEQUE
both ends
queue = rear insert + front delete
stack = same-end insert/delete

DLL
prev + data + next
given-node deletion = O(1)
prev=NULL → first
next=NULL → last

CIRCULAR LIST
last->next = head
do not use p != NULL as normal termination
one node: head->next == head
```

# 97. ABSOLUTE MCQ RULES

When stuck, check these in this exact order:

1. **What data structure is actually being used?**
    
2. **What convention is the question using?**
    
3. **Where are insertion and deletion happening?**
    
4. **Is priority larger-is-higher or smaller-is-higher?**
    
5. **Are equal priorities FIFO?**
    
6. **Is height measured in edges or levels?**
    
7. **Is the tree a general binary tree, complete binary tree, or BST?**
    
8. **Are duplicates allowed in the BST, and where are they inserted?**
    
9. **In LCRS, remember right = sibling, not child.**
    
10. **For pointer code, execute one statement at a time.**
    
11. **For queue/deque operations, trace the structure after every operation.**
    
12. **For traversal questions, write Root/Left/Right explicitly rather than relying only on abbreviations.**
    

# END OF NOTES