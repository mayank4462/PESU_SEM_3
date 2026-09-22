---
title: "UE25CS252A - Unit 1 - 30-Minute Revision + MCQ Master Sheet"
course: "Data Structures and its Applications (UE25CS252A)"
unit: "Unit 1"
source: "unit1_slides(4).pdf"
tags:
  - DSA
  - PESU
  - UE25CS252A
  - Unit1
  - MCQ
  - C
  - Revision
aliases:
  - "DSA Unit 1 30-Min Revision"
  - "UE25CS252A U1 MCQ Sheet"
---

# UE25CS252A - Unit 1
## 30-Minute Revision + MCQ Master Sheet

> [!abstract] What this note is
> **Goal:** revise the entire 224-page Unit 1 deck in ~30 minutes without rereading slide repetition.
>
> **Priority:** definitions → contrasts → code-trace traps → boundary cases → MCQ patterns.
>
> **Source:** `unit1_slides(4).pdf` (PES University, Unit 1). The source deck explicitly makes the first internal assessment objective-heavy: after Unit 1 + Unit 2, the ISA uses objective questions, with **10 one-mark + 5 two-mark questions per unit**.

---

# 0. ⏱️ The 30-Minute Route

| Time | Revise | What must stick |
|---|---|---|
| 0-3 min | Big picture | ADT vs DS, array vs list, stack basics |
| 3-7 min | Functions + pointers | pass-by-value, pointer arithmetic, alias/dangling/garbage |
| 7-10 min | Arrays | decay, O(1) access, row-major, `*p++`, one-past |
| 10-12 min | Structures | `.`, `->`, assignment, padding, self-reference |
| 12-15 min | Dynamic allocation | `malloc`, `free`, leak, dangling |
| 15-19 min | ADT + List | interface vs implementation, list properties |
| 19-23 min | Linked list + header node | sequential access, ordered insertion, header-node advantage |
| 23-26 min | Stack | LIFO, interface, implementation, push/pop |
| 26-28 min | Stack applications | bracket checking |
| 28-29 min | Infix/postfix | conversion + evaluation + operand order |
| 29-30 min | Polynomial + skiplist/multilist + traps | last-pass MCQ memory dump |

> [!tip] Revision rule
> **Do not reread examples passively.** For every code snippet, ask: **What points where? What gets changed? What becomes inaccessible? What gets dereferenced?**

---

# 1. 🧠 Unit 1 in One Mental Map

```text
C foundations
│
├── Functions
│   ├── pass by value
│   └── recursion
│
├── Pointers
│   ├── address
│   ├── arithmetic
│   ├── aliasing
│   ├── dangling pointer
│   └── garbage / memory leak
│
├── Arrays
│   ├── homogeneous
│   ├── fixed size
│   ├── random access
│   └── multidimensional / row-major
│
├── Structures
│   ├── heterogeneous members
│   ├── named fields
│   └── structure assignment
│
├── Dynamic allocation
│   ├── malloc
│   └── free
│
├── ADT
│   └── interface ≠ implementation
│
├── List
│   ├── sequential access
│   └── insertion / deletion
│
├── Linked list
│   ├── self-referential node
│   └── ordered insertion
│
├── Header-node list
│   └── fewer boundary cases
│
├── Stack
│   └── LIFO
│
├── Stack applications
│   ├── balanced brackets
│   └── expression conversion/evaluation
│
└── Extensions
    ├── polynomial via list
    ├── skiplist
    └── multilist / sparse matrix
```

---

# 2. 🔥 Functions - What MCQs Love

## Core rules

- C uses **parameter passing by value**.
- Arguments are **copied** to corresponding parameters during a function call.
- To change the caller's object through a parameter, pass a **pointer to that object** and dereference it inside the function.
- A function call creates an **activation record / stack frame** containing parameters, local variables, temporaries and return address.
- Returning from the function removes the activation record.
- Control transfers to the called function; after return, execution resumes at the instruction following the call.

## Recursion

- A function calling itself, directly or indirectly, is **recursion**.
- A recursive function needs an **escape hatch / base case**.
- The deck's key idea: **"if + recursion" can be equivalent to an iterative loop**.
- Recursion may take more time, but can be easier when the underlying data structure/problem is recursive.

### Why functions are useful

`Divide and conquer • Sharing • Maintenance • Reliability • Reuse`

