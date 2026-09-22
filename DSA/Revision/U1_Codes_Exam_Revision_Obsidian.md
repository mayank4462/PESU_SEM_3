---
title: Unit 1 C Codes — Exam Revision
aliases: [U1 Codes, Unit 1 Programs]
tags: [DSA, C, Unit-1, Revision, Obsidian]
---

# Unit 1 C Codes — Exam Revision

> **Purpose:** 30–60 minute revision before the exam. Learn the **logic + skeleton + critical C lines**, not the full programs word-for-word.
>
> **Rule:** For every program, first remember the English algorithm. Then the code snippets become easy to reconstruct.

---

# ⚡ 5-Minute Master Cheat Sheet

| Topic | Core memory |
|---|---|
| Stack using array | `top = -1`, push = `++top`, pop = `top--`, empty = `top == -1`, full = `top == MAX-1` |
| Stack using linked list | `top = NULL`, push node at front, pop first node, free removed node |
| Simple parentheses | `(` → push, `)` → pop; empty stack before `)` = invalid; final stack must be empty |
| All brackets | Push `([{`; on `)]}` pop and check matching pair; final stack empty |
| Postfix evaluation | Number → push; operator → `val2=pop()`, `val1=pop()`, compute, push result |
| Infix → postfix | Operand → output; operator → compare `F(top)` vs `G(incoming)`; pop higher precedence; `F==G` usually means matching `()` → discard |
| Singly linked list | `createNode()`, insertion changes links, deletion changes links + `free()` |
| Ordered linked list | Find first place where next value is greater than new value; insert there |
| Header-node list | Sentinel `head` is **not data**; actual list starts at `head->next` |
| Josephus | Circular list; `tail->link` is first node; delete kth node and move counting forward |
| Factorial interface | `main.c` uses function, `fact.c` defines it, `fact.h` declares it, Makefile builds everything |

---

# 🧠 Universal C Patterns to Remember

## Pointer to struct

```c
struct stack s;

// pass address when function expects a pointer
push(&s, value);
```

Inside the function:

```c
s->top
s->arr[s->top]
```

`->` is used because `s` is a **pointer to a struct**.

---

## Dynamic node creation

For linked structures:

```c
struct node *newNode = (struct node *)malloc(sizeof(struct node));

if(newNode == NULL)
    return NULL;
```

Then initialize:

```c
newNode->data = value;
newNode->next = NULL;
```

### Remember

> `malloc` creates memory → fill fields → connect node → `free()` when deleting.

---

## Header guard

```c
#ifndef STACK_H
#define STACK_H

// declarations

#endif
```

### Why?

Prevents the same header from being included multiple times.

---

# 1. Stack_Using_Array

## What this folder contains

- `stack.h` → stack structure + function declarations
- `stack.c` → push/pop/display/isFull/isEmpty
- `client.c` → input + calls stack functions
- Original implementation uses an **integer array** and `top`

---

## 1.1 Structure

### Logic

Stack is represented using:

```text
array + top
```

`top` stores the index of the current top element.

```c
#define MAX 100

struct stack
{
    int arr[MAX];
    int top;
};
```

### State of `top`

```text
-1          → empty
0           → one element
MAX - 1     → full
```

### MEMORY LINE

> **Array stack = `top` starts at `-1`.**

---

## 1.2 Initialization

The client directly initializes:

```c
struct stack s;
s.top = -1;
```

### Algorithm

```text
Create stack
Set top = -1
```

No `malloc()` is needed because the array is inside the struct.

---

## 1.3 `isEmpty()`

### Algorithm

```text
If top == -1 → empty
Otherwise → not empty
```

### Code

```c
bool isEmpty(struct stack *s)
{
    return (s->top == -1);
}
```

---

## 1.4 `isFull()`

Last valid array index is `MAX - 1`.

### Code

```c
bool isFull(struct stack *s)
{
    return (s->top == MAX - 1);
}
```

### Equivalent form

You may also see:

```c
s->top + 1 == MAX
```

Same condition.

---

## 1.5 PUSH

### English + C algorithm

```text
Check whether stack is full.
    ↓
If not full:
    increment top
    place value at new top
```

### Core snippet

```c
if(!isFull(s))
{
    s->arr[++s->top] = value;
}
```

### Understand this

```c
++s->top
```

means:

```text
s->top = s->top + 1
```

and only then store the value.

### MEMORY TRICK

> **PUSH = `++top` → store.**

---

## 1.6 POP

### English + C algorithm

```text
Check whether stack is empty.
    ↓
If not empty:
    take arr[top]
    decrement top
    return removed value
```

### Core snippet

```c
if(!isEmpty(s))
{
    return s->arr[s->top--];
}

return -1;
```

### Understand this

```c
s->arr[s->top--]
```

means:

```text
Take current top element
THEN top = top - 1
```

### MEMORY TRICK

> **POP = take first → `top--`.**

---

## 1.7 DISPLAY

### Algorithm

```text
Start from top
Move backwards until index 0
Print every element
```

### Code

```c
for(int i = s->top; i >= 0; i--)
{
    printf("%d ", s->arr[i]);
}
```

### Why backwards?

Because stack is naturally viewed from **top → bottom**.

---

## 1.8 `client.c` pattern

### Algorithm

```text
Create stack
Initialize top = -1
Read n
Read n values and push them
Ask whether user wants to pop
Pop while needed
Display remaining stack
```

### Important client line

```c
push(&s, value);
```

Since `push()` expects `struct stack *`, pass the address with `&`.

---

## 1.9 Exam reconstruction

```text
STRUCT
    arr[MAX]
    top

INIT
    top = -1

EMPTY
    top == -1

FULL
    top == MAX - 1

PUSH
    if not full
        arr[++top] = value

POP
    if not empty
        return arr[top--]

DISPLAY
    i = top → 0
```

---

## 1.10 MCQ / trap zone

### Trap 1

Empty is **not** `top == 0`.

Correct:

```c
top == -1
```

### Trap 2

Last valid index is `MAX - 1`, not `MAX`.

### Trap 3

These are different:

```c
arr[++top]
arr[top++]
```

For push, the current code uses:

```c
arr[++top] = value;
```

### Trap 4

These are different:

```c
arr[top--]
arr[--top]
```

The ZIP's pop operation uses:

```c
arr[top--]
```

### Complexity

- Push → **O(1)**
- Pop → **O(1)**
- isEmpty/isFull → **O(1)**
- Display → **O(n)**

---

# 2. ExpressionEvaluation

## What this folder does

Evaluates a **postfix expression** using an integer stack.

Example:

```text
2 3 +
```

means:

```text
2 + 3 = 5
```

The current client supports:

```text
+  -  *  /  ^
```

and supports multi-digit numbers such as `123`.

---

## 2.1 Stack structure

Same basic array stack idea, but it stores `int` operands.

