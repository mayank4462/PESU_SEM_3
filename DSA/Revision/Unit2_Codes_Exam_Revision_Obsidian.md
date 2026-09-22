# Unit 2 — Data Structures Codes: Exam Revision

> [!abstract] How to use this sheet
> This is the **1-hour-before-exam reconstruction guide** for the Unit-2 code ZIP.
> 
> Every topic follows the pattern:
> **idea → structure → init/deinit → operation logic in English → exact C snippet → memory line → complexity → MCQ traps**
>
> The goal is **not** to memorize a 100-line program. Memorize the invariant + the 2–5 critical lines and rebuild the rest.

---

# Unit 2 Big Picture

| Folder | Main idea | Core representation |
|---|---|---|
| `Queue_Array` | Normal / linear queue | Array + `front`, `rear` |
| `Queue_List` | Queue using linked list | `front`, `rear` pointers |
| `CircularQueue_Array` | Circular queue | Array + `front`, `rear`, `count` |
| `CircularQueue_List` | Circular linked queue | `front`, `rear`, last node points to front |
| `Circular` | Circular linked list | **Only `tail` pointer** |
| `DoublyLinkedList` | Doubly linked list | `llink`, `data`, `rlink` |
| `PriorityQueue_Array` | Ordered priority queue | Sorted array of jobs |
| `PriorityQueue_List` | Ordered priority queue | Sorted linked list of jobs |
| `UnorderedPriorityQueue_Array` | Unordered priority queue | Unsorted array; search during dequeue |
| `UnorderedPriorityQueue_List` | Unordered priority queue | Unsorted linked list; search during dequeue |
| `Josephus` | Folder present in ZIP | **No source files in the supplied ZIP** |

---

# 1. Queue_Array

## 1.1 Basic idea

A queue follows:

> **FIFO = First In → First Out**

There are two indices:

- `front` → element to be removed next
- `rear` → most recently inserted element

This folder implements a **linear array queue**, not a circular one.

### Structure

```c
#define MAX 5

struct queue
{
    int q[MAX];
    int front;
    int rear;
};
```

### Remember the invariant

```text
front → first valid element
rear  → last inserted element
```

---

## 1.2 Initialization

The array starts empty.

### Logic

```text
front = 0
rear = -1
```

### Code

```c
void init(struct queue *ptr_queue)
{
    ptr_queue->front = 0;
    ptr_queue->rear = -1;
}
```

### MEMORY LINE

> **Linear Queue init → `front = 0`, `rear = -1`**

This is one of the most important Unit-2 lines.

---

## 1.3 Deinitialization

The ZIP simply resets the indices.

```c
void deinit(struct queue *ptr_queue)
{
    ptr_queue->front = 0;
    ptr_queue->rear = -1;
}
```

No dynamic memory exists here, so nothing needs to be `free()`d.

---

## 1.4 isEmpty()

Queue is empty when there is no valid element between `front` and `rear`.

### Logic

```text
If front > rear
    empty
```

### Code

```c
bool isEmpty(struct queue *ptr_queue)
{
    return ptr_queue->front > ptr_queue->rear;
}
```

### MEMORY LINE

> **Linear array queue empty → `front > rear`**

Example:

```text
front = 3
rear  = 2

3 > 2 → EMPTY
```

---

## 1.5 isFull()

Because this is a **linear** queue, the last usable array location is `MAX - 1`.

```c
bool isFull(struct queue *ptr_queue)
{
    return ptr_queue->rear == MAX - 1;
}
```

### MEMORY LINE

> **Linear array queue full → `rear == MAX - 1`**

---

## 1.6 ENQUE

Enqueue = insert at the rear.

### Logic

```text
Check if queue is NOT full

If not full:
    increment rear
    place key at q[rear]
```

### Code snippet

```c
if(!isFull(ptr_queue))
{
    ptr_queue->q[++ptr_queue->rear] = key;
}
```

### Read this as

```text
++rear
q[rear] = key
```

### MEMORY TRICK

> **Queue enqueue → same pattern as stack push for the insertion index: `++rear`**

---

## 1.7 DEQUE

Dequeue = remove from the front.

### Logic

```text
Check if queue is NOT empty

If not empty:
    take q[front]
    increment front
    return removed value
```

### Code

```c
int to_be_deleted = ptr_queue->q[ptr_queue->front++];
return to_be_deleted;
```

### Read this as

```text
value = q[front]
front++
```

### MEMORY TRICK

> **Queue dequeue → read `front`, then `front++`**

So:

```text
ENQUE → ++rear
DEQUE → front++
```

---

## 1.8 DISPLAY

Traverse from `front` to `rear`.

```c
for(int i = ptr_queue->front;
    i <= ptr_queue->rear;
    i++)
{
    printf("%d\n", ptr_queue->q[i]);
}
```

### MEMORY LINE

> **Queue display → `front → rear`**

---

## 1.9 The whole queue in 6 lines

```text
STRUCT
    q[MAX]
    front
    rear

INIT
    front = 0
    rear = -1

ENQUE
    q[++rear] = key

DEQUE
    q[front++]

EMPTY
    front > rear

FULL
    rear == MAX - 1
```

---

## 1.10 Major conceptual trap

Suppose:

```text
MAX = 5

enqueue: 10 20 30 40 50
dequeue: 10 20
```

Now:

```text
front = 2
rear  = 4
```

Positions `0` and `1` are free, **but this linear implementation does not reuse them**.

Another enqueue sees:

```text
rear == MAX - 1
```

and reports full.

> [!warning] MCQ TRAP
> A **linear array queue** can become “full” even when there are unused cells at the beginning.
>
> A **circular queue** is specifically used to reuse those cells.

---

## 1.11 Complexity

| Operation | Time |
|---|---:|
| Enqueue | `O(1)` |
| Dequeue | `O(1)` |
| Display | `O(n)` |
| isEmpty / isFull | `O(1)` |

