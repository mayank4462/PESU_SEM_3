---
title: "DSA Unit 2 — 30-Minute Revision"
tags:
  - dsa
  - unit-2
  - revision
  - mcq
  - linked-list
  - queue
  - priority-queue
  - tree
  - bst
  - deque
source: "u2_dsa(4).pdf — PES University slides"
revision_target: "~30 minutes"
---

# DSA Unit 2 — ⚡ 30-Minute Revision

> [!abstract] What this note is
> A **high-yield compression of the uploaded Unit 2 PDF**. It keeps the definitions, implementation logic, pointer rules, operation behaviour, complexity patterns, quiz traps, and the Round Robin experiential task — without reproducing the slide deck.
>
> **Exam mode:** Read the 🔥 / 🧠 / ⚠️ callouts first. Then do the MCQ Killer Sheet.

## 🗺️ 30-Minute Route

|      Time | Topic                   | What to remember                                                    |
| --------: | ----------------------- | ------------------------------------------------------------------- |
|   0–4 min | Circular List           | cycle ≠ NULL, `do...while`, one-node case, Josephus                 |
|   4–8 min | Doubly Linked List      | `prev_` + `next_`, 4 pointer changes, head/tail edge cases          |
|  8–12 min | Circular Array Queue    | `front_ == rear_`, but **when** you test it matters                 |
| 12–18 min | Priority Queue          | priority decides deletion; implementation changes cost              |
| 18–24 min | Trees + BST             | `n−1` edges, binary tree, inorder, BST direction, worst-case search |
| 24–27 min | Deque + Stack via Deque | both-end operations, composition/delegation                         |
| 27–30 min | MCQ Killer Sheet        | exact traps from the slides                                         |

---

# 1. 🔄 Circular Linked List

**Source focus:** PDF pp. 12–19.

## Core idea

A circular linked list is a list in which:

- The **last node points back to the first node**.
- There is a pointer to a node in the list; `head_` is used in the given implementation.
- “First” and “last” are less meaningful because the structure is a cycle.
- It is useful when elements form a cycle or must be processed repeatedly.

### Structure

```c
struct node {
    int key_;
    struct node *next_;
};

struct clist {
    node_t *head_;
};
```

> [!tip] 🧠 Mental picture
> **Singly list:** `A → B → C → NULL`
>
> **Circular list:** `A → B → C ↘`
> `        ↖──────────`

## Traversal — the #1 MCQ trap

In a normal singly linked list, you often write:

```c
while (temp != NULL) { ... }
```

In a circular list, that is the wrong stopping idea because the pointer can keep cycling.

The slide explicitly prefers:

```c
do {
    // process temp
    temp = temp->next_;
} while (temp != head_);
```

### 🔥 MCQ rule

> **Circular list → stop when you return to the starting node, NOT when `temp == NULL`.**

If an option says **“check whether current has returned to the starting node”**, that is the source-aligned answer.

## Edge cases

### Empty list

```text
head_ == NULL
```

### One-node circular list

```text
head_->next_ == head_
```

> [!warning] MCQ trap
> A one-node **circular** list does **not** have `next_ == NULL`.
> It points to itself.

### Delete the only node

After deleting the sole node:

```text
head_ = NULL
```

### Implementation points from the slides

- Initialize → `head_ = NULL`.
- Deinitialize → traverse, free nodes until back at the first node, then ground `head_`.
- Insertion → similar pointer operation to linked-list insertion; update `head_` when needed.
- Deletion → handle the circular link correctly.
- Traversal → prefer `do...while` and stop when you reach the first node again.
- Search → traverse until the designated key is found or you return to the starting point.

## Applications

- Multiplayer turn management
- Repeating music playlist
- Repeating image slideshow
- Token passing / ring-based algorithms

---

# 2. ☯️ Josephus Problem

## Problem pattern

There are `n` people in a circle.

- Start at a designated point.
- Move in a fixed direction.
- Skip `k−1` people.
- Remove the `k`-th person.
- Continue until one survivor remains.

### Example in the slides

```text
n = 7
k = 3

Eliminated: 3, 6, 2, 7, 5, 1
Survivor:   4
```