```c
struct stack
{
    int key[MAX];
    int top;
};
```

Initialization:

```c
ptr_stack->top = -1;
```

---

## 2.2 The CORE postfix algorithm

Scan the expression **left → right**.

### Case 1: digit

Build the number.

### Case 2: space

A complete number has ended → push it.

### Case 3: operator

Pop two values:

```text
val2 = first pop
val1 = second pop
```

Then:

```text
result = val1 operator val2
```

Push the result back.

### Final result

The stack contains the answer.

---

## 2.3 Multi-digit number construction ⭐

The important line is:

```c
val = val * 10 + ch - '0';
```

### Example: reading `123`

```text
start val = 0
read '1' → 0*10 + 1 = 1
read '2' → 1*10 + 2 = 12
read '3' → 12*10 + 3 = 123
```

### MEMORY LINE

> **Digits stream in → `val = val*10 + digit`.**

`ch - '0'` converts the digit character into its numeric value.

---

## 2.4 Detecting numbers

The ZIP checks:

```c
else if(ch >= '0' && ch <= '9')
{
    is_num = 1;
    val = val * 10 + ch - '0';
}
```

So:

```text
is_num = 1
```

means we are currently building a number.

---

## 2.5 Pushing a completed number

When a space arrives:

```c
else if(ch == ' ' && is_num)
{
    push(&mystack, val);
    val = 0;
    is_num = 0;
}
```

### Algorithm

```text
space + number was being built
        ↓
push number
reset val
reset is_num
```

The current `client.c` additionally handles the case where the expression does **not** end with a space:

```c
if(is_num)
{
    push(&mystack, val);
}
```

This prevents the last operand from being lost.

---

## 2.6 Operator handling ⭐⭐⭐

When the current character is an operator:

```c
if(ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^')
```

First:

```c
val2 = pop(&mystack);
val1 = pop(&mystack);
```

Then calculate:

```c
case '+': push(&mystack, val1 + val2); break;
case '-': push(&mystack, val1 - val2); break;
case '*': push(&mystack, val1 * val2); break;
case '/': push(&mystack, val1 / val2); break;
```

For power:

```c
case '^':
    push(&mystack, (int)round(pow(val1, val2)));
    break;
```

---

## 2.7 WHY `val2` is popped first ⭐⭐⭐

Suppose:

```text
8 2 /
```

Before `/`:

```text
8   ← bottom
2   ← top
```

First pop = `2` → right operand.

Second pop = `8` → left operand.

Therefore:

```c
val2 = pop();
val1 = pop();
```

and then:

```c
val1 / val2
```

### Huge exam trap

Do **not** do:

```c
val1 = pop();
val2 = pop();
```

and then `val1 - val2`, because subtraction/division would reverse the operands.

---

## 2.8 Result

The current client uses:

```c
printf("result : %d\n", peek(&mystack));
```

because after evaluating the complete postfix expression, the final result is at the top.

---

## 2.9 Postfix evaluation in one flow

```text
Read symbol
   ↓
Digit? ──YES──> build number
   │
   NO
   ↓
Space + number complete? ──YES──> push number
   │
   NO
   ↓
Operator?
   ↓
val2 = pop()
val1 = pop()
   ↓
compute val1 op val2
   ↓
push(result)
   ↓
End of expression
   ↓
peek() = answer
```

---

## 2.10 Stack functions used here

### `push`

```c
ptr_stack->key[++ptr_stack->top] = key;
```

### `pop`

```c
return ptr_stack->key[ptr_stack->top--];
```

### `peek`

```c
return ptr_stack->key[ptr_stack->top];
```

### `init`

```c
ptr_stack->top = -1;
```

---

## 2.11 Important code-quality traps in the ZIP

The older `.md` version uses:

```c
gets(str);
```

`gets()` is unsafe and should not be used in modern C.

The current `client.c` uses:

```c
scanf(" %[^\n]", str);
```

which is better for this lab's simple input style.

Also, malformed postfix expressions can cause stack underflow, and division by zero is not guarded. These are **implementation limitations**, not part of the basic exam algorithm.

---

## 2.12 Complexity

If there are `n` characters/tokens:

- Scan → **O(n)**
- Stack push/pop → **O(1)**
- Overall → **O(n)**
- Extra stack space → **O(n)**

---

# 3. Factorial

## What this folder teaches

This is less about the factorial formula and more about **splitting a C program into interfaces/modules**.

Folder contains:

```text
main.c
fact.c
fact.h
Makefile
```

There are also generated `.o` and `.exe` files in the ZIP; those are build artifacts, not logic you need to memorize.

---

## 3.1 `fact.h` — declaration / interface

The header tells the compiler that the function exists.

```c
#ifndef FACT_H
#define FACT_H

int factorial(int n);

#endif
```

### Think of it as

> **“Here is the function signature. The actual code is somewhere else.”**

---

## 3.2 `fact.c` — implementation

The actual factorial function is defined here.

### Algorithm

```text
fact = 1
for i = n down to 1
    fact = fact * i
return fact
```

### Core code

```c
int factorial(int n)
{
    int fact = 1;

    for(int i = n; i > 0; i--)
    {
        fact *= i;
    }

    return fact;
}
```

### MEMORY LINE

> **Factorial = accumulator `1` + loop downward.**

---

## 3.3 `main.c` — client / user interaction

### Flow

```text
read n
↓
call factorial(n)
↓
store result
↓
print result
```

Core call:

```c
int fact_result = factorial(n);
```

---

## 3.4 Why `#include "fact.h"`?

Both `main.c` and `fact.c` include the header because both need to know the function prototype.

```c
#include "fact.h"
```

---

## 3.5 Header guard logic

```c
#ifndef FACT_H
#define FACT_H

...

#endif
```

### Meaning

```text
#ifndef → Is FACT_H not defined?
#define → Define it now
#endif → Finish guard
```

Prevents repeated inclusion.

---

## 3.6 Makefile idea

The ZIP's Makefile establishes:

```text
main.c  → main.o
fact.c  → fact.o
main.o + fact.o → executable
```

Important target idea:

```makefile
factorial_program: main.o fact.o
	gcc -o factorial_program main.o fact.o
```

Object-file dependency rules tell `make` which `.c`/`.h` files each `.o` depends on.

---

## 3.7 Build flow ⭐

```text
main.c ──compile──> main.o

fact.c ──compile──> fact.o

main.o + fact.o ──link──> factorial_program.exe
```

### MEMORY LINE

> `.h` = declaration, `.c` = implementation, `.o` = compiled object, final executable = linked objects.

---

## 3.8 MCQ zone

- `fact.h` contains **prototype**, not the factorial implementation.
- `fact.c` contains **definition**.
- `main.c` contains `main()`.
- Makefile automates compile + link.
- Header guards prevent duplicate inclusion.

---

# 4. Infix_to_Postfix

## What this folder does

Converts an infix expression such as:

```text
A+B*C
```

to postfix form according to operator precedence.

The ZIP uses the **F/G precedence-function technique**.

---

## 4.1 Core idea

There are two precedence functions:

### `G(symbol)`

> Precedence of a symbol **when it is coming from the input**.

### `F(symbol)`

> Precedence of a symbol **when it is already on the stack**.

### Main comparison

```text
If F(stack_top) > G(incoming)
        → pop stack to postfix
Else if F(stack_top) != G(incoming)
        → push incoming
Else
        → discard matching pair / pop
```

This is the heart of the program.

---

## 4.2 Stack structure

This stack stores `char`, because operators and parentheses are characters.

```c
struct stack
{
    char key[MAX];
    int top;
};
```

Initialization:

```c
ptr_stack->top = -1;
```

Push:

```c
ptr_stack->key[++ptr_stack->top] = ch;
```

Pop:

```c
return ptr_stack->key[ptr_stack->top--];
```

Peek:

```c
return ptr_stack->key[ptr_stack->top];
```

---

## 4.3 G table — incoming/input precedence

From the current ZIP:

| Symbol | `G` |
|---|---:|
| `+`, `-` | 1 |
| `*`, `/` | 3 |
| `$`, `^` | 6 |
| `(` | 9 |
| `)`, `=` | 0 |
| operand/other | 7 |

### Important

`(` gets a **high incoming precedence** (`9`) so that it can enter the stack.

---

## 4.4 F table — stack precedence

From the current ZIP:

| Symbol | `F` |
|---|---:|
| `+`, `-` | 2 |
| `*`, `/` | 4 |
| `$`, `^` | 5 |
| `(` | 0 |
| `#` | -1 |
| operand/other | 8 |

### The important relationship

```text
operator already on stack → use F
incoming operator           → use G
```

---

## 4.5 Why F and G are different

For normal operators, `F` and `G` being different lets the algorithm control associativity.

Examples:

```text
+ : F=2, G=1
* : F=4, G=3
^ : F=5, G=6
```

So exponentiation behaves differently from the lower-precedence operators in the comparison process.

---

## 4.6 Operand recognition ⭐

The current client supports identifiers rather than only single letters.

It checks:

```c
if(isalpha(symbol) || symbol == '_')
```

Then continues while the next characters are:

```c
isalnum(...) || ... == '_'
```

So names such as:

```text
abc
x1
_total
value42
```

are treated as one operand token.

### Core idea

```text
operand starts
    ↓
copy characters
    ↓
continue while alphanumeric or '_'
    ↓
append space to postfix
```

Core snippet:

```c
do
{
    *p++ = infix[i];
    i++;
} while(isalnum(infix[i]) || infix[i] == '_');

*p++ = ' ';
i--;
```

---

## 4.7 Operator handling ⭐⭐⭐

For a non-operand symbol:

```c
while(!is_empty(&mystack) &&
      F(peek(&mystack)) > G(symbol))
{
    *p++ = pop(&mystack);
    *p++ = ' ';
}
```

### English

```text
While stack-top precedence is greater than incoming precedence:
    pop stack
    append popped operator to postfix
```

---

## 4.8 After popping

Now compare again:

```c
if(F(peek(&mystack)) != G(symbol))
{
    push(&mystack, symbol);
}
else
{
    pop(&mystack);
}
```

### Meaning

#### `F != G`

Push the incoming operator.

#### `F == G`

Discard the matching pair by popping the stack top.

This is especially important for:

```text
( ... )
```

because:

```text
F('(') = 0
G(')') = 0
```

So when `)` arrives after the matching `(` is on the stack:

```text
F('(') == G(')')
```

and the `(` is removed instead of being written to postfix.

---

## 4.9 Final stack flushing

After the entire infix expression is scanned:

```c
while(!is_empty(&mystack))
{
    *p++ = pop(&mystack);
    *p++ = ' ';
}
```

### English

> Any operators still waiting on the stack are now appended to postfix.

Finally:

```c
*p = '\0';
```

terminates the postfix string.

---

## 4.10 Complete conversion mental model

```text
Read symbol
    ↓
Operand?
 ├─ YES → copy to postfix
 │        continue identifier
 │
 └─ NO
      ↓
While F(top) > G(symbol)
      ↓
pop → postfix
      ↓
F(top) == G(symbol)?
 ├─ YES → pop/discard
 └─ NO  → push symbol

After input ends:
    pop everything remaining
```

---

## 4.11 Parentheses memory trick

For this specific F/G system:

```text
Input '('  → G is HIGH (9) → easy to push
Input ')'  → G is LOW (0)  → pops operators
Stack '('  → F is 0

F('(') == G(')')
→ pop matching '(' and discard it
```

---

## 4.12 Current source details worth knowing

### `ctype.h`

The current client uses:

```c
#include <ctype.h>
```

to detect:

```c
isalpha()
isalnum()
```

### Postfix buffer

The current client uses:

```c
char postfix[2 * MAX + 1];
```

because it appends spaces between tokens/operators.

---

## 4.13 Important implementation quirks

The source calls:

```c
F(peek(&mystack))
```

after the `while` even if the stack is empty.

`peek()` then prints a stack-empty message and returns `\0`, which falls through the `default` case of `F()`.

So the program can still work for the expected valid input, but it is **noisy and not ideal defensive coding**.

Also remember:

- Input is treated as a valid infix expression.
- The converter itself does not robustly validate malformed expressions.
- The identifier-aware version is more capable than the older Markdown version in the same folder.

---

## 4.14 Complexity

For expression length `n`:

- Scan input → **O(n)**
- Every operator is pushed and popped at most once → **O(n)** total
- Overall → **O(n)**
- Stack/postfix storage → **O(n)**

---

# 5. Josephus

## What this folder does

Implements the **Josephus elimination problem** using a **circular linked list**.

People are stored in a circle, and every `k`th person is eliminated until one survivor remains.

---

## 5.1 Core structures

```c
struct node
{
    int key;
    char name[MAX];
    struct node *link;
};

struct list
{
    struct node *tail;
};
```

### Critical circular-list fact ⭐⭐⭐

With a `tail` pointer:

```text
tail->link = FIRST NODE
```

So:

```text
head = tail->link
```

There is **no `NULL` at the end** of a non-empty circular list.

---

## 5.2 Initialization

```c
void init_clist(struct list* ptr_list)
{
    ptr_list->tail = NULL;
}
```

### Meaning

```text
tail == NULL → list is empty
```

---

## 5.3 Create node

### Algorithm

```text
malloc node
check allocation
copy name
store key
set link = NULL
return node
```

Important pieces:

```c
struct node *temp = (struct node *)malloc(sizeof(struct node));
strcpy(temp->name, name);
temp->key = key;
temp->link = NULL;
```

`strcpy()` is used because names are stored as strings.

---

## 5.4 Insert at end of circular list