---

# 2. Queue_List

## 2.1 Basic idea

Same FIFO queue, but now use linked nodes.

This avoids the fixed-array capacity restriction because nodes are dynamically allocated.

### Node

```c
struct node
{
    int key;
    struct node *next;
};
```

### Queue

```c
struct queue
{
    struct node *front;
    struct node *rear;
};
```

### Visual

```text
front
  ↓
[10] → [20] → [30] → NULL
                         ↑
                        rear
```

---

## 2.2 Initialization

Initially there are no nodes.

```c
void init(struct queue *ptr_queue)
{
    ptr_queue->front = ptr_queue->rear = NULL;
}
```

### MEMORY LINE

> **Linked queue init → `front = rear = NULL`**

---

## 2.3 Deinitialization

Because nodes were dynamically allocated, they must be freed.

### Logic

```text
While queue is not empty:
    dequeue one node

Finally:
    front = rear = NULL
```

### Code

```c
while(!isEmpty(ptr_queue))
{
    deque(ptr_queue);
}

ptr_queue->front = ptr_queue->rear = NULL;
```

---

## 2.4 createNode()

### Logic

```text
malloc a node
If malloc fails → return NULL
Store key
Set next = NULL
Return node
```

### Snippet

```c
struct node *newNode =
    (struct node *)malloc(sizeof(struct node));

newNode->key = key;
newNode->next = NULL;
```

### MEMORY LINE

> **Create node → malloc → fill data → `next = NULL`**

---

## 2.5 ENQUE

Insertion always happens at the **rear**.

There are TWO cases.

### Case 1 — Queue empty

```text
front == NULL / rear == NULL

Both front and rear must point to newNode.
```

```c
if(ptr_queue->rear == NULL)
{
    ptr_queue->rear = newNode;
    ptr_queue->front = newNode;
}
```

### Case 2 — Queue non-empty

```text
rear->next = newNode
rear = newNode
```

```c
else
{
    ptr_queue->rear->next = newNode;
    ptr_queue->rear = newNode;
}
```

### MEMORY TRICK

> **Enqueue in linked queue → attach after `rear`, then move `rear` forward.**

---

## 2.6 DEQUE

Delete from the **front**.

### Logic

```text
Save front's next
Save front's key
free old front
Move front to next

If front becomes NULL:
    rear must also become NULL
```

### Critical snippet

```c
struct node *temp = ptr_queue->front->next;
int value = ptr_queue->front->key;

free(ptr_queue->front);
ptr_queue->front = temp;
```

Then:

```c
if(ptr_queue->front == NULL)
{
    ptr_queue->rear = NULL;
}
```

### Why update `rear`?

Because deleting the last remaining node makes BOTH pointers `NULL`.

> [!warning] GOLDEN EDGE CASE
> **One-node queue:** `front == rear`.
>
> After dequeue:
>
> ```text
> front = NULL
> rear  = NULL
> ```

---

## 2.7 isEmpty()

```c
return ptr_queue->front == NULL;
```

### MEMORY LINE

> **Linked queue empty → `front == NULL`**

---

## 2.8 isFull()

This ZIP does something unusual.

It tries `malloc()`:

```c
struct node *newNode =
    (struct node *)malloc(sizeof(struct node));

if(newNode == NULL)
    return true;

free(newNode);
return false;
```

### Meaning

There is no fixed `MAX` capacity.

The code treats “cannot allocate another node” as full.

> [!warning] MCQ TRAP
> **Linked queue full is not based on `rear == MAX - 1`.**
>
> Here, `isFull()` effectively means:
>
> **“Can I allocate another node?”**

---

## 2.9 DISPLAY

Start from `front` and keep following `next`.

```c
struct node *temp = ptr_queue->front;

while(temp != NULL)
{
    printf("%d\n", temp->key);
    temp = temp->next;
}
```

### MEMORY LINE

> **Linked queue display → `front → NULL`**

---

## 2.10 Complexity

| Operation | Time |
|---|---:|
| Enqueue | `O(1)` |
| Dequeue | `O(1)` |
| Display | `O(n)` |
| isEmpty | `O(1)` |
| isFull | `O(1)` malloc attempt |

---

# 3. CircularQueue_Array

## 3.1 Why circular queue?

Linear array queue wastes positions at the beginning after dequeues.

Circular queue fixes this by **wrapping around**.

### Structure

Same basic array:

```c
struct queue
{
    int q[MAX];
    int front;
    int rear;
};
```

But this implementation additionally uses:

```c
int count = 0;
```

`count` tells us exactly how many elements exist.

---

## 3.2 Initialization

```c
ptr_queue->front = 0;
ptr_queue->rear = -1;
count = 0;
```

### MEMORY LINE

> **Circular queue → `front=0`, `rear=-1`, `count=0`**

---

## 3.3 ENQUE

The key circular formula is:

```c
rear = (rear + 1) % MAX;
```

Then:

```c
q[rear] = key;
count++;
```

### Complete core

```c
ptr_queue->rear =
    (ptr_queue->rear + 1) % MAX;

ptr_queue->q[ptr_queue->rear] = key;

count++;
```

### Why `% MAX`?

When `rear` reaches the end:

```text
MAX - 1 → 0
```

So the array wraps around.

### MEMORY TRICK

> **Circular move forward → `(index + 1) % MAX`**

---

## 3.4 DEQUE

Take the current front element:

```c
int value = ptr_queue->q[ptr_queue->front];
```

Then wrap `front`:

```c
ptr_queue->front =
    (ptr_queue->front + 1) % MAX;
```

And decrement count:

```c
count--;
```

### MEMORY TRICK

> **Circular dequeue → read `front`, then `(front+1)%MAX`, then `count--`.**

---

## 3.5 isEmpty()

```c
return count == 0;
```

## 3.6 isFull()