> [!tip] MCQ shortcut
> **Josephus = circular structure + repeated counting + deletion + one survivor.**
> If the question says “every kth person is removed until one remains,” think **circular list/Josephus** immediately.

---

# 3. ↔️ Doubly Linked List (DLL)

**Source focus:** PDF pp. 32–40.

## Core idea

Each node contains **two links**:

```c
struct node {
    int key_;
    struct node *prev_;
    struct node *next_;
};
```

The list has:

```c
struct DLL {
    node_t *head_;
    node_t *tail_;
};
```

### Direction

```text
NULL ← A ↔ B ↔ C → NULL
^                  ^
head_              tail_
```

> [!tip] 🔥 One-line memory hook
> **DLL = `prev_` gives backward movement + `next_` gives forward movement.**

## Key edge rules

### Empty list

Both change when the first node is inserted:

```text
head_ = new_node
tail_ = new_node
```

### Add at beginning

- `head_` changes.
- `tail_` does not normally change unless the list was empty.

### Add at end

- `tail_` changes.
- `head_` does not normally change unless the list was empty.

### Add in the middle

There are **4 pointer changes**.

Suppose `temp` is inserted immediately after `p`:

```text
Before:
A ↔ p ↔ Q

After:
A ↔ p ↔ temp ↔ Q
```

The essential pointer relationships are:

```c
temp->next_ = p->next_;
temp->prev_ = p;
p->next_->prev_ = temp;
p->next_ = temp;
```

> [!warning] MCQ killer
> If the option only updates `p->next_` and `temp` but **forgets the old next node's `prev_`**, the DLL is broken in the backward direction.

## Delete a known middle node

For node `p`:

```c
p->prev_->next_ = p->next_;
p->next_->prev_ = p->prev_;
```

Then the removed node itself must be released if the question asks about correct memory handling.

> [!warning] Important distinction
> The two pointer assignments **isolate/unlink** `p`.
> They do **not by themselves free the allocated node**.
>
> Therefore, a question asking “does this handle memory correctly?” may distinguish unlinking from actually deallocating.

## Deletion edge cases

| Case | What changes? |
|---|---|
| Beginning | `head_` |
| End | `tail_` |
| Middle | predecessor/successor links |
| Only node | **both `head_` and `tail_`** |

## Singly vs Doubly — high-yield table

| Operation (worst case, `n` nodes) | Singly LL | Doubly LL |
|---|---:|---:|
| Move forward 1 step | 1 | 1 |
| Move backward | `n` / re-traversal | **1** |
| Insert before known node | `n` / find predecessor | **1** using `prev_` |
| Delete designated known node | `n` / find predecessor | **1** using local links |
| Search for a key | `n` | `n` |

> [!tip] MCQ pattern
> **DLL improves local insertion/deletion and backward movement; it does NOT magically make searching O(1).**

## Advantages

- Bidirectional traversal
- Easy deletion
- Easy insertion before/after a known node
- Natural for navigation

## Disadvantages

- Extra memory for `prev_`
- More pointer manipulation
- More opportunities for bugs
- Search is still linear

## Applications

- Browser history
- Music playlist
- Undo/redo
- LRU cache

---

# 4. 🧱 Queue Using a Circular Array

**Source focus:** PDF pp. 53–59.

## Structure

```c
#define MAXSIZE 5

struct queue {
    int key_[MAXSIZE];
    int front_;
    int rear_;
};
```

The slides use a **circular array** so positions wrap around with `% MAXSIZE`.

## Initialization

The provided implementation uses:

```c
front_ = rear_ = 0;
```

> [!warning] MCQ trap
> Do **not** blindly use the common `-1` queue convention here.
> **For THIS implementation, both start at 0.**

## Empty vs Full — the sneakiest point

The source uses the same comparison:

```c
front_ == rear_
```

for both tests — but at different moments.

### `is_empty()`

Called **before dequeuing / before incrementing `front_`**:

```c
return front_ == rear_;
```

### `is_full()`

Called **after incrementing `rear_` during enqueue**:

```c
return front_ == rear_;
```