### Case 1: list empty

```text
new node points to itself
node becomes tail
```

Code:

```c
if(ptr_list->tail == NULL)
{
    temp->link = temp;
    ptr_list->tail = temp;
}
```

Diagram:

```text
      ┌──────────┐
      ↓          │
    [A] ─────────┘
    tail = A
```

---

### Case 2: list already has nodes

Suppose:

```text
tail → C
head → A
```

To append `D`:

```c
temp->link = ptr_list->tail->link;
ptr_list->tail->link = temp;
ptr_list->tail = temp;
```

### Meaning

```text
D->link = old head
old tail->link = D
new tail = D
```

So:

```text
A → B → C → D → A
```

---

## 5.5 Josephus deletion function

The ZIP calls this function:

```c
delete_end(&mylist, k);
```

but **despite the name, it is not ordinary “delete from end.”**

It finds and removes the `k`th node in the circular traversal.

---

## 5.6 Deleting the kth node ⭐⭐⭐

Start with:

```c
struct node *prev = ptr_list->tail;
struct node *curr = ptr_list->tail->link;
int count = 1;
```

So:

```text
prev = tail
curr = head
count = 1
```

Then:

```c
while(count != k)
{
    prev = curr;
    curr = curr->link;
    count++;
}
```

### Meaning

Move `curr` until it is the kth person.

`prev` stays immediately behind it.

---

## 5.7 Actual deletion

Once `curr` is the person to eliminate:

```c
prev->link = curr->link;
free(curr);
ptr_list->tail = prev;
```

### English

```text
Bypass curr
Free curr
Make prev the new tail
```

Because the list is circular:

```text
new head = new tail->link
```

which naturally makes counting continue from the node after the eliminated person.

---

## 5.8 Why `tail = prev`?

Example:

```text
A → B → C → D → A
```

Suppose `C` is deleted.

Before:

```text
prev = B
curr = C
```

After:

```text
B → D
```

and if:

```text
tail = B
```

then:

```text
tail->link = D
```

So the new logical first node is exactly the node after the eliminated person.

---

## 5.9 Display circular list

### Algorithm

```text
curr = tail->link   // first node
print curr
move curr = curr->link
stop when curr returns to first node
```

Core pattern:

```c
struct node *curr = ptr_list->tail->link;

do
{
    printf("%s ", curr->name);
    curr = curr->link;
}
while(curr != ptr_list->tail->link);
```

### Important

Use a **`do...while`** pattern because a non-empty circular list must print the first node before checking whether we have returned to the start.

---

## 5.10 Client flow

```text
initialize circular list
    ↓
read n and k
    ↓
insert n people
    ↓
while n > 1
    delete kth person
    display remaining list
    n--
    ↓
print survivor
```

Final survivor:

```c
printf("Survivor: %s\n", mylist.tail->link->name);
```

Since only one node remains, `tail->link` points to that survivor.

---

## 5.11 Circular linked-list MCQ traps

### Trap 1

Non-empty circular list does **not** end in `NULL`.

### Trap 2

```c
tail->link
```

is the first node/head.

### Trap 3

For one-node circular list:

```text
tail->link == tail
```

### Trap 4

The function named `delete_end()` is being used as the **Josephus kth-node elimination operation**, not normal tail deletion.

### Trap 5

The source's loop relies on the circular structure. `curr != NULL` is not a meaningful termination mechanism for a normal non-empty circular list.

---

## 5.12 Complexity

- Insert end with tail pointer → **O(1)**
- Each kth elimination → **O(k)** traversal in the code's direct implementation
- Full Josephus run → depends on `n` and `k`; the simple implementation repeatedly traverses the circle
- Display remaining circle → **O(n)**

---

# 6. Linked List

## What this folder contains

A standard **singly linked list** with a plain `head` pointer.

Supported operations:

### Insertion

1. Beginning
2. End
3. Position
4. Before key
5. Ordered

### Deletion

6. Beginning
7. End
8. Position
9. Key
10. All

### Other

11. Display

---

## 6.1 Node structure

```c
struct node
{
    int data;
    struct node *next;
};
```

### Mental picture

```text
[data|next] → [data|next] → [data|next] → NULL
```

---

## 6.2 `createNode()`

### Algorithm

```text
malloc node
if malloc fails → return NULL
store data
set next = NULL
return node
```

### Core code

```c
struct node *newNode = (struct node *)malloc(sizeof(struct node));

if(newNode == NULL)
    return NULL;

newNode->data = value;
newNode->next = NULL;

return newNode;
```

### MEMORY LINE

> **Every insertion starts with a new node.**

---

## 6.3 INSERT AT BEGINNING ⭐

### Situation

```text
head → A → B → NULL
```

Insert `X`.

### Algorithm

```text
create X
X->next = head
head = X
return head
```

### Core snippet

```c
newNode->next = head;
head = newNode;
```

### Diagram

```text
Before:
head → A → B → NULL

After:
head → X → A → B → NULL
```

### Complexity

**O(1)**

---

## 6.4 INSERT AT END ⭐

### Algorithm

```text
create node
if list empty → new node becomes head
otherwise traverse until curr->next == NULL
connect curr->next = newNode
return head
```

Core traversal:

```c
struct node *curr = head;

while(curr->next != NULL)
{
    curr = curr->next;
}

curr->next = newNode;
```

### Key idea

You stop at the **last node**, not one node after it.

### Complexity

**O(n)**

---

## 6.5 INSERT AT POSITION ⭐⭐⭐

Positions in this implementation are **1-indexed**.

```text
position 1 = first real node
position 2 = second node
...
```

### Algorithm

```text
if pos < 1 → invalid
if pos == 1 → use insertAtBeginning
if head == NULL → invalid for pos > 1

create new node
curr = head
i = 1

move curr until it is at position pos-1

if position does not exist → free new node and stop

newNode->next = curr->next
curr->next = newNode
```

### Core pointer rearrangement

```c
newNode->next = curr->next;
curr->next = newNode;
```

### This is the pattern to memorize

> **New node points forward first; previous node is then redirected to new node.**

---

## Why the order matters

Wrong:

```c
curr->next = newNode;
newNode->next = curr->next;
```

Now `newNode->next` points to itself.

Correct:

```c
newNode->next = curr->next;
curr->next = newNode;
```

---

## 6.6 INSERT BEFORE KEY ⭐⭐⭐

Function concept:

```c
insertKey(head, key, value)
```

means:

> Insert `value` **before the node whose data == key**.

### Special case

If the key is at the head:

```c
if(head->data == key)
    return insertAtBeginning(head, value);
```

### Otherwise

Start at head and search for the node **whose next node** contains the key:

```c
while(curr->next != NULL && curr->next->data != key)
{
    curr = curr->next;
}
```

Then insert:

```c
newNode->next = curr->next;
curr->next = newNode;
```

### MEMORY TRICK