```c
return count == MAX;
```

### IMPORTANT

The code does **not** depend on `front == rear` for full/empty detection.

`count` removes that ambiguity.

---

## 3.7 DISPLAY

Start at `front`.

Print exactly `count` elements.

```c
int index = ptr_queue->front;

for(int i = 0; i < count; i++)
{
    printf("%d\n", ptr_queue->q[index]);

    index = (index + 1) % MAX;
}
```

### MEMORY LINE

> **Circular display → start at `front`, repeat `count` times, wrap using `% MAX`.**

---

## 3.8 The entire idea

```text
ENQUE
    rear = (rear + 1) % MAX
    q[rear] = key
    count++

DEQUE
    value = q[front]
    front = (front + 1) % MAX
    count--

EMPTY
    count == 0

FULL
    count == MAX
```

---

## 3.9 Linear vs Circular Queue

| Feature | Linear Array Queue | Circular Array Queue |
|---|---|---|
| Enqueue index | `++rear` | `(rear+1)%MAX` |
| Dequeue index | `front++` | `(front+1)%MAX` |
| Empty | `front > rear` | `count == 0` |
| Full | `rear == MAX-1` | `count == MAX` |
| Reuses freed front cells? | ❌ | ✅ |
| Wrap-around? | ❌ | ✅ |

> [!tip] MCQ MEMORY
> Seeing **`% MAX`** in queue code is a huge hint that it is **circular**.

---

# 4. CircularQueue_List

## 4.1 Basic idea

This is a linked-list queue where the last node points back to the first node.

```text
front
 ↓
[10] → [20] → [30]
 ↑             ↓
 └─────────────┘
               ↑
              rear
```

The queue stores:

```c
struct queue
{
    struct node *front;
    struct node *rear;
};
```

---

## 4.2 Node structure

```c
struct node
{
    int key;
    struct node *next;
};
```

---

## 4.3 Initialization

```c
ptr_queue->front = ptr_queue->rear = NULL;
```

---

## 4.4 ENQUE

### Case 1 — First node

Both pointers point to the new node.

But because the queue is circular:

```c
newNode->next = newNode;
```

### Core

```c
ptr_queue->rear = newNode;
ptr_queue->front = newNode;
newNode->next = newNode;
```

### Case 2 — Existing queue

Insert after rear and point new node back to front:

```c
ptr_queue->rear->next = newNode;
newNode->next = ptr_queue->front;
ptr_queue->rear = newNode;
```

### MEMORY TRICK

> **Circular linked queue enqueue:**
>
> `rear → newNode → front`

---

## 4.5 DEQUE

Delete from `front`.

### Case 1 — Only one node

Condition:

```c
ptr_queue->front == ptr_queue->rear
```

Then:

```c
free(ptr_queue->front);
ptr_queue->front = ptr_queue->rear = NULL;
```

### Case 2 — More than one

```text
save front->next
free old front
front = next
rear->next = front
```

### Critical snippet

```c
struct node *temp = ptr_queue->front->next;

free(ptr_queue->front);
ptr_queue->front = temp;

ptr_queue->rear->next = ptr_queue->front;
```

### MEMORY LINE

> **After deleting front in circular queue, reconnect `rear->next = front`.**

---

## 4.6 isEmpty()

```c
return ptr_queue->front == NULL;
```

---

## 4.7 isFull()

Again, this linked implementation checks whether `malloc()` works.

```c
struct node *newNode =
    (struct node *)malloc(sizeof(struct node));

if(newNode == NULL)
    return true;

free(newNode);
return false;
```

So there is no fixed queue size.

---

## 4.8 DISPLAY

Because there is no `NULL` after the final node, a normal:

```c
while(temp != NULL)
```

would loop forever.

Therefore the code uses **do-while**.

```c
struct node *temp = ptr_queue->front;

do
{
    printf("%d\n", temp->key);
    temp = temp->next;
}
while(temp != ptr_queue->front);
```

### GOLDEN MEMORY LINE

> **Circular linked traversal → stop when you come BACK to the starting node.**

---

## 4.9 Complexity

| Operation | Time |
|---|---:|
| Enqueue | `O(1)` |
| Dequeue | `O(1)` |
| Display | `O(n)` |
| isEmpty | `O(1)` |

---

# 5. Circular

## 5.1 Basic idea

This folder implements a **circular linked list**.

The clever part:

> You only need a **tail pointer**.

Because:

```text
tail->next = head
```

Therefore the head is always available as:

```c
list->tail->next
```

### Structure

```c
struct node
{
    int data;
    struct node *next;
};

struct linked_list
{
    struct node *tail;
};
```

---

## 5.2 Initialization

```c
list->tail = NULL;
```

---

## 5.3 Empty condition

```c
list->tail == NULL
```

### MEMORY LINE

> **Circular list stores only `tail`; head = `tail->next`.**

This is one of the biggest things to remember.

---

## 5.4 First-node creation

If list is empty:

```c
newNode->next = newNode;
list->tail = newNode;
```

So:

```text
      ┌──────┐
      ↓      │
    [10] ────┘
```

A single node points to itself.

---

# 5.5 INSERTION AT BEGINNING

### Logic

If empty:

```text
newNode->next = newNode
tail = newNode
```

Otherwise:

```text
newNode->next = head
tail->next = newNode
```

Remember:

```c
head = list->tail->next;
```

### Critical code

```c
newNode->next = list->tail->next;
list->tail->next = newNode;
```

### Important

`tail` does **not** change.

### MEMORY TRICK

> **Insert beginning in circular list → change `tail->next`, not `tail`.**

---

# 5.6 INSERTION AT END

### Logic

The new node goes after current tail.

```text
newNode->next = head
tail->next = newNode
tail = newNode
```

### Code

```c
newNode->next = list->tail->next;
list->tail->next = newNode;
list->tail = newNode;
```

### MEMORY TRICK