> [!danger] 🔥 Golden MCQ rule
> **Same code ≠ same meaning. Timing matters.**
>
> `is_empty` → compare **before** incrementing front.
>
> `is_full` → compare **after** incrementing rear.

## Circular movement

The source uses:

```c
++rear_ % MAXSIZE
```

and

```c
++front_ % MAXSIZE
```

So an index wraps around after reaching the array boundary.

## Enqueue flow

```text
1. Increment rear with wrap-around.
2. Update rear.
3. Check full.
4. If not full → store key at new rear.
5. If full → roll rear back and report full.
```

Source code pattern:

```c
temp = ++rear_ % MAXSIZE;
rear_ = temp;
if (!is_full(queue)) {
    key_[temp] = key;
}
else {
    rear_ = (rear_ - 1) % MAXSIZE;
    /* queue full */
}
```

## Dequeue flow

```text
1. Check empty.
2. If not empty, increment front with wrap-around.
3. Read the key.
4. Update front.
5. Return the key.
```

> [!tip] Memory hook
> **Enqueue moves `rear_`; dequeue moves `front_`.**

## Deinitialize

The slide explicitly says:

```c
front_ = rear_ = 0;
```

and **NO LOOP** is required.

Why? The implementation is array-backed, so there are no individually allocated nodes to free.

## Queue applications from the slides

- CPU scheduling / batch processing
- Printer spooler
- Breadth First Search
- Simulation models
- Message queues in operating systems
- Toll booth waiting lines

---

# 5. 🚨 Priority Queue — Core Concept

**Source focus:** PDF pp. 73–77 and pp. 91–98.

A priority queue is an **ADT** where each element has an associated priority.

### The defining operation

> **Deletion removes the element with the highest priority first.**

The numeric convention is not universal in the slides:

- Higher number **could** mean higher priority.
- Lower number **could** mean higher priority.

So never assume “bigger number = higher priority” unless the question explicitly defines that convention.

### Ordinary queue vs priority queue

```text
Ordinary queue → processing primarily follows arrival/FIFO behaviour
Priority queue → processing primarily follows priority
```

The source explicitly describes the distinction as deletion being determined by priority.

## Important interface

```text
initialize()
deinitialize()
enqueue(x)
dequeue()
isEmpty()
isFull()
```

The slides emphasize that **how enqueue is implemented affects how dequeue works**.

## Possible implementations in the slides

1. Unordered list
2. Ordered list
3. Unordered array
4. Ordered array
5. Heap tree

> [!tip] 🧠 Big idea
> A priority queue is an **ADT**, not a specific data structure.
> List, array, and heap are implementation choices.

## Applications

- CPU scheduling
- Hospital emergency services
- Printer scheduling
- Network packet scheduling
- Dijkstra’s shortest path
- Prim’s minimum spanning tree
- Event-driven simulation
- Huffman coding

---

# 6. 📋 Priority Queue — Unordered List

**Source focus:** PDF pp. 95–98.

## Meaning

The elements are **not maintained in priority order**.

```text
Example:
(priority values)  4 → 1 → 7 → 2 → 5
```

The list itself does not stay sorted by priority.

## Enqueue

The slide implementation:

- Make a node.
- Populate it.
- Add at the rear.
- Change `front` if required.
- Only a few operations.

### Complexity

**Enqueue → O(1)** / constant number of pointer operations in the described implementation.

## Dequeue

To remove the highest-priority component:

1. Traverse front → rear.
2. Find the **leftmost** node with the highest priority.
3. Copy/return the job.
4. Delete that node suitably.

### Complexity

**Dequeue → O(n)** worst case.

> [!danger] MCQ killer
> **Unordered list = cheap enqueue, expensive dequeue.**
>
> Why? You do not maintain order during insertion, so you must search during deletion.

### Tie clue

The slide says to find the **leftmost highest-priority node**.

Therefore, for this implementation, when equal priorities occur, the earlier/highest-priority match encountered from the front is selected.

Do **not** turn that into a universal rule for every priority queue implementation.

---

# 7. 📈 Priority Queue — Ordered List