> [!tip] MCQ trigger
> **"C supports pass by reference"** → treat as **false** in this course's framing. C passes values; a pointer value can be passed so the pointed-to object can be modified.

---

# 3. 🎯 Pointers - The Trap Zone

## Definition

A pointer **holds the address of another entity**.

A pointer:
- is **not the same thing as an integer**;
- has a type (`int *` is not the same type as `double *`);
- supports pointer arithmetic.

## Pointer arithmetic taught in the deck

| Operation | Result |
|---|---|
| `p + k` | pointer |
| `p - k` | pointer |
| `p - q` | integer (distance between compatible pointers) |

> [!danger] High-yield trap
> Pointer arithmetic moves in **elements of the pointed-to type**, not raw bytes.

## Aliasing

**Aliasing = two pointers refer to the same location.**

```c
int *p = ...;
int *q = p;
```

Now `p` and `q` are aliases. Changing the object through one is visible through the other.

## Dangling pointer vs garbage

| Concept | Meaning |
|---|---|
| **Dangling pointer** | pointer value exists, but the referenced location is no longer valid/available |
| **Garbage** | allocated location still exists, but the program has lost access to it |
| **Memory leak** | garbage created by lost dynamic memory that cannot be reclaimed by the program |

The deck explicitly distinguishes these: dereferencing a dangling pointer is **undefined behaviour**; C does not provide garbage collection.

### 🚨 The single best memory trick

```text
DANGLING = I still have the pointer, but the place is dead.
GARBAGE  = the place exists, but I lost the pointer.
```

### Classic trace

```c
int *p = malloc(sizeof(int));
*p = 111;
```

If the function returns without `free(p)` and `p` disappears, the allocated block becomes **garbage → memory leak**.

---

# 4. 📦 Arrays

## Core properties

- Array has a fixed number of elements.
- Elements are **homogeneous** (same type).
- Size is specified when created.
- Ordinary array size is fixed; it **cannot grow or shrink**.
- Indexing is **zero-based**.
- Array supports **random/direct access**.
- Access time does not depend on the element's position.

The source describes array access as random access and constant with respect to position.

## Array "decay" idea used by the professor

The deck's wording:
- at compile time, an array is an array and knows its size;
- at runtime, it **degenerates to a pointer** in expressions/function passing;
- the runtime size is not stored with the pointer value.

This is why C does not automatically bounds-check array accesses.

> [!warning] Don't overgeneralize
> `sizeof(array)` and `sizeof(pointer)` are **not** the same when `array` is an actual array object.

## The `*p++` trap

```c
int a[] = {10,20,30,40,50};
int *p = a + 2;
int x = *p++;
```

Evaluation:

```text
x = *p     -> 30
p = p + 1  -> points to a[3]
```

### Memorize

```text
*p++  = *(p++)
```

Not `(*p)++`.

> [!tip] MCQ shortcut
> **Postfix `++` binds to the pointer expression first.** Dereference happens using the old pointer value.

## Array comparison

```c
int a[] = {1,2,3};
int b[] = {1,2,3};
```

`a == b` does **not** compare element-by-element contents. In an expression, the array names decay to pointers, so this compares addresses; distinct arrays are at different locations.

> [!tip] MCQ trigger
> **Same contents ≠ same array address.**

## One-past-the-end pointer

```c
int *p = a + 5;
```

For an array of 5 elements, `a + 5` is a valid **one-past-the-end pointer**.

It may be used for comparison/subtraction/iteration boundaries, but **must not be dereferenced**.

Hence:

```c
p - a = 5
```

> [!danger] Killer distinction
> **Valid pointer value** does NOT mean **valid object access**.

## 2D arrays

C stores a 2D array in **row-major order**.

For:

```c
int a[R][C];
```

0-based linear position of `a[i][j]` is:

```text
position = i*C + j
```

Example from the deck:

```text
int a[5][4];
a[3][2] -> 3*4 + 2 = 14
```

### Function parameter trap

```c
void foo(int a[][10]);
```

This accepts a 2D array whose row width is **10 columns**.

The source also gives:

```c
void bar(int n, int a[][n]);
```

Here the number of columns is supplied through the first parameter.

> [!tip] MCQ trigger
> For a 2D array parameter, the **column dimension matters** for address calculation. The source explicitly says the first dimension can be omitted, while the needed later dimension must be known.

---

# 5. 🧱 Structures

## Structure vs array