> **Insert end → first connect new node to head, then move `tail` to new node.**

---

# 5.7 DELETE AT BEGINNING

### Case 1 — Empty

```c
if(list->tail == NULL)
    return;
```

### Case 2 — One node

A one-node circular list satisfies:

```c
list->tail->next == list->tail
```

Then:

```c
free(list->tail);
list->tail = NULL;
```

### Case 3 — Multiple nodes

Head is:

```c
struct node *first = list->tail->next;
```

Then skip it:

```c
list->tail->next = first->next;
free(first);
```

### MEMORY TRICK

> **Circular delete beginning → save `tail->next`, bypass it, free it.**

---

# 5.8 DELETE AT END

The tail needs to be deleted.

But there is no previous pointer stored.

So traverse until:

```c
curr->next == list->tail
```

Then `curr` is the node before tail.

### Core

```c
struct node *curr = list->tail;

while(curr->next != list->tail)
{
    curr = curr->next;
}

curr->next = list->tail->next;
free(list->tail);
list->tail = curr;
```

### MEMORY LINE

> **Delete end → find node whose `next == tail`, reconnect it to head, move tail backward.**

---

# 5.9 TRAVERSAL

Since the list is circular, `NULL` never arrives.

Start from head:

```c
struct node *curr = list->tail->next;
```

Then use:

```c
do
{
    printf("%d ", curr->data);
    curr = curr->next;
}
while(curr != list->tail->next);
```

### MEMORY TRICK

> **Circular list traversal → `do...while`, stop when back at HEAD.**

---

# 5.10 Deinitialization

The code first breaks the circle:

```c
struct node *head = list->tail->next;
list->tail->next = NULL;
```

Now it has become an ordinary linear linked list ending in `NULL`.

Then free nodes one by one:

```c
while(curr != NULL)
{
    struct node *temp = curr;
    curr = curr->next;
    free(temp);
}
```

Finally:

```c
list->tail = NULL;
```

### VERY GOOD CONCEPTUAL TRICK

> **To safely destroy a circular list, first break the circle, then free like a normal linked list.**

---

# 6. DoublyLinkedList

## 6.1 Basic idea

Every node has BOTH directions:

```text
NULL ← [A] ⇄ [B] ⇄ [C] → NULL
```

### Structure

```c
struct node
{
    struct node *llink;
    int data;
    struct node *rlink;
};
```

`llink` = previous node  
`rlink` = next node

---

# 6.2 createNode()

A new isolated node has no neighbors:

```c
newNode->llink = NULL;
newNode->data = value;
newNode->rlink = NULL;
```

### MEMORY LINE

> **New doubly node → both links initially `NULL`.**

---

# 6.3 INIT

The code uses a pointer-to-pointer because it needs to modify the caller's `head`.

```c
void init(struct node **head)
{
    *head = NULL;
}
```

### Important distinction

If:

```c
struct node *head;
```

then:

```c
init(&head);
```

because the function needs the **address of head**.

---

# 6.4 INSERT AT BEGINNING

### Logic

```text
Create new node
newNode->rlink = old head

If old head exists:
    old head->llink = newNode

head = newNode
```

### Core code

```c
newNode->rlink = head;

if(head != NULL)
{
    head->llink = newNode;
}

head = newNode;
```

### MEMORY TRICK

> **Insert beginning: connect new → old head, then old head → new, then move head.**

---

# 6.5 INSERT AT END

### Logic

```text
If head is NULL:
    newNode itself becomes head

Else:
    traverse to last node
    last->rlink = newNode
    newNode->llink = last
```

### Core

```c
while(curr->rlink != NULL)
{
    curr = curr->rlink;
}

curr->rlink = newNode;
newNode->llink = curr;
```

### MEMORY LINE

> **At the end: `oldTail->rlink = new`, `new->llink = oldTail`.**

---

# 6.6 INSERT AFTER A KEY

Find the first node whose data equals `key`.

```c
while(curr != NULL && curr->data != key)
{
    curr = curr->rlink;
}
```

If not found:

```c
if(curr == NULL)
    return head;
```

Then insert after `curr`.

### Four-link logic

```text
newNode->rlink = curr->rlink
newNode->llink = curr

If curr has a next node:
    curr->rlink->llink = newNode

curr->rlink = newNode
```

### Exact core

```c
newNode->rlink = curr->rlink;
newNode->llink = curr;

if(curr->rlink != NULL)
{
    curr->rlink->llink = newNode;
}

curr->rlink = newNode;
```

### GOLDEN MEMORY

> **Inserting between two nodes in a DLL means fixing BOTH directions.**

---

# 6.7 DELETE AT BEGINNING

### Logic

```text
If empty → NULL

Save old head
Move head right

If new head exists:
    new head's llink = NULL

free old head
return new head
```

### Code

```c
struct node *temp = head;

head = head->rlink;

if(head != NULL)
{
    head->llink = NULL;
}

free(temp);
```

---

# 6.8 DELETE AT END

If only one node exists:

```c
if(head->rlink == NULL)
{
    return deleteAtBeginning(head);
}
```

Otherwise traverse to the final node.

```c
while(curr->rlink != NULL)
{
    curr = curr->rlink;
}
```

Then:

```c
curr->llink->rlink = NULL;
free(curr);
```

### MEMORY LINE

> **Delete last node → previous node's `rlink = NULL`, then free last.**

---

# 6.9 DELETE BY KEY

Find the node first:

```c
while(curr != NULL && curr->data != key)
{
    curr = curr->rlink;
}
```

If not found:

```c
if(curr == NULL)
    return head;
```

If deleting head:

```c
if(curr == head)
{
    return deleteAtBeginning(head);
}
```

Otherwise reconnect neighbors.

### Core

```c
if(curr->rlink != NULL)
{
    curr->rlink->llink = curr->llink;
}

curr->llink->rlink = curr->rlink;

free(curr);
```