**Source focus:** PDF pp. 116–118.

## Meaning

The list is kept **ordered according to priority**.

## Initialization

```text
front = NULL
rear  = NULL
```

## Empty / Full

- Empty → `front == NULL`
- Full → memory allocation fails (as described by the slide)

## Enqueue

Steps:

1. Make a node.
2. Populate it.
3. Find the correct position.
4. Change `front` if required.

Because the correct insertion position has to be found:

**Enqueue → O(n)** in the described implementation.

## Dequeue

Because the elements are already ordered:

1. Copy the job to return.
2. Move `front`.
3. Release the removed node.
4. Adjust `rear` if necessary.

**Dequeue → O(1)** in the described implementation.

> [!tip] Memory hook
> **Ordered structure pays during enqueue so dequeue can be cheap.**

---

# 8. 🗃️ Priority Queue — Unordered Array

**Source focus:** PDF pp. 119–120.

## State representation

The implementation tracks the **number of elements**.

### Initialization / Deinitialization

```text
num_elements = 0
```

### Empty

```text
num_elements == 0
```

### Full

```text
num_elements == array_size
```

## Enqueue

- Check full.
- Add at the **end**.
- Increase the size.
- Only a few operations.

**Enqueue → O(1)** in the described implementation.

### Why?

There is no need to search for a priority position because the array is unordered.

## Dequeue

- Traverse from the beginning.
- Find the **leftmost** highest-priority element.
- Copy the job.
- Shift elements **right to left** from the selected position until the end.
- Decrease size.
- Return the job.

**Dequeue → O(n)**.

> [!warning] MCQ wording to memorize
> In the source implementation, unordered-array dequeue involves **shifting right → left** to close the hole.

---

# 9. 📊 Priority Queue — Ordered Array

**Source focus:** PDF pp. 121–122.

## State

Again, the implementation tracks the **number of elements**.

- Initialize → count = 0
- Empty → count = 0
- Full → count = array size

## Enqueue

Because the array must remain priority-ordered:

1. Check full.
2. Traverse **right to left**.
3. Shift elements as needed.
4. Find the insertion position.
5. Insert.
6. Increase size.

**Enqueue → O(n)**.

## Dequeue

The priority ordering means the appropriate end already contains the element to remove.

The slide says:

- Check empty.
- Find the **rightmost element**.
- Decrement size.
- Return the rightmost job **before changing size**.

**Dequeue → O(1)**.

> [!tip] 🚀 Memory hook
> **Ordered array = expensive enqueue, cheap dequeue.**

---

# 10. ⚔️ Priority Queue Comparison — MUST MEMORIZE

| Implementation | Enqueue | Dequeue | Why? |
|---|---:|---:|---|
| **Unordered list** | **O(1)** | **O(n)** | Insert anywhere cheap; search highest priority on delete |
| **Ordered list** | **O(n)** | **O(1)** | Find insertion position; front already gives priority order |
| **Unordered array** | **O(1)** | **O(n)** | Append cheaply; scan + shift on delete |
| **Ordered array** | **O(n)** | **O(1)** | Shift to preserve order; remove from rightmost end |
| **Heap tree** | Listed as an implementation | — | This PDF section does not develop its operations in detail |

### 🔥 The pattern

```text
UNORDERED → enqueue cheap, dequeue expensive
ORDERED   → enqueue expensive, dequeue cheap
```

That single pattern kills a large fraction of implementation-complexity MCQs.

---

# 11. 🌳 Trees — General Tree

**Source focus:** PDF pp. 137–144.

## Core definition

A tree is a **non-linear data structure** for representing hierarchical relationships using a parent–child structure.

A tree can be viewed as:

- Empty, OR
- A root followed by a number of trees called **subtrees**.

## Terminology appearing in the slides

### Genealogy language

- Parent
- Child
- Siblings
- Ancestor
- Descendant
- Uncle

### Botany language

- Root
- Nodes
- Leaves
- Internal nodes
- External nodes
- Branch

### Other terms

- Path
- Level
- Height

## Fundamental properties

### 1. Acyclic

You cannot start at a node, traverse around, and return to the same node.