| Array | Structure |
|---|---|
| homogeneous | generally heterogeneous |
| elements selected by index | members selected by name |
| index-based | field/member-based |
| array name decays to pointer in expressions | structure variable remains a structure value |
| array assignment is not allowed | structure assignment is allowed |

The source describes structures as having named, generally heterogeneous components and notes random access to fields.

## Anonymous struct vs `typedef`

```c
struct { int a; double b; } x;
```

- `x` is a **variable**.
- The structure type is anonymous.

```c
typedef struct { int a; double b; } x;
```

- `x` is a **type name / alias**.

### Access operators

```c
obj.member
ptr->member
```

Equivalent pointer form:

```c
ptr->member
```

means accessing a member through a pointer.

## Structure assignment

```c
struct what { int a; double b; } a = {1279, 3.14}, b;
b = a;
```

Structures **can be assigned**. Conceptually, the members are copied **field-by-field**.

## Structure size / padding

Do **not** blindly add member sizes.

The deck says structure size depends on implementation and is at least the sum of member sizes. Padding/alignment can make the actual size larger.

Classic quiz pattern:

```c
struct what { char ch; int n; };
```

Even with `char = 1 byte` and `int = 4 bytes`, the structure need not be 5 bytes; padding can be present.

> [!danger] MCQ phrase
> **"Depends on implementation"** is the deck's intended trap for structure size questions.

---

# 6. 🧨 Dynamic Memory Allocation

C uses library functions rather than a special allocation operator.

## `malloc`

```c
malloc(number_of_bytes)
```

Key points from the deck:
- returns a dynamically allocated block;
- caller requests the number of **bytes**;
- returns `NULL` on failure;
- allocation bookkeeping stores block-size information;
- the returned `void *` can be converted to a desired pointer type.

Typical pattern:

```c
int *p = (int *)malloc(sizeof(int));
```

## `free`

```c
free(ptr);
```

- releases the block pointed to by `ptr`;
- the size does not need to be supplied again;
- `free(NULL)` does nothing;
- freeing something that was not returned by `malloc`, or freeing the same block twice, is **undefined behaviour**.

The deck explicitly calls out the second `free` and invalid-pointer `free` cases as undefined behaviour.

## Aliasing + `free`

```c
int *p = malloc(sizeof(int));
*p = 100;
int *q = p;
free(q);
```

`p` and `q` were aliases. `free(q)` frees the same allocation.

After `free(q)`:

```text
q -> dangling
p -> dangling
```

Dereferencing `p` is undefined behaviour.

> [!tip] MCQ killer rule
> **`free()` acts on the allocated block, not on the variable name.** Any alias to that block is affected.

## Lost pointer = garbage / leak

```c
p = p->next;
```

If `p` was the **only** pointer to an allocated first node, moving `p` forward can make the first node unreachable.

That first allocated node becomes **garbage**, hence a **memory leak** in C.

---

# 7. 🧩 Abstract Data Type (ADT)

## Definition to memorize

An **ADT specifies what operations are available and what they do, not how they are implemented.**

```text
ADT = INTERFACE / BEHAVIOUR
     NOT implementation details
```

The deck emphasizes:
- interface, not implementation;
- user and implementor depend on the interface;
- implementation can change without affecting the user;
- this is presented as **inversion of dependency**.

The source explicitly states that an ADT specifies operations/behaviour and the interface rather than implementation.

## Type concept

A type tells us:
1. **what values are legal**;
2. **what operations are possible**;
3. but **not how those operations are implemented**.

### C vs Python framing from the deck

| C | Python |
|---|---|
| variable type declared | type associated with value |
| type determined at compile time | type is dynamic |
| declared type does not change | variable can receive values of different types |

## Data structure

The deck's definition:

> A **data structure is an ADT** with components arranged suitably to achieve some goal.

Do not confuse:

```text
ADT -> what / interface
Implementation -> how
Data structure -> organized components used to achieve a goal
```

---

# 8. 📋 List - The Core ADT

A **list**:
- is a data structure;
- is an ADT;
- has components at **ordinal positions**;
- supports **sequential access**;
- supports insertion/deletion;
- may be ordered according to a key;
- can be implemented in more than one way, e.g. linked list or array.

## Array vs list - MCQ comparison