### Mental picture

```text
A ⇄ X ⇄ B
```

Delete `X`:

```text
A ⇄ B
```

So:

```text
A->rlink = B
B->llink = A
```

---

# 6.10 DLL MASTER RULE

Whenever inserting/deleting in the middle:

> **Fix BOTH links.**

For middle insertion:

```text
new->llink
new->rlink
left->rlink
right->llink
```

For middle deletion:

```text
left->rlink = right
right->llink = left
```

---

# 6.11 Complexity

For this exact implementation:

| Operation | Time |
|---|---:|
| Insert beginning | `O(1)` |
| Insert end | `O(n)` |
| Insert after key | `O(n)` |
| Delete beginning | `O(1)` |
| Delete end | `O(n)` |
| Delete key | `O(n)` |

---

# 7. PriorityQueue_Array

## 7.1 What is a priority queue?

Each element is a `job`:

```c
struct job
{
    int job_no;
    int priority;
};
```

The queue is organized by priority rather than simple FIFO.

The ZIP's **ordered priority queue** keeps the array in **ascending priority order**.

That means:

```text
smallest priority value → front
largest priority value  → end
```

### Queue structure

```c
#define MAXSIZE 10

struct queue
{
    struct job q[MAXSIZE];
    int n;
};
```

`n` = number of jobs currently stored.

---

# 7.2 Job helper functions

## set_job()

```c
ptr_job->job_no = job_no;
ptr_job->priority = priority;
```

## get_priority()

```c
return ptr_job->priority;
```

## display_job()

Prints job number and priority.

### MEMORY LINE

> **Job module = create/set → display → read priority.**

---

# 7.3 Initialization

```c
ptr_queue->n = 0;
```

### MEMORY LINE

> **Array priority queue count → `n = 0`.**

---

# 7.4 isEmpty / isFull

```c
return ptr_queue->n == 0;
```

```c
return ptr_queue->n == MAXSIZE;
```

Very straightforward.

---

# 7.5 ENQUE — ordered insertion

This is the most important part.

We want the array sorted in ascending priority.

Suppose:

```text
[2] [5] [8]
```

and new priority = `4`.

We start from the right end and shift larger priorities right:

```text
[2] [5] [8]
       ↑
       shift 8
```

Then:

```text
[2] [4] [5] [8]
```

### Logic

```text
n = current number of jobs
j = n - 1
priority = new job priority

While j >= 0 AND existing priority > new priority:
    shift q[j] one position right
    j--

Place new job at q[j+1]
Increase n
```

### Core code

```c
int n = ptr_queue->n;
int j = n - 1;
int priority = get_priority(ptr_job);

while(j >= 0 &&
      get_priority(&ptr_queue->q[j]) > priority)
{
    ptr_queue->q[j+1] = ptr_queue->q[j];
    j--;
}

ptr_queue->q[j+1] = *ptr_job;
ptr_queue->n++;
```

### MEMORY TRICK

> **Ordered array enqueue = insertion sort step.**
>
> Start at **RIGHT**, shift larger elements RIGHT, insert into `j+1`.

---

# 7.6 Why `j+1`?

After shifting:

```text
j
```

has moved one position to the left of the correct location.

Therefore insertion goes at:

```c
q[j+1]
```

This is a classic MCQ/code-tracing point.

---

# 7.7 DEQUE

Because the smallest priority is stored at index `0`, dequeue removes:

```c
q[0]
```

First save it:

```c
struct job job_dltd = ptr_queue->q[0];
```

Then shift every remaining element left:

```c
for(int i = 0; i < ptr_queue->n - 1; i++)
{
    ptr_queue->q[i] = ptr_queue->q[i+1];
}
```

Finally:

```c
ptr_queue->n--;
```

### MEMORY LINE

> **Ordered array PQ dequeue → remove index `0`, shift left.**

---

# 7.8 Complexity

| Operation | Time |
|---|---:|
| Enqueue | `O(n)` |
| Dequeue | `O(n)` in this implementation |
| isEmpty | `O(1)` |
| isFull | `O(1)` |

> [!warning] Important
> Some textbooks describe an ordered priority queue where dequeue is `O(1)`.
> **This exact ZIP implementation shifts the array after dequeue, so its dequeue is `O(n)`.**
> For code-tracing questions, follow the actual code.

---

# 8. PriorityQueue_List

## 8.1 Basic idea

Same **ordered priority queue**, but use a linked list.

### Node

```c
struct node
{
    struct job job;
    struct node *next;
};
```

### Queue

```c
struct queue
{
    struct node *front;
    struct node *rear;
};
```

The list remains in **ascending priority order**.

---

# 8.2 Initialization

```c
ptr_queue->front = ptr_queue->rear = NULL;
```

---

# 8.3 createNode()

```c
newNode->job = *ptr_job;
newNode->next = NULL;
```

The job structure is copied into the new node.

---

# 8.4 ENQUE — ordered linked insertion

This is the central algorithm.

### Logic

```text
Create new node

If queue empty:
    front = rear = newNode

Else:
    start at front

    move while:
        current priority >= new priority

    Now insertion position is found

    If insertion is at beginning:
        newNode->next = front
        front = newNode

    Else:
        prev->next = newNode
        newNode->next = current

        If inserted at end:
            rear = newNode
```

### Search loop

```c
struct node *prev = NULL;
struct node *cur = ptr_queue->front;

while(cur != NULL &&
      get_priority(&cur->job) >=
      get_priority(&newNode->job))
{
    prev = cur;
    cur = cur->next;
}
```

### Insertion at beginning

```c
if(prev == NULL)
{
    newNode->next = ptr_queue->front;
    ptr_queue->front = newNode;
}
```

### Insertion in middle/end

```c
else
{
    prev->next = newNode;
    newNode->next = cur;

    if(cur == NULL)
    {
        ptr_queue->rear = newNode;
    }
}
```