### 2. Connected

Any two nodes are connected by a path.

### 3. Edge rule

For a tree with `n` nodes:

```text
Edges = n − 1
```

> [!danger] MCQ killer
> **Tree with `n` nodes → exactly `n−1` edges.**
>
> Add one extra edge → a cycle appears.
>
> Remove an edge → the structure becomes more than one tree/component.

## Possible general-tree ADT

```text
Create / Destroy
Check empty / full
Add / Remove node
Traverse
Find root
Find children
Check leaf
Find height
Find root-to-node path
```

## Applications from the slides

- File-system structure
- Search trees
- Databases / indexing / B-trees
- Expression evaluation
- Compiler design
- Document representation
- Heap tree / priority queue
- Process structure
- Organization structure
- Decision trees in AI

---

# 12. 🌲 Binary Tree

**Source focus:** PDF pp. 145–150.

## Definition

A binary tree is a tree in which **every node has at most two children**.

The two children are distinguished as:

- **Left child**
- **Right child**

A binary tree is either:

- Empty, OR
- A root together with two disjoint binary trees:
  - left subtree
  - right subtree

> [!warning] MCQ trap
> **Binary ≠ exactly two children.**
>
> It means **at most two** children.
>
> A node may have 0, 1, or 2 children.

## Inorder traversal

The source sequence is:

```text
LEFT → ROOT → RIGHT
```

For the shown tree:

```text
        A
       / \
      B   C
     / \   \
    D   E   F
```

Inorder:

```text
D B E A C F
```

> [!tip] 🧠 Traversal mnemonic
> **INorder → root is IN the middle.**
>
> `L N R` / `Left → Node → Right`

---

# 13. 🔁 Binary Representation of a General Tree

The slide's mapping is:

- Root of the general tree → root of the binary tree.
- A **child** in the general tree → appears to the **left** of the node.
- A **sibling** → appears to the **right** of the node.

> [!warning] MCQ trap
> Do not confuse the mapping:
>
> **child → left**
>
> **sibling → right**

---

# 14. 🔎 Binary Search Tree (BST)

## Definition / insertion rule from the slides

Every node has a key.

For the convention explicitly used in the source:

```text
key < node key → left subtree
otherwise      → right subtree
```

So:

```text
        50
       /  \
     <50  >=50
```

## BST MCQ facts

### Least key

The node with the least key **cannot have a left subtree** under the source's ordering rule.

Why? Anything on its left would have to be smaller.

> [!tip] Fast recognition
> **Minimum BST key = keep going LEFT.**
>
> Therefore the minimum has **no left child/subtree**.

### Worst-case search

A BST can become skewed rather than balanced.

For `n` nodes, the worst-case number of comparisons can reach:

```text
O(n)
```

The source's quiz gives `n` as the worst-case comparison choice.

> [!danger] MCQ trap
> Do **not** automatically answer `log₂ n` for a BST.
>
> `log₂ n` corresponds to the balanced/ideal intuition; the source's question asks for the **worst case**, where a skewed BST can behave linearly.

## BST interface in the slides

```text
Create tree
Destroy tree
Insert key
Display in inorder
Find number of nodes
Find number of leaves
```

## Recursive routine model

The source emphasizes recursion **on nodes**, with the model:

```text
Empty tree
OR
Tree with left and right subtrees
```

---

# 15. 🧮 Tree MCQ Formula/Fact Box

| Fact | Remember |
|---|---|
| Tree with `n` nodes | **`n−1` edges** |
| Binary tree | **At most 2 children** |
| Inorder | **Left → Root → Right** |
| BST smaller key | **Left** |
| BST otherwise (source convention) | **Right** |
| Minimum key in BST | **No left subtree** |
| BST worst-case search | **`n` comparisons / O(n)** |
| Maximum leaves in binary tree | **`⌊(n+1)/2⌋`** |
| Longest root-to-leaf path in a binary tree with `n` nodes | **`n−1` edges** |

> [!note] Source-vs-inference note
> The last two numerical facts are reflected in the slide quizzes. They are standard structural consequences of binary-tree properties; use them as MCQ facts for this unit.