| Property | Array | Linked-list style list |
|---|---|---|
| Access | direct/random | sequential |
| Access to i-th element | fast | requires traversal |
| Insert/delete | generally costly because positions/elements may shift | can require only a few pointer operations once position is known |
| Growth | fixed-size array | can grow via nodes |

> [!tip] Exact idea behind many list MCQs
> **"Sequential access" is the phrase that kills the "direct i-th element" option.**

### Ordinal position

Every list element has a **position**.

A list may be **empty**.

A list can be modified; it is not intrinsically fixed-size like an ordinary array.

---

# 9. 🔗 Linked List

## Self-referential structure

A linked-list node is a structure that contains a pointer to another object of the **same structure type**.

```c
struct node
{
    int key_;
    struct node *next_;
};
```

This is valid because the member is a **pointer**, so its size is known without recursively embedding an infinite-size object.

### The invalid version

```c
struct node
{
    int key_;
    struct node next;
};
```

This attempts to contain another complete `struct node` inside itself, leading to an infinitely recursive object size. It does not compile.

> [!danger] MCQ memory
> **Self-reference = pointer to same struct, NOT a struct object of the same type.**

## Typical list representation from the deck

```c
struct node
{
    int key_;
    struct node *next_;
};

struct mylist
{
    node_t *head_;
};
```

The list stores a `head_` pointer to the first node.

## Traversal

```c
void disp(node_t *q)
{
    while(q != NULL)
    {
        printf("%d ", q->key_);
        q = q->next_;
    }
}
```

### Why `q = q->next_`, not `q++`?

Because linked-list nodes are **not assumed to be adjacent in memory**.

`q++` moves to the next object in pointer arithmetic; it does not follow the linked structure.

> [!tip] Killer phrase
> **Array traversal = address arithmetic. Linked-list traversal = pointer following.**

## Access complexity idea

For `n` nodes, the last node may require traversing essentially all `n` nodes.

```text
First node -> little/no traversal
Last node  -> up to n nodes traversed
```

Translated into complexity language:

```text
Access by position in linked list: O(n)
```

---

# 10. 🧮 Ordered Linked List

An **ordered list** stores nodes in key order.

Insertion must place the new node at the correct location.

### Boundary cases

1. empty list
2. insert at beginning
3. insert in middle
4. insert at end

The deck explicitly teaches these as insertion boundary conditions.

## Generic ordered insertion logic

```c
node_t *prev = NULL;
node_t *pres = ptr_list->head_;

while(pres != NULL && pres->key_ < temp->key_)
{
    prev = pres;
    pres = pres->next_;
}
```

At the end:

```text
prev -> node after which insertion happens
pres -> node before which insertion happens
```

Then:

```c
if(prev == NULL)
{
    temp->next_ = pres;
    ptr_list->head_ = temp;
}
else
{
    temp->next_ = pres;
    prev->next_ = temp;
}
```

### MCQ pointer picture

```text
Before:
prev -> pres

Insert temp:
prev -> temp -> pres
```

The same middle/end pointer arrangement works once the insertion point is found.

> [!tip] Why a trailing pointer?
> A singly linked list cannot move backward. `prev` remembers the node immediately before `pres` so its `next_` pointer can be changed.

---

# 11. 🪪 Header Node / Dummy Node List

A **header node** is a dummy node inserted before the actual data nodes.

Conceptually:

```text
head_
  |
  v
[DUMMY] -> [data] -> [data] -> NULL
```

## Why it is useful

The deck's central point:

> **The header node makes insertion simpler.**

Why?
- the list never has the true "head pointer becomes the new data node" special case;
- insertion at the beginning becomes structurally like any other insertion after the header;
- deletion logic can also be simplified because there is always a predecessor node for the first real element;
- the client sees the same interface while the implementation changes internally.

## Count field

A list structure may contain a `count_` field tracking the number of components.

Then:

```text
insert -> count_++
delete  -> count_--
```

Finding the size no longer needs a full traversal.

> [!tip] MCQ trap
> A cached `count_` is useful **only if every insertion/deletion keeps it updated**.

## Important header-node ideas

| Question | Think |
|---|---|
| Is header a real user element? | No, dummy/sentinel node |
| Does it represent list data? | No |
| Main benefit? | simplify boundary cases |
| Does client interface need to change? | No |
| Does insertion at beginning need a special head change? | Not in the same way; header is already first |

---

# 12. 🥞 Stack

## Definition