---

# 8.5 HIGH-VALUE EQUAL-PRIORITY TRAP

Notice:

```c
get_priority(&cur->job) >=
get_priority(&newNode->job)
```

It is `>=`, not `>`.

Therefore the traversal moves **past existing equal-priority jobs**.

So among equal priorities, a newly inserted job goes **after existing equal-priority jobs**.

That gives FIFO-style stability among equal priorities.

> [!tip] MCQ GOLD
> `>=` in the traversal → **insert after equal-priority elements**.
>
> `>` would stop before equal-priority elements.

This tiny operator can completely change the answer.

---

# 8.6 DEQUE

Because the smallest priority is at `front`, dequeue is simply:

```text
save front job
move front
if front becomes NULL:
    rear = NULL
free old node
return job
```

### Core

```c
struct node *temp = ptr_queue->front;
struct job job_dltd = temp->job;

ptr_queue->front = temp->next;
```

Then handle the empty queue:

```c
if(ptr_queue->front == NULL)
{
    ptr_queue->rear = NULL;
}
```

Finally:

```c
free(temp);
return job_dltd;
```

### MEMORY LINE

> **Ordered linked PQ → insertion searches, dequeue is from front.**

---

# 8.7 Complexity

| Operation | Time |
|---|---:|
| Enqueue | `O(n)` |
| Dequeue | `O(1)` |
| isEmpty | `O(1)` |
| isFull | `O(1)` malloc attempt |

---

# 9. UnorderedPriorityQueue_Array

## 9.1 Basic idea

Unlike the ordered version, the array is **not sorted** after insertion.

Therefore enqueue is simple:

```text
put new job at q[n]
n++
```

Priority comparison happens during dequeue.

### Structure

```c
struct queue
{
    struct job q[MAXSIZE];
    int n;
};
```

---

# 9.2 ENQUE

```c
ptr_queue->q[ptr_queue->n] = *ptr_job;
ptr_queue->n++;
```

### MEMORY LINE

> **Unordered PQ enqueue → append; no searching, no shifting.**

---

# 9.3 DEQUE — search for best priority

The ZIP's **array unordered implementation selects the MINIMUM priority value**.

Start with:

```c
int index = 0;
```

Then scan:

```c
for(int i = 1; i < ptr_queue->n; i++)
{
    if(get_priority(&ptr_queue->q[i]) <
       get_priority(&ptr_queue->q[index]))
    {
        index = i;
    }
}
```

So the element with the smallest numeric priority becomes `index`.

### MEMORY TRICK

> **Unordered array PQ → scan whole queue during dequeue.**

---

# 9.4 Remove selected element

Save the job:

```c
struct job job_dltd = ptr_queue->q[index];
```

Then remove the hole by shifting later elements left:

```c
for(int i = index; i < ptr_queue->n - 1; i++)
{
    ptr_queue->q[i] = ptr_queue->q[i + 1];
}
```

Finally:

```c
ptr_queue->n--;
```

---

# 9.5 Complexity

| Operation | Time |
|---|---:|
| Enqueue | `O(1)` |
| Dequeue | `O(n)` |
| isEmpty | `O(1)` |
| isFull | `O(1)` |

---

# 9.6 Tie behavior

The comparison is:

```c
<
```

not `<=`.

Therefore if two jobs have equal minimum priority, the code keeps the **first one encountered**.

> [!tip] MCQ
> Strict `<` while searching for minimum → earliest minimum stays selected.

---

# 10. UnorderedPriorityQueue_List

## 10.1 Basic idea

Again, the list is **not sorted**.

### Enqueue

Always insert at the end like a normal queue:

```text
front → new nodes → rear
```

### Dequeue

Search the entire list for the required priority.

---

# 10.2 Structure

```c
struct node
{
    struct job job;
    struct node *next;
};

struct queue
{
    struct node *front;
    struct node *rear;
};
```

---

# 10.3 ENQUE

### Empty

```c
if(ptr_queue->rear == NULL)
{
    ptr_queue->front = ptr_queue->rear = newNode;
}
```

### Existing queue

```c
ptr_queue->rear->next = newNode;
ptr_queue->rear = newNode;
```

### MEMORY LINE

> **Unordered list PQ enqueue = normal linked queue enqueue.**

No priority comparison happens here.

---

# 10.4 DEQUE — find highest priority

Here is a VERY important ZIP-specific detail:

The unordered **list** implementation selects the **MAXIMUM priority value**.

It starts with:

```c
struct node *max = ptr_queue->front;
```

and compares:

```c
if(get_priority(&cur->job) >
   get_priority(&max->job))
{
    max = cur;
    prev_max = prev;
}
```

So:

```text
larger numeric priority → selected
```

### This is different from `UnorderedPriorityQueue_Array`.

Array version:

```c
<
```

→ selects MIN priority.

List version:

```c
>
```

→ selects MAX priority.

> [!danger] GOD-LEVEL CODE TRAP
> **DO NOT assume both unordered PQ implementations in this ZIP use the same priority direction.**
>
> `UnorderedPriorityQueue_Array` → **MIN priority**
>
> `UnorderedPriorityQueue_List` → **MAX priority**
>
> The comparison operator decides it.

---

# 10.5 Why `prev_max`?

If the node to delete is not the front, we need the previous node to bypass it.

During traversal:

```c
struct node *prev = NULL;
struct node *cur = ptr_queue->front;
```

Whenever a new max is found:

```c
prev_max = prev;
max = cur;
```

Then advance:

```c
prev = cur;
cur = cur->next;
```

---

# 10.6 Remove selected MAX node

### Case 1 — MAX is front

```c
ptr_queue->front =
    ptr_queue->front->next;
```

### Case 2 — MAX is elsewhere

```c
prev_max->next = max->next;
```

### Case 3 — MAX is rear