---

# 16. ↔️ Deque

**Source focus:** PDF pp. 152–158.

## Definition

A **deque** is a data structure that supports:

- Add at both ends.
- Delete at both ends.

Pronounced **“deck.”**

## Conceptual picture

```text
        add/delete
          ↕
FRONT ⇄ [ ... ] ⇄ REAR
          ↕
        add/delete
```

The key phrase for MCQs is:

> **Both ends.**

---

# 17. ♻️ Stack Implemented Using a Deque

The source demonstrates **composition** / reuse:

- Also described as embedding / layering / containment.
- It is a reuse mechanism.
- It avoids reinventing the wheel.

## Deque interface shown

```c
init()
disp_forward()
disp_backward()
add_in_begin()
add_at_end()
remove_in_begin()
remove_at_end()
is_empty()
is_full()
deinit()
```

## Stack interface shown

```c
push()
pop()
is_empty_stack()
is_full_stack()
init_stack()
deinit_stack()
```

## Mapping used by the source

For the shown stack implementation, the stack's top is represented through the deque's **beginning**:

```text
push → add_in_begin
pop  → remove_in_begin
```

The other stack operations delegate to the corresponding deque operations:

```text
is_empty_stack → is_empty
is_full_stack  → is_full
init_stack     → init
 deinit_stack  → deinit
```

Example from the slide:

```c
void push(stack_t *ptr_stack, int key)
{
    add_in_begin(&ptr_stack->list_, key);
}

int pop(stack_t *ptr_stack)
{
    return remove_in_begin(&ptr_stack->list_);
}
```

> [!tip] MCQ phrase
> **Stack using deque = composition + delegation.**

---

# 18. ⏱️ Round Robin Scheduling — Experiential Learning

**Source focus:** PDF pp. 160–163.

This section turns the earlier list concepts into a scheduling exercise.

## Task 1 — Circular doubly linked list

Starting input:

> Doubly linked list header + implementation.

Goal: make it **circular**.

Required behaviour:

- Add a node before the current position.
- Remove a node at the current position.
- Modify display routines for the circular structure.
- Build a menu-driven client.

## Task 2 — Replace `int` key with a `job`

- Create a type called `job`.
- Provide interfaces for setting/displaying a job.
- Change the node's key field from `int` to `job`.
- Modify the client accordingly.

## Task 3 — Scheduling

Each job has:

- Time required for completion.
- Time already allotted.

On processing a job:

1. Increase the time allotted by the time slice.
2. If allotted time **exceeds** the time for completion, remove the job.

> [!warning] Exact source wording matters
> The slide says **“exceeds”** the completion time. Keep the wording in mind if an MCQ is copied directly from this task.

### Why the data structure fits

Round Robin repeatedly cycles through runnable jobs, so the task naturally fits a **circular** structure.

---

# 19. 🧠 MASTER COMPARISON SHEET

## Lists

| Feature | Circular LL | Doubly LL |
|---|---|---|
| Last links to first? | **Yes** | No, in the described non-circular DLL |
| Backward movement | Not provided by singly `next_` link | **Yes, via `prev_`** |
| Termination idea | Return to start | `NULL` at ends |
| Empty indicator | `head_ == NULL` | `head_ == NULL` / `tail_ == NULL` |
| One-node special shape | `node->next_ == node` | `head == tail`, endpoints null where applicable |

## Priority queues

| Storage | Enqueue | Dequeue |
|---|---:|---:|
| Unordered list | **O(1)** | **O(n)** |
| Ordered list | **O(n)** | **O(1)** |
| Unordered array | **O(1)** | **O(n)** |
| Ordered array | **O(n)** | **O(1)** |

## Tree / BST

```text
Tree:        n nodes → n−1 edges
Binary tree: ≤ 2 children per node
Inorder:     L → Root → R
BST:         smaller → left, otherwise → right
Min BST key: no left subtree
BST worst:   O(n)
```

---

# 20. 🚨 MCQ KILLER SHEET — EXACT TRAPS

### Circular List

**Q:** How is traversal end detected?

→ Return to the **starting node**.