A stack:
- is a data structure;
- has zero or more components;
- operates at **one end**, called the **top**;
- follows **LIFO** = Last In, First Out;
- does not support ordinary traversal as a list does.

```text
        TOP
         |
       [ 40 ] <- pop here
       [ 30 ]
       [ 20 ]
       [ 10 ]
```

## Interface

| Operation | Meaning |
|---|---|
| Create | create stack |
| Destroy | destroy stack |
| Push | add at top |
| Pop | remove from top |
| Peek | inspect top without removing |
| Is_empty | check empty |
| Is_full | check full |

## Implementations taught

A stack can be implemented using:
- **array**
- **linked list**

> [!tip] ADT connection
> The stack **interface stays the same** even when the implementation changes from array to linked list.

## Array-stack push trap

The deck's quiz uses the convention:

```text
use current top index
store value
then increase top
```

So conceptually:

```text
top -> insertion location
push -> write at top -> top changes
```

The exact initial `top` convention depends on the implementation, so in MCQs follow the convention given in the code/question.

## Full stack

A full stack does not have one universal "correct" user-facing action in the deck's quiz. The user/client decides what action is appropriate.

---

# 13. 🧷 Stack Application - Well-Formed Parentheses

## Why counting alone fails

Equal counts of `(` and `)` are **necessary but not sufficient**.

Example:

```text
)(
```

Counts match, but the first character is an unmatched closing parenthesis.

Another invalid example from the deck:

```text
())(())
```

## Algorithm: only `()`

```text
1. Start with empty stack.
2. Scan the expression left to right.
3. If '(' -> push a marker.
4. If ')' -> pop.
   If the stack is empty before the pop -> invalid.
5. After the entire input:
   stack empty -> valid
   stack non-empty -> invalid
```

### Two different failure moments

| Failure | Meaning |
|---|---|
| Closing bracket arrives when stack is empty | too many closing brackets / invalid immediately |
| Input ends while stack is non-empty | unmatched opening brackets remain |

> [!tip] MCQ pair to memorize
> **Extra right bracket -> pop from empty stack.**
>
> **Extra left bracket -> non-empty stack at the end.**

## Multiple bracket types

For `()`, `[]`, `{}`:

When opening:

```text
( -> push ')'
[ -> push ']'
{ -> push '}'
```

When closing:

```text
stack must not be empty
closing symbol must match stack top
```

Examples from the deck:

```text
Well formed:    (){}[]   ([]{})   ([{}])
Not well formed: ([)]    ([]])    (][)
```

### Killer insight

With multiple bracket types, a single generic placeholder is **not enough** because nesting order/type matters.

---

# 14. 🔄 Infix vs Postfix

## Notation

### Infix
Operator is between operands:

```text
3 + 4
```

### Postfix
Operator comes after operands:

```text
3 4 +
```

## Why postfix is useful

Postfix removes the need to apply precedence rules during evaluation.

A **stack of operands** is sufficient for postfix evaluation.

Examples from the deck:

```text
3 + 4                  -> 3 4 +
5 * 6 + 7              -> 5 6 * 7 +
5 + 6 * 7              -> 5 6 7 * +
(3 + 4) * (5 + 6)      -> 3 4 + 5 6 + *
```

> [!tip] MCQ trigger
> **Postfix contains no parentheses.**

---

# 15. 🧮 Postfix Evaluation - The #1 Expression MCQ Trap

Algorithm:

```text
Read left to right.

Operand -> PUSH
Operator -> POP required operands, compute, PUSH result

At the end -> exactly one result remains
```

## Operand order is critical

For an operator such as subtraction or division:

```text
val2 = first pop
val1 = second pop
result = val1 operator val2
```

Example:

```text
8 3 -
```

Stack before `-`:

```text
8
3 <- top
```

Then:

```text
val2 = 3
val1 = 8
8 - 3 = 5
```

> [!danger] Never reverse the pops
> **First pop = RHS (`val2`)**
>
> **Second pop = LHS (`val1`)**

For `+` and `*`, reversing operands gives the same arithmetic result; for `-` and `/`, it does not.

---

# 16. 🛠️ Infix -> Postfix Conversion

The deck uses a **stack of operators**.

Core rules:

```text
Operand -> send directly to output
Operator -> compare precedence with stack top
Higher-priority operator can wait on the stack
Lower/equal-priority situation -> pop appropriate operators, then push current
End of input -> pop remaining operators
```

## Parentheses