> **To insert before a node, stand on the node BEFORE it.**

---

## 6.7 INSERT IN ORDER ⭐⭐⭐

Assumption:

> The linked list is already sorted.

Suppose:

```text
10 → 20 → 40 → NULL
```

Insert `30`.

### Algorithm

```text
Create new node
If list empty → new node is head
If value <= head->data → insert at beginning
Otherwise walk while next->data <= value
Insert before first node greater than value
```

Core:

```c
while(curr->next != NULL && curr->next->data <= value)
{
    curr = curr->next;
}

newNode->next = curr->next;
curr->next = newNode;
```

### Duplicate behavior

The `<=` means equal values are skipped first, so a new duplicate is inserted **after existing equal values**.

---

## 6.8 DELETE AT BEGINNING ⭐

### Algorithm

```text
If empty → return NULL
Store head->next
free(head)
return stored next node
```

Core:

```c
struct node *temp = head->next;
free(head);
return temp;
```

### MEMORY LINE

> **Delete first = save next → free old head → return next.**

### Complexity

**O(1)**

---

## 6.9 DELETE AT END ⭐⭐

### Cases

#### Empty

Return `NULL`.

#### One node

Free head and return `NULL`.

#### Multiple nodes

Walk until the **second-last node**.

### Core traversal

```c
while(curr->next->next != NULL)
{
    curr = curr->next;
}
```

Then:

```c
free(curr->next);
curr->next = NULL;
```

### MEMORY TRICK

> **To delete last node, stop at second-last.**

### Complexity

**O(n)**

---

## 6.10 DELETE AT POSITION ⭐⭐⭐

### Algorithm

```text
If empty / invalid → stop
If pos == 1 → delete beginning
Otherwise move to position pos-1
If target doesn't exist → stop
Save target node
Bypass target
free target
```

Core:

```c
struct node *temp = curr->next;
curr->next = temp->next;
free(temp);
```

Again:

> **Stand on previous node → bypass target → free target.**

---

## 6.11 DELETE BY KEY ⭐⭐⭐

Delete first node whose data equals `key`.

### Head special case

```c
if(head->data == key)
    return deleteAtBeginning(head);
```

### Otherwise search previous node

```c
while(curr->next != NULL && curr->next->data != key)
{
    curr = curr->next;
}
```

Then:

```c
struct node *temp = curr->next;
curr->next = temp->next;
free(temp);
```

### MEMORY LINE

> **Search by key → stop BEFORE matching node → bypass + free.**

---

## 6.12 DELETE ALL ⭐⭐⭐

### Algorithm

You cannot free a node and then use `head->next` from that freed node.

So first save the next pointer.

```c
while(head != NULL)
{
    temp = head->next;
    free(head);
    head = temp;
}

return NULL;
```

### Correct order

```text
save next
↓
free current
↓
move head to saved next
```

### MEMORY LINE

> **Delete-all = save next BEFORE free.**

---

## 6.13 DISPLAY

### Algorithm

```text
curr = head
while curr != NULL
    print data
    curr = curr->next
print NULL
```

Core:

```c
while(curr != NULL)
{
    printf("%d -> ", curr->data);
    curr = curr->next;
}

printf("NULL\n");
```

---

## 6.14 Return-value convention ⭐⭐⭐

This implementation returns `head` from insertion/deletion operations.

That means in `main.c` you MUST reassign:

```c
head = insertAtBeginning(head, value);
head = insertAtEnd(head, value);
head = deleteAtBeginning(head);
head = deleteAtEnd(head);
```

### Why?

Because operations such as insertion at beginning or deletion of first node can change the head pointer.

### Huge exam trap

```c
insertAtBeginning(head, value);
```

without assigning the returned head can lose the update.

---

## 6.15 Full function map

```text
createNode

INSERT
├── insertAtBeginning
├── insertAtEnd
├── insertAtPos
├── insertKey
└── insertOrdered

DELETE
├── deleteAtBeginning
├── deleteAtEnd
├── deleteAtPos
├── deleteKey
└── deleteAll

OTHER
└── display
```

---

## 6.16 Complexity table

| Operation | Complexity |
|---|---:|
| Create node | O(1) |
| Insert beginning | O(1) |
| Insert end | O(n) |
| Insert position | O(n) |
| Insert key | O(n) |
| Insert ordered | O(n) |
| Delete beginning | O(1) |
| Delete end | O(n) |
| Delete position | O(n) |
| Delete key | O(n) |
| Delete all | O(n) |
| Display | O(n) |

---

## 6.17 Linked-list pointer patterns to memorize

### Insert after `curr`

```c
newNode->next = curr->next;
curr->next = newNode;
```

### Delete node after `curr`

```c
temp = curr->next;
curr->next = temp->next;
free(temp);
```

### Move traversal pointer

```c
curr = curr->next;
```

### Last node

```c
curr->next == NULL
```

### Second-last node

```c
curr->next->next == NULL
```

These four patterns cover a huge portion of linked-list questions.

---

# 7. OrderList-code

## Why this folder is different

This is still a singly linked list, but it uses a **header / sentinel node**.

Instead of:

```c
struct node *head;
```

it uses:

```c
struct linked_list
{
    struct node *head;
};
```

The `head` node is a **dummy node** and does not represent actual user data.

Actual data begins at:

```c
list->head->next
```

---

## 7.1 Structures ⭐⭐⭐

```c
struct node
{
    int data;
    struct node *next;
};

struct linked_list
{
    struct node *head;
};
```

### Mental picture

```text
list->head
    ↓
[DUMMY] → [10] → [20] → [30] → NULL
```

The dummy node is always first.

---

## 7.2 `initList()`

### Algorithm

```text
malloc dummy node
if allocation fails → stop
set dummy data = 0
set dummy next = NULL
```

Core:

```c
list->head = (struct node *)malloc(sizeof(struct node));

list->head->data = 0;
list->head->next = NULL;
```

### Important

`data = 0` is unused/sentinel data.

An empty list means:

```c
list->head->next == NULL
```

not:

```c
list->head == NULL
```

while the list is initialized.

---

## 7.3 Why a header node helps ⭐⭐⭐

In a normal linked list, inserting at position 1 often needs a special case:

```text
if(pos == 1)
    change head
```

With a sentinel, the dummy node is already the “previous node” of the first actual element.

So insertion logic becomes uniform:

```text
curr starts at dummy head
↓
walk to previous position
↓
insert after curr
```

### MEMORY LINE

> **Header node removes many head-special-case branches.**

---

## 7.4 INSERT AT BEGINNING

### Algorithm

```text
create node
newNode->next = first actual node
head->next = newNode
```

Core:

```c
newNode->next = list->head->next;
list->head->next = newNode;
```

### Diagram

```text
Before:
[DUMMY] → A → B

After:
[DUMMY] → X → A → B
```

Notice that `list->head` itself never changes.