**Q:** What if you use `while (temp != NULL)` on a circular list?

→ It can become an **infinite loop** because traversal never reaches `NULL` through the circular links.

**Q:** One-node circular list?

→ `head_->next_ == head_`.

**Q:** Delete the only node?

→ `head_ = NULL`.

**Q:** Preferred traversal loop in the slides?

→ **`do...while`**.

---

### Doubly Linked List

**Q:** `prev_` of the first node in a non-circular DLL?

→ **NULL**.

**Q:** Insert `temp` immediately after `p` — first two key assignments?

→ `temp->next_ = p->next_` and `temp->prev_ = p`.

**Q:** What is the missing pointer update for a non-terminal insertion?

→ **`p->next_->prev_ = temp`**.

**Q:** Inserting into an empty DLL changes what?

→ **Both head and tail**.

**Q:** Deleting the only DLL node changes what?

→ **Both head and tail**.

**Q:** Does a DLL make search constant-time?

→ **No. Still linear (`n`).**

---

### Circular Array Queue

**Q:** Initial `front_` and `rear_` in this implementation?

→ **0 and 0**.

**Q:** `is_empty()` check?

→ `front_ == rear_`, tested **before front increment**.

**Q:** `is_full()` check?

→ Same expression, `front_ == rear_`, but tested **after rear increment**.

**Q:** Why can the same expression mean two things?

→ **Because the implementation tests it at different points in the operation.**

**Q:** Deinitialize with a loop?

→ **No.** Just reset the indices.

**Q:** Different component/key type — change what?

→ **The key/component type**, not front/rear.

---

### Priority Queue

**Q:** What decides deletion?

→ **Priority**.

**Q:** Is high numeric value always high priority?

→ **No.** The source explicitly says either higher or lower numeric value may represent higher priority.

**Q:** Priority queue = FIFO?

→ **No.** Priority controls processing/deletion.

**Q:** Possible implementations in the source?

→ Unordered list, ordered list, unordered array, ordered array, heap tree.

**Q:** Unordered list enqueue?

→ **O(1)** / a few operations.

**Q:** Unordered list dequeue?

→ **O(n)** / traverse to highest priority.

**Q:** Ordered list enqueue?

→ **O(n)**.

**Q:** Ordered list dequeue?

→ **O(1)**.

**Q:** Unordered array enqueue?

→ **O(1)**, add at end.

**Q:** Unordered array dequeue?

→ **O(n)**, scan + shift.

**Q:** Unordered-array dequeue shift direction?

→ **Right → left**.

**Q:** Ordered array enqueue?

→ **O(n)** due to traversal/shifting.

**Q:** Ordered array dequeue?

→ **O(1)**, rightmost element.

---

### Trees

**Q:** Tree with `n` nodes has how many edges?

→ **`n−1`**.

**Q:** Can a tree contain a cycle?

→ **No**.

**Q:** Binary tree means?

→ **At most two children**.

**Q:** Inorder?

→ **Left → Root → Right**.

**Q:** General tree → binary representation: child goes where?

→ **Left**.

**Q:** Sibling goes where?

→ **Right**.

**Q:** BST smaller key goes where?

→ **Left**.

**Q:** Minimum-key BST node has what property?

→ **No left subtree**.

**Q:** BST worst-case search comparisons?

→ **`n` / O(n)**.

**Q:** Maximum leaves in a binary tree with `n` nodes?

→ **`⌊(n+1)/2⌋`**.

**Q:** Longest root-to-leaf path in an arbitrary binary tree with `n` nodes?

→ **`n−1` edges**.

---

### Deque / Stack

**Q:** Deque supports insertion/deletion where?

→ **Both ends**.

**Q:** Pronunciation?

→ **“Deck.”**

**Q:** Stack implemented using deque illustrates what?

→ **Composition / embedding / layering / containment + delegation**.

**Q:** In the slide's mapping, push uses which deque operation?

→ **`add_in_begin()`**.

**Q:** Pop?

→ **`remove_in_begin()`**.

---

# 21. ⚡ 60-SECOND PRE-EXAM CRAM