- A left parenthesis is pushed onto the operator stack.
- It acts as a **barrier** for operators inside it.
- A right parenthesis causes operators to be popped until the matching left parenthesis is found.
- The parentheses themselves do **not** appear in postfix output.

Example from the deck:

```text
3*(4+5)*6
-> 345+*6*
```

## Assignment MCQ

The deck asks for:

```text
a = b = c
```

The postfix form used by the quiz is:

```text
a b c = =
```

So think **right-associative assignment** when this exact style appears.

---

# 17. 🧪 Experiential Learning - Polynomial via List

The source's polynomial activity is mainly about using the list abstraction.

## Term type

A polynomial term has:

```text
coefficient + exponent
```

The term type is called `term_t` in the exercise.

## Required tasks

| Task | Core idea |
|---|---|
| Create polynomial | ordered list of terms, decreasing exponent |
| Evaluate | evaluate polynomial for a given variable value |
| Copy | create destination list from source list |
| Differentiate | transform each source term and build destination list |

### MCQ/implementation memory

> **Polynomial = ordered list of terms.**
>
> The source explicitly asks for comparison based on the **exponent** and ordering in **decreasing exponent**.

---

# 18. 🚄 Skiplist

## Intuition

A normal singly linked list may require walking almost the entire list to find a late element.

A **skiplist** adds multiple levels of forward pointers so higher levels can jump over large portions of the list.

```text
Level 2: 10 --------> 40 --------> 70 ----> 100
Level 1: 10 ---> 20 ---> 40 ---> 50 ---> 70 ---> 100
Level 0: 10 -> 20 -> 30 -> 40 -> 50 -> 60 -> 70 -> 80 -> 90 -> 100
```

## Key facts

- Bottom level contains **all elements in sorted order**.
- Higher levels contain fewer nodes and act as shortcuts.
- A node has active pointers for the levels in which it participates.
- The number of pointers in a node depends on the number of levels it participates in / levels planned.
- Search/insertion/deletion use a top-down shortcut idea.
- The deck notes that implementation of skiplist is **not part of the stated syllabus** even though its concept and salient points are shown.

> [!tip] MCQ one-liner
> **Skiplist = sorted linked list + multiple levels of forward shortcuts.**

---

# 19. 🕸️ Multilist

## Definition

A **multilist** is a list of lists.

A physical node may participate in **more than one logical list**.

That means the same data can be reached through different relationships/views.

### Example idea from the deck

Students and courses:

```text
Student -> courses taken
Course  -> students enrolled
```

Instead of storing duplicate copies of the relationship, a multilist allows multiple logical traversals through shared physical data.

## Applications listed in the deck

- sparse matrix
- graph representation
- social networks
- multiple views in files

## Salient points

```text
+ multiple relationships
+ avoid duplicate data
+ avoid multiple independent updates
- more complicated pointer management
- more complicated insertion/deletion
```

---

# 20. 🧮 Sparse Matrix Connection

A large sparse matrix may contain mostly zeros.

Instead of storing every cell, store only the **non-zero elements** together with row/column information.

The deck connects this to multilists so that both row-wise and column-wise traversal can be supported.

> [!tip] MCQ trigger
> **Sparse matrix = use less storage when most entries are zero.**
>
> The deck frames this as a space-saving representation that trades storage structure complexity for efficiency.

---

# 21. ⚡ Mega MCQ Trap Sheet

## Pointer / memory

| Trap | Correct mental rule |
|---|---|
| Array and pointer have same `sizeof` | ❌ actual array `sizeof` is total array size; pointer `sizeof` is pointer size |
| `a == b` means same contents | ❌ compares addresses after array-to-pointer conversion |
| `a + n` for an array of `n` elements is invalid | ❌ pointer value one-past-end is valid |
| one-past pointer may be dereferenced | ❌ never |
| two pointers assigned `q = p` | ✅ aliasing |
| `free(q)` only affects `q` variable | ❌ it releases the shared allocation |
| dangling pointer = memory leak | ❌ dangling and garbage are different concepts |
| garbage = pointer with invalid target | ❌ garbage is allocated memory with no accessible pointer |
| `p = p->next` cannot cause leak | ❌ it can lose the only pointer to the previous node |

## Arrays