```c
if(max == ptr_queue->rear)
{
    ptr_queue->rear = prev_max;
}
```

Then:

```c
free(max);
```

### MEMORY TRICK

> **Deleting a middle/end linked-list node → you need the previous node to reconnect the chain.**

---

# 10.7 Complexity

| Operation | Time |
|---|---:|
| Enqueue | `O(1)` |
| Dequeue | `O(n)` |
| isEmpty | `O(1)` |
| isFull | `O(1)` malloc attempt |

---

# 11. The `job` Module — Common to Priority Queue Folders

Several priority-queue folders contain the same idea.

## Structure

```c
struct job
{
    int job_no;
    int priority;
};
```

## Set job

```c
void set_job(struct job *ptr_job,
             int job_no,
             int priority)
{
    ptr_job->job_no = job_no;
    ptr_job->priority = priority;
}
```

## Get priority

```c
int get_priority(struct job *ptr_job)
{
    return ptr_job->priority;
}
```

### Why is this helper useful?

The queue implementation does not directly depend on the internal field everywhere.

Instead it asks:

```c
get_priority(&job)
```

That keeps the priority comparison readable.

---

# 12. Client / `main()` Pattern in the ZIP

The queue folders use a similar menu-driven client.

Typical structure:

```text
Create queue
↓
init(&q)
↓
show menu
↓
read option
↓
while(option)
    switch(option)
        enqueue
        dequeue
        display
↓
deinit(&q)
```

### Priority queue client has one extra step

Before enqueue:

```text
read priority
↓
set_job(&jobx, job_no++, priority)
↓
enque(&q, &jobx)
```

### MEMORY LINE

> **Client = initialize → menu → operation → deinitialize**

Do not waste exam memory memorizing repeated `printf()` menu text unless explicitly asked.

---

# 13. Josephus

The supplied ZIP contains:

```text
Josephus/
```

but **no source files/content are present inside it**.

Therefore there is no implementation from this ZIP to convert into reconstruction notes.

> [!note]
> Do not invent a Josephus implementation and assume it is the one your faculty gave you. For this particular code set, the folder is empty.

---

# 14. MASTER COMPARISON — ALL QUEUES

## 14.1 Normal queues

| Implementation | Empty | Full | Enqueue | Dequeue |
|---|---|---|---|---|
| Array | `front > rear` | `rear == MAX-1` | `q[++rear]` | `q[front++]` |
| Linked list | `front == NULL` | malloc failure | after `rear` | remove `front` |
| Circular array | `count == 0` | `count == MAX` | `(rear+1)%MAX` | `(front+1)%MAX` |
| Circular list | `front == NULL` | malloc failure | after `rear`, link to front | remove `front` |

---

# 15. MASTER COMPARISON — PRIORITY QUEUES

| Type | Enqueue | Dequeue search? | Dequeue direction in ZIP | Main cost |
|---|---|---|---|---|
| Ordered Array | Insert in sorted order | No | Minimum at index `0` | Enqueue `O(n)` |
| Ordered List | Insert in sorted position | No | Minimum at `front` | Enqueue `O(n)` |
| Unordered Array | Append | Yes | **Minimum** | Dequeue `O(n)` |
| Unordered List | Append at rear | Yes | **Maximum** | Dequeue `O(n)` |

> [!danger] DO NOT MIX THESE UP
> The two unordered implementations in this ZIP are **not symmetric**:
>
> **Array unordered:** `<` → minimum priority
>
> **List unordered:** `>` → maximum priority

---

# 16. MASTER MEMORY MAP

## Queue

```text
FIFO

FRONT = remove from here
REAR  = insert here
```

### Linear Array

```text
init      → front=0, rear=-1
enqueue   → ++rear
dequeue   → front++
empty     → front > rear
full      → rear == MAX-1
```

### Linked Queue

```text
init      → front=rear=NULL
enqueue   → attach after rear
dequeue   → remove front
empty     → front==NULL
```

### Circular Array

```text
enqueue   → (rear+1)%MAX
dequeue   → (front+1)%MAX
empty     → count==0
full      → count==MAX
```

### Circular List

```text
rear->next = front

enqueue:
    rear → new → front

dequeue:
    remove front
    rear->next = new front
```

---

# 17. MASTER MEMORY MAP — LINKED LISTS

## Circular Singly Linked List

```text
Only tail stored

HEAD = tail->next
```

### Beginning insertion

```text
new->next = head
tail->next = new
```

### End insertion

```text
new->next = head
tail->next = new
tail = new
```

### Circular traversal

```c
do
{
    ...
}
while(curr != head);
```

---

## Doubly Linked List

```text
prev ⇄ current ⇄ next
```

### Insert between nodes

```text
new->llink = current
new->rlink = current->rlink

right->llink = new
current->rlink = new
```

### Delete current

```text
right->llink = left
left->rlink = right
free(current)
```

---

# 18. GOD-LEVEL MCQ MEMORY — UNIT 2

> [!danger] Read this section immediately before the exam.

## 18.1 Index direction cheats

### Stack-style insertion

```text
++top
```

### Queue insertion

```text
++rear
```

### Queue removal

```text
front++
```

Do not confuse:

```text
Stack POP      → top--
Queue DEQUEUE  → front++
```

---

## 18.2 Linear queue vs circular queue

### Linear

```c
rear == MAX - 1
```

means full.

### Circular

```c
rear = (rear + 1) % MAX
```

means wrap around.

> **Seeing `% MAX` in queue index movement = circular queue.**

---

## 18.3 The “front == rear” trap

Do NOT blindly use:

```text
front == rear
```

as the empty/full condition.

In this ZIP:

### Linear queue

```text
empty → front > rear
```

### Circular array

```text
empty → count == 0
full  → count == MAX
```

The meaning depends on the implementation.

---

## 18.4 The linear queue “fake full” trap

If:

```text
front > 0
rear == MAX-1
```

there may be free cells before `front`, but this **linear** queue still says full.

Why?

Because it does not wrap around.

---

## 18.5 Circular queue invariant

Always remember:

```text
next index = (current + 1) % MAX
```

Examples:

```text
4 → 0
0 → 1
1 → 2
```

for `MAX = 5`.

---

## 18.6 Circular linked list head

If only `tail` is stored:

```c
head = tail->next;
```

This is a MASSIVE MCQ target.

> **tail->next = head**

Not:

```text
head->next = tail
```

---

## 18.7 Circular traversal

Normal linked list:

```c
while(curr != NULL)
```

Circular list:

```c
do
{
    ...
}
while(curr != head);
```

> **No NULL terminator in a proper circular list.**

---

## 18.8 One-node circular list

For the circular list:

```text
tail->next == tail
```

For the circular queue:

```text
front == rear
```

These conditions identify the single-node case.

---

## 18.9 Doubly linked list = two directions

Node:

```c
llink | data | rlink
```

So every middle insertion/deletion usually requires **two neighboring links** to be updated.

### Insertion

```text
left → new
new → right
right → new
new → left
```

### Deletion

```text
left → right
right → left
```

---

## 18.10 Priority queue: WATCH THE COMPARATOR

This is perhaps the strongest Unit-2 MCQ trick.

### Ordered Array

```c
existing_priority > new_priority
```

→ larger elements shift right

→ array stays ascending

→ smallest priority at front.

### Ordered List

```c
existing_priority >= new_priority
```

→ new equal-priority job goes AFTER old equal-priority jobs.

### Unordered Array

```c
current < best
```

→ find **minimum** priority.

### Unordered List

```c
current > max
```

→ find **maximum** priority.

> [!danger] MEMORIZE THE OPERATOR, NOT JUST “priority queue”
>
> `>` / `>=` / `<` can completely change which job gets dequeued first.

---

## 18.11 Equal-priority trick

Ordered list uses:

```c
>=
```

during the search.

Therefore:

```text
same priority
↓
move past existing one
↓
new one inserted after it
```

So equal-priority jobs retain arrival order.

Think:

> **`>=` = “go past equals too.”**

---

## 18.12 Unordered PQ trick

### Ordered PQ

```text
Sorting happens during ENQUEUE.
```

### Unordered PQ

```text
Searching for best happens during DEQUEUE.
```

This gives the classic trade-off:

```text
ORDERED
enqueue expensive
dequeue easier

UNORDERED
enqueue easy
dequeue expensive
```

For this ZIP, remember that the unordered **array/list priority direction differs**, so check the comparator too.

---

## 18.13 Array priority queue `n`

In priority-array implementations:

```c
n
```

means:

> **number of jobs currently stored**

Not:

```text
last index
```

Therefore:

```text
last valid index = n - 1
```

This is why insertion starts:

```c
int j = n - 1;
```

and append in an unordered array is:

```c
q[n] = job;
n++;
```

---

## 18.14 Why `j+1` after shifting?

In ordered array insertion:

```c
while(j >= 0 && condition)
{
    q[j+1] = q[j];
    j--;
}
```

After the loop, `j` has moved one position before the insertion point.

Hence:

```c
q[j+1] = newJob;
```

> **If MCQ asks where the new element goes after that shift loop → `j+1`.**

---

## 18.15 Linked queue one-node deletion

When removing the only node:

```text
front becomes NULL
```

therefore:

```text
rear must ALSO become NULL
```

The ZIP explicitly handles this in both normal and circular linked queues.

---

## 18.16 Linked queue “full”

A linked queue does not have:

```text
rear == MAX-1
```

Instead the ZIP tests whether a new node can be allocated:

```c
malloc(...)
```

If allocation fails:

```text
isFull = true
```

This is an implementation-specific detail worth recognizing.

---

# 19. LAST 2-MINUTE CODE RECALL

## Normal Queue Array

```c
front = 0;
rear = -1;

q[++rear] = key;

value = q[front++];

empty → front > rear
full  → rear == MAX-1
```

## Circular Queue Array

```c
rear = (rear + 1) % MAX;
front = (front + 1) % MAX;

count++;

count--;

empty → count == 0
full  → count == MAX
```

## Linked Queue

```c
front = rear = NULL;

rear->next = newNode;
rear = newNode;

temp = front->next;
free(front);
front = temp;
```

## Circular List

```c
head = tail->next;

new->next = tail->next;
tail->next = new;
tail = new;       // only for insertion at end
```

## Doubly List

```c
new->llink = curr;
new->rlink = curr->rlink;

if(curr->rlink)
    curr->rlink->llink = new;

curr->rlink = new;
```

## Ordered Priority Array

```c
j = n - 1;

while(j >= 0 && old_priority > new_priority)
{
    q[j+1] = q[j];
    j--;
}

q[j+1] = newJob;
```

## Unordered Priority Array

```c
index = 0;

if(priority[i] < priority[index])
    index = i;
```

→ **MIN**

## Unordered Priority List

```c
if(priority[cur] > priority[max])
{
    max = cur;
    prev_max = prev;
}
```

→ **MAX**

---

# 20. FINAL EXAM MANTRA

> **QUEUE = FIFO**
>
> **FRONT removes**
>
> **REAR inserts**
>
> **LINEAR ARRAY → no wrap**
>
> **CIRCULAR ARRAY → `% MAX`**
>
> **LINKED QUEUE → front + rear**
>
> **CIRCULAR LIST → tail, and `tail->next` is head**
>
> **DOUBLY LIST → two links must be repaired**
>
> **ORDERED PQ → sort on insertion**
>
> **UNORDERED PQ → search on deletion**
>
> **AND ALWAYS CHECK THE COMPARISON OPERATOR.**
>
> `>` vs `<` is not decoration. **It decides which priority wins.**