---

## 7.5 INSERT AT END

### Algorithm

Start from the **dummy head**, then walk to the last actual node.

```c
struct node *curr = list->head;

while(curr->next != NULL)
{
    curr = curr->next;
}

curr->next = newNode;
```

Because the empty list already has the dummy node, there is no `head == NULL` special case.

---

## 7.6 INSERT AT POSITION

Positions are still 1-indexed for actual data.

The sentinel is treated like position `0`.

### Algorithm

```text
curr = dummy head
 i = 0

Move while:
    i < pos-1
    AND next node exists

After movement, curr is the node after which the insertion happens.
```

Core:

```c
struct node *curr = list->head;
int i = 0;

while(i < pos - 1 && curr->next != NULL)
{
    curr = curr->next;
    i++;
}
```

Then:

```c
newNode->next = curr->next;
curr->next = newNode;
```

### Why position 1 is easy

For `pos = 1`:

```text
i = 0
pos - 1 = 0
```

The loop runs zero times.

So `curr` remains at dummy head, and insertion happens immediately after it.

---

## 7.7 INSERT BEFORE KEY

This version doesn't need a “key is at head” branch.

Start from dummy:

```c
struct node *curr = list->head;
```

Search the **next** node:

```c
while(curr->next != NULL && curr->next->data != key)
{
    curr = curr->next;
}
```

Then insert:

```c
newNode->next = curr->next;
curr->next = newNode;
```

### Why this is so clean

The dummy node acts as the predecessor of the first real node.

---

## 7.8 INSERT ORDERED ⭐⭐⭐

Assumption:

> The real list is already sorted.

Start at the dummy node:

```c
struct node *curr = list->head;
```

Move while:

```c
curr->next != NULL && curr->next->data <= value
```

Then insert.

Core:

```c
while(curr->next != NULL && curr->next->data <= value)
{
    curr = curr->next;
}

newNode->next = curr->next;
curr->next = newNode;
```

### Duplicate rule

Because of `<=`, the new equal-valued node is placed **after existing equal values**.

---

## 7.9 DELETE AT BEGINNING

The first real node is:

```c
list->head->next
```

### Algorithm

```text
If no real node → nothing to delete
Save first real node
Move dummy->next to second node
free old first node
```

Core:

```c
struct node *temp = list->head->next;
list->head->next = temp->next;
free(temp);
```

No head replacement is needed.

---

## 7.10 DELETE AT END

Start at dummy head.

Move until current node is second-last actual node:

```c
while(curr->next->next != NULL)
{
    curr = curr->next;
}
```

Then:

```c
free(curr->next);
curr->next = NULL;
```

The sentinel makes even the one-element case uniform.

---

## 7.11 DELETE AT POSITION

Start from dummy, treating it as position 0.

```c
struct node *curr = list->head;
int i = 0;

while(i < pos - 1 && curr->next != NULL)
{
    curr = curr->next;
    i++;
}
```

Then:

```c
struct node *temp = curr->next;
curr->next = temp->next;
free(temp);
```

### Key point

Same pointer rearrangement as normal linked list, but the **dummy node eliminates the position-1 special case**.

---

## 7.12 DELETE BY KEY

Start from dummy.

Search the next node:

```c
while(curr->next != NULL && curr->next->data != key)
{
    curr = curr->next;
}
```

Then bypass + free:

```c
struct node *temp = curr->next;
curr->next = temp->next;
free(temp);
```

Again, no head special case.

---

## 7.13 DELETE ALL ⭐⭐⭐

Important difference from the normal linked-list version:

The **dummy head itself was dynamically allocated**, so it also must be freed.

Current code starts at:

```c
struct node *curr = list->head;
```

Then:

```c
while(curr != NULL)
{
    temp = curr->next;
    free(curr);
    curr = temp;
}

list->head = NULL;
```

### MEMORY LINE

> **Header-node delete-all frees the sentinel too.**

---

## 7.14 Reinitialization after `deleteAll()`

The current implementation sets:

```c
list->head = NULL;
```

So the struct no longer has a valid sentinel.

Therefore the client does:

```c
deleteAll(&list);
initList(&list);
```

after option 10.

### Exam trap

After `deleteAll()` in this specific implementation:

> **Call `initList()` before using the list again.**

---

## 7.15 API difference vs normal linked list ⭐⭐⭐

### Normal linked list

Functions return the potentially changed `head`:

```c
head = insertAtBeginning(head, value);
```

### Header-node list

Functions receive the list structure by pointer and directly modify it:

```c
insertAtBeginning(&list, value);
```

Functions are mostly `void`.

### MEMORY COMPARISON

```text
Plain list:
head pointer itself may change
→ return head

Header-node list:
sentinel stays the same
→ modify list->head->next
→ no head reassignment needed
```

---

## 7.16 Header-node vs normal list

| Feature | Plain linked list | Header-node list |
|---|---|---|
| Empty representation | `head == NULL` | `head->next == NULL` after init |
| Insert at beginning | changes head | does not change sentinel |
| Position 1 special case | usually yes | usually no |
| Key at first node special case | usually yes | no |
| API | return `head` | mutate `list` |
| Extra node | no | yes, dummy node |

---

## 7.17 Complexity

The asymptotic costs are similar to a normal singly linked list:

- Insert beginning → **O(1)**
- Insert end → **O(n)**
- Insert position → **O(n)**
- Insert key → **O(n)**
- Insert ordered → **O(n)**
- Delete beginning → **O(1)**
- Delete end → **O(n)**
- Delete position → **O(n)**
- Delete key → **O(n)**
- Delete all → **O(n)**
- Display → **O(n)**

---

# 8. Stack_All_Brackets

## What this folder does

Checks whether an expression has **balanced `()`, `[]`, and `{}` brackets**.

Example of valid form:

```text
{[()]}
```

Invalid examples:

```text
([)]
```

or

```text
((]
```

---

## 8.1 Stack type

Because we store bracket characters, the array is `char`:

```c
struct stack
{
    char key[MAX];
    int top;
};
```

Initialization:

```c
ptr_stack->top = -1;
```

---

## 8.2 Core balanced-brackets algorithm ⭐⭐⭐

Scan the string left → right.

### Opening bracket

If current character is:

```text
(  [  {
```

→ **push it**.

### Closing bracket

If current character is:

```text
)  ]  }
```

then:

```text
If stack empty → invalid
Else pop top bracket
Check whether popped bracket matches current closer
Mismatch → invalid
```

At the end:

```text
valid flag must still be true
AND stack must be empty
```

Both conditions matter.

---

## 8.3 Opening bracket condition

Core code:

```c
if(ch == '(' || ch == '[' || ch == '{')
{
    push(&mystack, ch);
}
```

### Memory

> **Every opener gets pushed.**

---

## 8.4 Closing bracket condition

```c
else if(ch == ')' || ch == ']' || ch == '}')
```