| Question pattern | Instant answer |
|---|---|
| random access? | ✅ yes |
| index starts at? | ✅ 0 |
| homogeneous? | ✅ yes |
| can grow/shrink automatically? | ❌ no |
| `*p++` | `*(p++)` |
| `p-a` where `p=a+k` | `k` |
| `a[i][j]` 2D location | row-major: `i*columns + j` |
| array out-of-bounds checked automatically? | ❌ C does not do runtime bounds checking |

## Structures

| Pattern | Answer |
|---|---|
| anonymous struct declaration ending in `x` | `x` is variable |
| `typedef struct {...} x;` | `x` is a type alias |
| `obj.field` | object access |
| `ptr->field` | pointer access |
| structure assignment | ✅ allowed |
| structure with `char + int` size | may exceed simple sum due to implementation/alignment/padding |
| direct self-containing `struct node next;` | ❌ invalid recursive size |
| `struct node *next;` | ✅ self-reference pattern |

## ADT / list

| Trap | Correct rule |
|---|---|
| ADT specifies implementation | ❌ interface/behaviour |
| list must be non-empty | ❌ may be empty |
| list gives direct i-th access | ❌ linked-list style access is sequential |
| every list element has a position | ✅ |
| list has only one possible implementation | ❌ can have multiple implementations |
| header node is a real data item | ❌ dummy/sentinel |
| header node changes client interface | ❌ implementation can change independently |

## Stack

| Trap | Correct rule |
|---|---|
| stack supports indexing | ❌ |
| stack rule | ✅ LIFO |
| push location | ✅ top |
| pop location | ✅ top |
| peek removes element | ❌ |
| implementation only with array | ❌ array or linked list |
| stack full means program must always abort | ❌ client decides action in deck's quiz |

## Bracket checking

```text
Extra ')'  -> stack empty when trying to pop
Extra '('  -> stack non-empty after full scan
Different bracket types -> top must match the expected closer
Counting only -> NOT enough
```

## Postfix

```text
Postfix has no parentheses.
Operand -> push.
Operator -> pop operands, compute, push result.
First pop -> val2 / RHS.
Second pop -> val1 / LHS.
Final stack -> exactly one result.
```

## Skiplist / multilist

```text
Skiplist -> sorted linked list + multiple shortcut levels.
Bottom skiplist level -> all sorted elements.
Higher level -> fewer nodes / shortcuts.
Multilist -> physical nodes may belong to multiple logical lists.
```

---

# 22. 🧠 "Spot the Correct Option" Patterns

> [!tip] Words that deserve suspicion
> **always / never / only / exactly / must / cannot**
>
> These are often where MCQ distractors overstate a rule. Check the boundary case before selecting.

### Pattern 1 - Pointer boundary

```text
Valid pointer arithmetic != valid dereference
```

### Pattern 2 - Memory ownership

```text
Copying pointer -> copies address, not the pointed-to object.
```

### Pattern 3 - Array vs structure

```text
Array assignment      -> not allowed
Structure assignment  -> allowed
```

### Pattern 4 - ADT

```text
Same interface + different implementation = still same ADT abstraction
```

### Pattern 5 - Linked list

```text
Insert/delete can be pointer-cheap
BUT
finding the position may still require traversal.
```

### Pattern 6 - Header node

```text
Header does not magically make every operation O(1).
It mainly removes awkward boundary cases.
```

### Pattern 7 - Stack

```text
Everything happens at TOP.
```

### Pattern 8 - Postfix subtraction/division

```text
read left -> right
first pop = right operand
second pop = left operand
```

---

# 23. 🧪 30-Second Code Traces

## A. Alias trace

```c
int *p = malloc(sizeof(int));
*p = 100;
int *q = p;
int a = *p;
*q = 110;
```

Final:

```text
a   = 100
*p  = 110
*q  = 110
```

Why? `q` aliases `p`; `a` is a separate copied integer.

---

## B. One-past-end trace

```c
int a[5];
int *p = a + 5;
```

```text
p-a = 5
```

But:

```c
*p   // INVALID: dereference of one-past pointer
```

---

## C. Self-loop trace

```c
struct node { int key; struct node *next; } x;
x.key = 10;
x.next = &x;
```

Then:

```text
x.next == &x
x.next->key == 10
x.next->next == &x
```

This is a **cycle/self-loop**, not automatically a dangling pointer.

---

## D. Ordered insertion trace

```text
Current: 10 -> 20 -> 40
Insert: 30
```

Find:

```text
prev = 20
pres = 40
```