```text
CIRCULAR LL
• Last → first
• NEVER use NULL as normal traversal terminator
• Prefer do...while; stop when back at start
• 1 node → next points to itself
• Delete only node → head = NULL
• Josephus = kth elimination in a circle

DLL
• prev + next
• backward movement = 1 step
• search still O(n)
• middle insertion = 4 pointer changes
• first insertion into empty list changes head + tail
• deleting only node changes head + tail

QUEUE (circular array implementation)
• front = rear = 0 at init
• enqueue moves rear
• dequeue moves front
• wrap using % MAXSIZE
• empty check and full check use same equality,
  BUT at different times
• deinit = reset indices, NO LOOP

PRIORITY QUEUE
• deletion is by priority
• numeric direction of priority is NOT universal
• unordered → cheap enqueue / expensive dequeue
• ordered   → expensive enqueue / cheap dequeue
• unordered list: O(1), O(n)
• ordered list:   O(n), O(1)
• unordered array: O(1), O(n)
• ordered array:   O(n), O(1)
• unordered-array dequeue shifts right → left

TREE
• tree is connected + acyclic
• n nodes → n−1 edges
• binary = ≤2 children
• inorder = LNR
• general-tree binary representation: child-left, sibling-right
• BST: smaller-left, otherwise-right
• minimum BST key has no left subtree
• BST worst search = O(n)

DEQUE
• insert/delete at both ends
• stack via deque = composition/delegation

ROUND ROBIN
• circular doubly linked list
• each job has completion time + allotted time
• each processing round adds one time slice
• if allotted time exceeds completion time → remove job
```

---

# 22. 🎯 MCQ Strategy — How to Attack the Question

> [!tip] 1. Look for the implementation first
> A question saying “priority queue” is incomplete for complexity. Ask:
> **unordered or ordered? list or array?**

> [!tip] 2. Watch the operation timing
> In the circular-array queue, `front_ == rear_` is deliberately reused for both empty/full tests. The trick is **when** the comparison happens.

> [!tip] 3. Treat words like “worst case” as a command
> BST + “worst case” → think **skewed tree → linear**.

> [!tip] 4. Pointer questions: draw only the local neighbourhood
> For insertion/deletion, draw `prev ↔ current ↔ next` and make sure **both directions** remain correct.

> [!tip] 5. “At most” ≠ “exactly”
> Binary tree → **at most 2** children.

> [!tip] 6. “Circular” should trigger a NULL alarm
> Normal end-of-list logic is not the right stopping condition. Return-to-start is the key idea.

> [!tip] 7. Do not import conventions from another implementation
> Queue questions often tempt you with `front = rear = -1`. The source's implementation uses **0**.

> [!tip] 8. Preserve source-specific wording
> For Round Robin, the slide says remove a job when allotted time **exceeds** completion time.

---

# 23. 📌 Source Map

| PDF pages | Topic |
|---|---|
| 1–20 | Circular List + Josephus |
| 21–41 | Doubly Linked List |
| 42–60 | Circular-array Queue |
| 61–78 | Priority Queue — concept |
| 79–99 | Priority Queue — unordered list |
| 100–124 | Priority Queue — other implementations |
| 125–151 | Trees, Binary Tree, BST |
| 152–159 | Deque + Stack using Deque |
| 160–164 | Round Robin Scheduling experiential learning |

---

# ✅ Final Memory Chain

```text
CIRCULAR → return to start
DLL      → prev + next; 4 pointer changes in middle insertion
QUEUE    → same equality, different timing
PQ       → priority decides deletion; order determines cost
TREE     → connected + acyclic; n−1 edges
BINARY   → ≤2 children; inorder = LNR
BST      → left smaller; worst case O(n)
DEQUE    → both ends
RR       → circular jobs + time slice
```

> [!success] Exam finish line
> **If you remember only one pattern from the entire unit:**
>
> **Structure determines where the work happens.**
>
> Unordered → work on **dequeue/delete**.
>
> Ordered → work on **enqueue/insert**.
>
> Circular → work with **return-to-start**, not `NULL`.
>
> Doubly linked → pointer operations become easier because you have **both directions**.