First check empty:

```c
if(is_empty(&mystack))
{
    is_valid = 0;
}
```

Why?

A closing bracket cannot exist without an unmatched opener.

---

## 8.5 Pop and match ⭐⭐⭐

If stack is not empty:

```c
top_ch = pop(&mystack);
```

Then:

```c
if ((ch == ')' && top_ch != '(') ||
    (ch == ']' && top_ch != '[') ||
    (ch == '}' && top_ch != '{'))
{
    is_valid = 0;
}
```

### Logic in plain English

```text
')' must match '('
']' must match '['
'}' must match '{'
```

---

## 8.6 Final check ⭐⭐⭐

This is critical:

```c
if(is_valid && is_empty(&mystack))
{
    printf("Well formed expression\n");
}
```

### Why check the stack at the end?

Consider:

```text
(((
```

No mismatch happens while scanning, so `is_valid` can stay `1`.

But the stack still contains unmatched opening brackets.

Therefore:

```text
valid flag == true
AND
stack empty == true
```

are both required.

### MEMORY LINE

> **Balanced brackets = no mismatch + no leftovers.**

---

## 8.7 Short-circuit scanning

The loop is:

```c
while(is_valid && (ch = *s++) != '\0')
```

Meaning:

```text
Continue while expression is still valid
AND
characters remain
```

Once invalidity is detected, the next loop condition fails.

---

## 8.8 What characters matter?

Only the bracket characters affect this algorithm.

Other characters are ignored.

So alphabetic/digit/operator characters do not enter the stack.

---

## 8.9 Complexity

For `n` characters:

- Scan → **O(n)**
- Push/pop → **O(1)** each
- Overall → **O(n)**
- Extra stack space → **O(n)**

---

## 8.10 MCQ traps

### Trap 1

A valid prefix does **not** guarantee the entire string is balanced.

### Trap 2

`is_valid == 1` alone is insufficient.

Need:

```c
is_valid && is_empty(...)
```

### Trap 3

LIFO matters.

For:

```text
([{}])
```

the closing brackets must match the **most recent opener** first.

### Trap 4

Stack stores `char`, not `int`.

---

# 9. Stack_LinkedList

## What this folder does

Implements a stack using a **linked list** instead of a fixed-size array.

Main structure:

```c
struct node
{
    int value;
    struct node *next;
};

struct stack
{
    struct node *top;
};
```

---

## 9.1 Stack representation

```text
top
 ↓
[30] → [20] → [10] → NULL
```

The top of the stack is simply the **first linked-list node**.

Therefore push/pop can both happen at the front.

---

## 9.2 Initialization

The client does:

```c
struct stack s;
s.top = NULL;
```

### Meaning

```text
top == NULL → empty stack
```

---

## 9.3 PUSH ⭐⭐⭐

### Algorithm

```text
Create a new node
Store value
New node points to current top
Move top to new node
```

### Core code

```c
struct node *newNode = malloc(sizeof(struct node));

newNode->value = value;
newNode->next = s->top;
s->top = newNode;
```

### Diagram

Before:

```text
top → A → B → NULL
```

After pushing X:

```text
top → X → A → B → NULL
```

### MEMORY LINE

> **Linked stack push = insert at beginning.**

---

## 9.4 POP ⭐⭐⭐

### Algorithm

```text
If empty → underflow
Otherwise:
    save top node
    save its value
    move top to next node
    free old top
    return value
```

Core:

```c
struct node *temp = s->top;
int value = temp->value;

s->top = temp->next;
free(temp);

return value;
```

### MEMORY LINE

> **Pop = save top → move top → free old top.**

---

## 9.5 DISPLAY

Traverse from `top`:

```c
struct node *curr = s->top;

while(curr != NULL)
{
    printf("%d -> ", curr->value);
    curr = curr->next;
}
```

---

## 9.6 `isEmpty()`

```c
return (s->top == NULL);
```

Very straightforward.

---

## 9.7 `isFull()` — special idea ⭐⭐

Unlike an array stack, there is no fixed `MAX` in this implementation.

The code tests whether another node can be allocated:

```c
struct node *temp = malloc(sizeof(struct node));

if(temp == NULL)
    return true;

free(temp);
return false;
```

### Meaning

```text
malloc succeeds → stack can grow
malloc fails → treat stack as full
```

So “full” means **no more heap memory is available**, not “top reached MAX-1.”

---

## 9.8 Array stack vs linked stack ⭐⭐⭐

| Feature | Array stack | Linked-list stack |
|---|---|---|
| Top representation | integer index | node pointer |
| Empty | `top == -1` | `top == NULL` |
| Capacity | fixed | dynamic until memory exhaustion |
| Push | `arr[++top]` | create node at front |
| Pop | `arr[top--]` | remove first node |
| Push complexity | O(1) | O(1) |
| Pop complexity | O(1) | O(1) |

### MEMORY LINE

> **Stack ADT stays the same; only the storage mechanism changes.**

---

# 10. Stack_SimpleParantheses

## What this folder does

Checks only one bracket type:

```text
( )
```

Example:

```text
((()))
```

---

## 10.1 Stack structure

Same array-based character stack style as the other bracket program:

```c
struct stack
{
    char key_[MAXSIZE];
    int top_;
};
```

The underscore naming is simply this folder's style.

Initialization:

```c
ptr_stack->top_ = -1;
```

---

## 10.2 Core algorithm ⭐⭐⭐

Scan left → right.

### If `(`

Push it.

### If `)`

```text
If stack empty → invalid
Else pop the opener
```

At end:

```text
valid flag must still be true
AND stack must be empty
```

---

## 10.3 Opening bracket

```c
if(ch == '(')
{
    push(&mystack, '(');
}
```

---

## 10.4 Closing bracket

```c
else if(ch == ')')
{
    if(!is_empty(&mystack))
    {
        ch1 = pop(&mystack);
        if(ch1 == '(')
        {
            is_valid = 1;
        }
    }
    else
    {
        is_valid = 0;
    }
}
```

### More logical way to remember it

Because only `'('` is ever pushed:

```text
')' + empty stack → invalid
')' + non-empty stack → pop one '('
```

The popped value check is therefore largely redundant for this specific implementation.

---

## 10.5 Final check ⭐⭐⭐

```c
if(is_valid && is_empty(&mystack))
```

Again:

> **No premature closing parenthesis + no leftover opening parentheses = balanced.**

---

## 10.6 Comparison with full bracket version

### Simple parentheses

Only:

```text
(
)
```

### All brackets

```text
(
[
{
)
]
}
```

The full version must additionally compare the **type** of the bracket.

---

## 10.7 Implementation quirks

The source contains:

```c
is_valid = 1;
```

after successfully popping `'('`.

Since the loop only continues while `is_valid` is already true, this assignment is effectively redundant.

The important logic is actually:

```text
empty when ')' arrives → invalid
otherwise pop
final stack empty → valid
```

---

## 10.8 Complexity

For `n` characters:

- Time → **O(n)**
- Extra stack space → **O(n)**

---

# 🔥 Cross-Topic Comparison

## Array stack vs linked stack

| | Array | Linked list |
|---|---|---|
| Storage | contiguous array | dynamically allocated nodes |
| Empty | `top == -1` | `top == NULL` |
| Full | `top == MAX-1` | allocation failure |
| Push | increment index | add node at front |
| Pop | decrement index | remove node at front |
| Fixed capacity | yes | no practical fixed MAX |

---

## Normal linked list vs header-node list

| | Normal | Header node |
|---|---|---|
| Head | actual first data node | dummy/sentinel node |
| First data | `head` | `head->next` |
| Empty | `head == NULL` | `head->next == NULL` after init |
| Insert position 1 | special case usually needed | naturally inserts after sentinel |
| API | return updated head | mutate `list` |
| Extra memory | no dummy | one dummy node |

---

## Simple parentheses vs all brackets

```text
SIMPLE
( → push
) → pop

ALL
( [ { → push
) ] } → pop + match type
```

Both require:

```text
stack empty at end
```

to detect leftover opening brackets.

---

# 🧠 Most Important Code Snippets to Memorize

## Stack array

```c
s.top = -1;
```

```c
if(!isFull(s))
    s->arr[++s->top] = value;
```

```c
if(!isEmpty(s))
    return s->arr[s->top--];
```

```c
return s->top == -1;
```

```c
return s->top == MAX - 1;
```

---

## Linked-list insertion after a node

```c
newNode->next = curr->next;
curr->next = newNode;
```

## Linked-list deletion after a node

```c
temp = curr->next;
curr->next = temp->next;
free(temp);
```

## Delete all

```c
temp = head->next;
free(head);
head = temp;
```

## Circular list first node

```c
tail->link
```

## Circular list traversal

```c
do
{
    // process curr
    curr = curr->link;
}
while(curr != start);
```

## Header-node insertion at beginning

```c
newNode->next = list->head->next;
list->head->next = newNode;
```

## Linked stack push

```c
newNode->next = s->top;
s->top = newNode;
```

## Linked stack pop

```c
temp = s->top;
s->top = temp->next;
free(temp);
```

## Postfix evaluation

```c
val2 = pop(&mystack);
val1 = pop(&mystack);
```

then:

```c
push(&mystack, val1 - val2);
```

etc.

## Digit accumulation

```c
val = val * 10 + ch - '0';
```

## Balanced brackets

```c
if(ch == '(' || ch == '[' || ch == '{')
    push(&mystack, ch);
```

and:

```c
if ((ch == ')' && top_ch != '(') ||
    (ch == ']' && top_ch != '[') ||
    (ch == '}' && top_ch != '{'))
{
    is_valid = 0;
}
```

## Final bracket test

```c
if(is_valid && is_empty(&mystack))
```

---

# 🎯 10-Minute Exam Reconstruction Drill

Close the notes and try to write just the following from memory.

## Drill 1 — Array stack

```text
struct stack?
empty?
full?
push?
pop?
```

Expected mental answer:

```text
arr + top
-1
MAX-1
++top
[top--]
```

---

## Drill 2 — Singly linked list

Draw:

```text
A → B → C → NULL
```

Then write from memory:

```text
insert after B
 delete C
```

The two pointer patterns should immediately come out:

```c
newNode->next = curr->next;
curr->next = newNode;
```

and:

```c
temp = curr->next;
curr->next = temp->next;
free(temp);
```

---

## Drill 3 — Postfix

For:

```text
8 2 - 3 *
```

mentally do:

```text
8 push
2 push
- → val2=2, val1=8 → 6 push
3 push
* → val2=3, val1=6 → 18 push
```

Answer = `18`.

---

## Drill 4 — Balanced brackets

For:

```text
{[()]}
```

mental stack sequence:

```text
{ push
[ push
( push
) pop (
] pop [
} pop {
end → empty → valid
```

For:

```text
([)]
```

at `)`:

```text
stack top = '['
expected = '('
Mismatch → invalid
```

---

## Drill 5 — Infix to postfix

Remember only this first:

```text
Operand → output
Operator → compare F(top) and G(input)
F > G → pop
F != G → push
F == G → pop/discard
End → flush stack
```

Then recall the precedence tables.

---

# 🏁 Final 1-Minute Recall

```text
STACK ARRAY
    top = -1
    push  → ++top
    pop   → top--
    empty → -1
    full  → MAX-1

LINKED STACK
    top = NULL
    push at front
    pop at front
    free removed node

SINGLY LL
    head → first node
    insert: connect new node
    delete: bypass + free
    return new head when head can change

HEADER NODE LL
    head = dummy node
    real data starts head->next
    no head-changing return required

CIRCULAR LL / JOSEPHUS
    tail->link = first
    no NULL at end
    count kth node
    bypass + free
    tail = previous

SIMPLE BRACKETS
    ( → push
    ) → pop
    final stack empty

ALL BRACKETS
    ([{ → push
    )]} → pop + compare matching pair
    final stack empty

POSTFIX
    number → push
    operator → val2=pop, val1=pop
    compute val1 op val2
    push result

INFIX → POSTFIX
    G = incoming precedence
    F = stack precedence
    F > G → pop
    F != G → push
    F == G → discard matching pair
    flush stack at end

FACTORIAL INTERFACE
    .h → declaration
    .c → definition
    main → call
    Makefile → build/link
```

---

# ✅ Pre-Exam Checklist

Before entering the exam, make sure these are instant recall:

- [ ] `top = -1` for array stack
- [ ] `top == MAX - 1` for full
- [ ] `arr[++top]` for push
- [ ] `arr[top--]` for pop
- [ ] `top == NULL` for linked-list stack
- [ ] `tail->link` is first node in circular list
- [ ] `do...while` is natural for circular traversal
- [ ] Linked-list insertion = set `newNode->next` first
- [ ] Linked-list deletion = save target → bypass → `free`
- [ ] `deleteAll` saves next pointer before `free`
- [ ] Plain linked list operations return updated `head`
- [ ] Header-node list keeps dummy head and edits `head->next`
- [ ] Postfix subtraction/division order = second pop is left operand
- [ ] `val = val*10 + ch-'0'` builds multi-digit numbers
- [ ] Balanced brackets require **no mismatch + empty stack**
- [ ] Infix `F` = stack precedence, `G` = incoming precedence
- [ ] `F > G` → pop
- [ ] `F != G` → push
- [ ] `F == G` → discard matching pair
- [ ] Header guards = `#ifndef / #define / #endif`

---

> **Golden strategy:** Do not memorize the programs line-by-line. Memorize the **pointer movement / stack movement**, then use the snippets in this sheet to rebuild the syntax.