Insert:

```text
10 -> 20 -> 30 -> 40
```

Pointer update order:

```text
temp->next = pres
prev->next = temp
```

---

## E. Postfix trace

```text
7 8 9 + *
```

```text
push 7
push 8
push 9
+ -> 8+9 = 17
* -> 7*17 = 119
```

So the infix meaning is:

```text
7 * (8 + 9)
```

---

# 24. 🧷 Slide-Quiz Ambiguities / Typos to NOT Memorize Blindly

The deck contains several places where the printed quiz/code is ambiguous or appears to contain a typo. For exam revision, memorize the underlying concept, not the typo.

### 1. Array initialization question

The slide shows:

```c
int a[10];
int main() { }
```

Because `a` appears at file scope in the displayed code, storage-duration/context matters. The distractors are therefore not a safe rule to memorize as a universal statement.

**Memorize instead:**

```text
File-scope/static-duration array -> zero-initialized.
Automatic local array -> not automatically initialized.
```

### 2. Structure assignment quiz wording

The option says the destination gets a fieldwise copy of `b`, while the assignment is `b = a`.

**Rule:** destination `b` receives a memberwise copy of source `a`.

### 3. Pointer member access typo

The displayed expression `x.next.key` is inconsistent when `x.next` is a pointer.

**Rule:**

```c
x.next->key
```

is the pointer-member form.

### 4. `P->next_` vs `p->next_`

C is case-sensitive.

```text
p != P
```

If a quiz literally uses uppercase `P` when only lowercase `p` exists, that code does not mean the same variable.

### 5. `free(head_)` does not free the entire list

Freeing a single pointer target frees **one allocation**, not every node reachable after it.

You must explicitly traverse and free nodes one-by-one.

### 6. Uninitialized pointer vs dangling pointer

An uninitialized pointer is not automatically the same thing as a dangling pointer.

```text
Uninitialized -> indeterminate pointer value
Dangling      -> previously valid pointer to an object whose lifetime/access has ended
```

---

# 25. ✅ Final 60-Second Memory Dump

Say these without looking:

```text
1. C = pass by value.
2. Pointer = address + type.
3. q = p -> alias.
4. free(block) -> every alias to that block becomes dangling.
5. Lost allocated block -> garbage / leak.
6. Array = homogeneous + fixed-size + zero-based + random access.
7. *p++ = *(p++).
8. a+n can be one-past; one-past is not dereferenceable.
9. 2D arrays in the deck are row-major.
10. Structs are heterogeneous and assignment-compatible.
11. Struct size can include padding / implementation dependence.
12. Self-reference uses a pointer to the same struct.
13. ADT = interface/behaviour, not implementation.
14. List = sequential access + positions + insert/delete.
15. Linked list traversal = follow next pointer, not q++.
16. Ordered insertion has empty/beginning/middle/end cases.
17. Header node = dummy node that simplifies boundary cases.
18. Stack = LIFO, everything at top.
19. Extra ')' -> pop empty.
20. Extra '(' -> stack non-empty at end.
21. Postfix evaluation uses operand stack.
22. First pop = RHS; second pop = LHS.
23. Postfix has no parentheses.
24. Skiplist = linked list + shortcut levels.
25. Multilist = nodes can participate in multiple logical lists.
```

> [!success] Exam-mode mindset
> When an MCQ looks confusing, **draw the object graph**.
>
> - Pointer question? Draw arrows.
> - Linked-list question? Draw `prev -> temp -> pres`.
> - Memory question? Mark **reachable / unreachable / freed**.
> - Stack question? Draw TOP.
> - Postfix question? Draw the operand stack.
> - 2D array question? Write `row * columns + column`.
>
> Most "hard" Unit 1 MCQs collapse instantly once the state is drawn.

---

# 26. 📚 Source Coverage

This note compresses the supplied Unit 1 deck's teaching and quiz material across:

- Introduction + assessment context
- Functions and pointers
- Arrays
- Structures
- Dynamic allocation
- ADT and type concept
- List
- Linked list + ordered list
- List with header node
- Stack
- Well-formed parentheses/brackets
- Infix/postfix conversion and evaluation
- Polynomial experiential activity
- Skiplist
- Multilist + sparse matrix connection

The first pages identify the course as **Data Structures and its Applications (UE25CS252A), Unit 1**, and list programming in C plus pointers, arrays and structures as prerequisites.
