# Stack — Beyond the Course

> [!info] Scope This note covers stack topics that are **standard in DSA/interview prep** but were **not** in your handwritten notes or course code (`L8`–`L10`). Pairs with [[Stack Data Structure]] — read that one first for the ADT basics, array/list implementation, and the bracket-matching / postfix / infix-to-postfix material.

---

## 1. Complexity of Stack Operations

Worth stating explicitly since the course notes never did:

|Operation|Array-based|Linked-list-based|
|---|---|---|
|`push`|O(1)|O(1)|
|`pop`|O(1)|O(1)|
|`peek`|O(1)|O(1)|
|`is_empty` / `is_full`|O(1)|O(1)|
|Space|O(n), fixed capacity|O(n), dynamic|

All core operations are **O(1)** — that's _why_ stacks are used as a building block everywhere (function calls, undo systems, parsers) instead of something like an array with arbitrary-position insertion.

**Stack overflow / underflow** as OS-level concepts:

- **Stack overflow**: pushing past capacity. For the _array_ version this is your `is_full()` check. But the term also refers to a real OS phenomenon — e.g. uncontrolled/infinite recursion exhausting the **call stack** (the actual hardware/OS stack storing activation records), causing a crash. This is a different "stack" than your ADT, but the same LIFO principle.
- **Stack underflow**: popping an empty stack — your `is_empty()` check guards against this.

---

## 2. Dynamic (Resizable) Array Stack

Your course's array implementation uses a **fixed** `MAXSIZE`. In practice, most array-based stacks **grow** when full instead of just refusing to push.

**Idea:** when `is_full()` is true, allocate a new, larger array (commonly **double** the size), copy over existing elements, free the old array, then push.

c

```c
#include <stdio.h>
#include <stdlib.h>

struct stack
{
    int *key_;
    int  top_;
    int  capacity_;
};
typedef struct stack stack_t;

void init_stack(stack_t *ptr_stack)
{
    ptr_stack->capacity_ = 4;
    ptr_stack->key_ = (int *)malloc(sizeof(int) * ptr_stack->capacity_);
    ptr_stack->top_ = -1;
}

int is_empty(stack_t *ptr_stack)
{
    return ptr_stack->top_ == -1;
}

int is_full(stack_t *ptr_stack)
{
    return ptr_stack->top_ + 1 == ptr_stack->capacity_;
}

void resize(stack_t *ptr_stack)
{
    ptr_stack->capacity_ *= 2;
    ptr_stack->key_ = (int *)realloc(ptr_stack->key_, sizeof(int) * ptr_stack->capacity_);
}

void push(stack_t *ptr_stack, int val)
{
    if (is_full(ptr_stack))
    {
        resize(ptr_stack);       // grow instead of rejecting the push
    }
    ptr_stack->key_[++ptr_stack->top_] = val;
}

int pop(stack_t *ptr_stack)
{
    if (!is_empty(ptr_stack))
        return ptr_stack->key_[ptr_stack->top_--];
    printf("stack empty; cannot pop\n");
    return -1;
}
```

> [!tip] Amortized O(1) Even though a resize costs O(n) when it happens, doubling the capacity means resizes become exponentially rarer, so the **average (amortized)** cost per `push` across many operations is still O(1).

---

## 3. Circular Array Stack (index wraparound)

Not usually needed for a _plain_ stack (a plain stack only ever grows/shrinks from one end, so wraparound isn't necessary the way it is for queues) — but it's worth knowing why:

> [!note] Why circular indexing doesn't apply the same way here Circular buffering solves the problem of _reclaiming freed space at the front_ of a structure — relevant for **queues**, where you dequeue from the front and enqueue at the back, so the "used window" drifts. A stack only ever adds/removes at **one end** (`top_`), so the array naturally reuses the freed slot the moment you pop — no wraparound logic needed. This is a common interview _trick question_: "should you circular-index a stack?" — the answer is no, because a stack doesn't have the queue's drifting-window problem.

---

## 4. Two Stacks in One Array

A classic space-optimization trick: store two stacks in a single array by growing them from opposite ends toward the middle.

c

```c
#define SIZE 10
int arr[SIZE];
int top1 = -1;          // grows rightward from index 0
int top2 = SIZE;         // grows leftward from index SIZE-1

void push1(int val)
{
    if (top1 + 1 == top2) { printf("stack overflow\n"); return; }
    arr[++top1] = val;
}

void push2(int val)
{
    if (top1 + 1 == top2) { printf("stack overflow\n"); return; }
    arr[--top2] = val;
}

int pop1(void) { return (top1 == -1) ? -1 : arr[top1--]; }
int pop2(void) { return (top2 == SIZE) ? -1 : arr[top2++]; }
```

> [!example] `SIZE = 10`. Stack 1 pushes fill indices `0,1,2,...`. Stack 2 pushes fill indices `9,8,7,...`. They only collide (overflow) if `top1 + 1 == top2` — i.e. the two stacks meet in the middle. This uses the array's space far more flexibly than splitting it into two fixed halves.

---

## 5. Stack Using a Queue, and Queue Using a Stack

A common "make X behave like Y" exercise, useful for understanding both structures' tradeoffs.

### Stack using two queues

Make `push` expensive so `pop`/`top` stay simple:

c

```c
// push(x): enqueue x into q1, then move all of q2's elements... 
// Simplest version: always keep the newest element at the FRONT of one queue.
void push(int x)
{
    enqueue(q, x);
    // rotate the queue so the just-pushed element moves to the front
    for (int i = 0; i < queue_size(q) - 1; i++)
        enqueue(q, dequeue(q));
}
int pop(void)  { return dequeue(q); }   // front of q is always the "top"
int top(void)  { return front(q); }
```

- `push`: O(n) (the rotation)
- `pop` / `top`: O(1)

### Queue using two stacks

c

```c
stack_t in_stack, out_stack;

void enqueue(int x) { push(&in_stack, x); }

int dequeue(void)
{
    if (is_empty(&out_stack))
    {
        while (!is_empty(&in_stack))
            push(&out_stack, pop(&in_stack));   // reverse in_stack into out_stack
    }
    return pop(&out_stack);
}
```

- Each element moves from `in_stack` to `out_stack` **at most once**, so **amortized** O(1) per `dequeue`, even though a single call can look like O(n).

---

## 6. Infix ↔ Prefix ↔ Postfix — the missing conversions

Your course covered **infix → postfix** and **postfix evaluation** in detail. Here are the remaining standard conversions.

### 6.1 Infix → Prefix

**Trick:** reverse the infix string (swapping `(` ↔ `)` as you reverse), run the _infix→postfix_ algorithm on that reversed string, then reverse the resulting postfix string.

> [!example] `A + B * C` → prefix
> 
> 1. Reverse: `C * B + A` (and swap any brackets)
> 2. Convert to postfix using the usual algorithm: `C B * A +`
> 3. Reverse that result: `+ A * B C`
> 
> Prefix result: **`+ A * B C`**

### 6.2 Prefix Evaluation

Similar stack-of-operands idea as postfix evaluation, but scan **right to left**, and push operands directly; on an operator, pop two operands but apply them in **(op1, op2)** order (not reversed like postfix).

c

```c
// scan the prefix string from right to left
while (scanning right to left)
{
    if (isdigit(ch))
        push(ch - '0');
    else // operator
    {
        int op1 = pop();
        int op2 = pop();
        push(apply(ch, op1, op2));   // op1 <ch> op2, in that order
    }
}
// result = pop()
```

> [!example] `+ 3 * 4 5` (right to left: `5 4 * 3 +`)
> 
> - push 5
> - push 4
> - `*` → pop 4, pop 5 → push `4*5=20`
> - push 3
> - `+` → pop 3, pop 20 → push `3+20=23`
> - Result: **23**

### 6.3 Postfix → Infix

Scan left to right; push operands; on an operator, pop two operands `b` then `a`, and push the string `"(a op b)"`.

c

```c
// e.g. postfix "3 4 +"
// push "3", push "4"
// see '+' → pop "4" (b), pop "3" (a) → push "(3+4)"
// result: (3+4)
```

### 6.4 Summary table

|Conversion|Scan direction|Stack holds|Key trick|
|---|---|---|---|
|Infix → Postfix|left→right|operators|precedence + `(` high/low trick (covered in main note)|
|Infix → Prefix|—|operators|reverse input, do infix→postfix, reverse output|
|Postfix → Infix|left→right|sub-expressions (strings)|wrap `(a op b)` on each operator|
|Prefix → Infix|right→left|sub-expressions (strings)|wrap `(a op b)` on each operator|
|Postfix eval|left→right|operands (numbers)|`pop, pop`, apply `val2 op val1`|
|Prefix eval|right→left|operands (numbers)|`pop, pop`, apply `op1 op op2`|

---

## 7. Monotonic Stack Pattern

A very common interview pattern: maintain a stack where elements are kept in **increasing or decreasing order**, popping elements that violate the order as you scan.

### 7.1 Next Greater Element (NGE)

**Problem:** For each element in an array, find the next element to its right that is greater than it (or -1 if none).

c

```c
// arr[n] given; result[n] to fill
// stack holds INDICES, kept so that arr[stack] is decreasing top-to-bottom... 
// actually we pop whenever we find something bigger (a "monotonic decreasing" stack)
int stack[MAXSIZE], top = -1;
for (int i = n - 1; i >= 0; i--)
{
    while (top != -1 && arr[stack[top]] <= arr[i])
        top--;                              // pop smaller/equal elements
    result[i] = (top == -1) ? -1 : arr[stack[top]];
    stack[++top] = i;                       // push current index
}
```

> [!example] `arr = [4, 5, 2, 10, 8]` Scanning right to left with a monotonic decreasing stack:
> 
> |i|arr[i]|pops|NGE|stack after (indices, values shown)|
> |---|---|---|---|---|
> |4|8|—|-1|`[8]`|
> |3|10|pop 8|-1|`[10]`|
> |2|2|—|10|`[10, 2]`|
> |1|5|pop 2|10|`[10, 5]`|
> |0|4|—|5|`[10, 5, 4]`|
> 
> Result: `[5, 10, 10, -1, -1]`

Each element is pushed and popped **at most once** → overall **O(n)**, even though there's a nested loop.

### 7.2 Stock Span Problem

**Problem:** For each day's stock price, find how many consecutive previous days (including today) had price ≤ today's price.

c

```c
int span[n];
int stack[MAXSIZE], top = -1;   // stack of indices, prices decreasing top-down

for (int i = 0; i < n; i++)
{
    while (top != -1 && price[stack[top]] <= price[i])
        top--;
    span[i] = (top == -1) ? (i + 1) : (i - stack[top]);
    stack[++top] = i;
}
```

Same monotonic-stack idea as NGE, just tracking distance instead of value. O(n) overall.

### 7.3 Min Stack (O(1) minimum retrieval)

**Problem:** Design a stack that supports `push`, `pop`, `top`, and `get_min` — **all in O(1)**.

**Trick:** maintain a second, auxiliary stack that tracks the minimum-so-far at each level.

c

```c
stack_t main_stack, min_stack;

void min_push(int x)
{
    push(&main_stack, x);
    if (is_empty(&min_stack) || x <= peek(&min_stack))
        push(&min_stack, x);
    else
        push(&min_stack, peek(&min_stack));   // repeat current min
}

int min_pop(void)
{
    pop(&min_stack);
    return pop(&main_stack);
}

int get_min(void)
{
    return peek(&min_stack);
}
```

> [!example] Pushing `5, 3, 7, 2`
> 
> |push|main_stack (top→bottom)|min_stack (top→bottom)|
> |---|---|---|
> |5|`5`|`5`|
> |3|`3 5`|`3 3`|
> |7|`7 3 5`|`3 3 3`|
> |2|`2 7 3 5`|`2 3 3 3`|
> 
> `get_min()` is always just `peek(min_stack)` → O(1), and popping keeps both stacks in sync.

---

## 8. Real-World / Systems Applications (not implemented in course code)

- **Browser back button**: visited pages pushed onto a stack; "back" pops the current page and shows the previous one. (A "forward" button typically needs a _second_ stack.)
- **Undo/Redo in editors**: your course notes _mention_ this (`Ctrl+Z`) but never implement it. Typically needs **two stacks** — an undo stack and a redo stack. Every new action clears the redo stack.
- **Call stack / recursion**: every recursive function call implicitly uses the OS call stack to store each invocation's local variables and return address — this is the real mechanism behind the "stack frame" your notes mention conceptually.
- **DFS (Depth-First Search) & backtracking**: can be implemented either via actual recursion (using the call stack implicitly) or by maintaining an **explicit stack** of nodes to visit — useful when recursion depth would risk a stack overflow.
- **Tower of Hanoi**: canonically taught via recursion, but can also be simulated with an explicit stack storing `(n, from, to, via)` states — a good exercise in converting recursion to iteration using a stack, mirroring how the compiler does it under the hood.

---

## 9. Built-in / Library Stacks

Your course builds a stack from scratch in C (good for understanding internals), but most real code uses a language's built-in stack:

|Language|Typical stack tool|
|---|---|
|C++|`std::stack<T>` (STL, adapter over `deque` by default)|
|Python|plain `list` (`append`/`pop`) or `collections.deque` (faster for this use)|
|Java|`java.util.Stack` or preferably `Deque`/`ArrayDeque`|
|C|no built-in — you write it yourself, exactly as in your course|

Knowing the manual C implementation (which your course already covers thoroughly) is what makes these built-ins easy to understand later — they're all doing the same `push`/`pop`/`top` idea, usually backed by a dynamic array or linked structure.

---

## 10. Quick self-test

- [ ]  Convert `A * B + C / D` to prefix by hand, then verify with the reverse-and-convert trick.
- [ ]  Implement `get_min()` for a min-stack and trace it through pushing `[8, 3, 3, 6, 1]` then popping twice.
- [ ]  Implement Next Greater Element for `[13, 7, 6, 12]` by hand before checking with code.
- [ ]  Implement undo/redo with two stacks for a simple text-append-only editor.
- [ ]  Explain, in your own words, why circular indexing isn't needed for a plain array stack but is needed for a circular queue.

> [!info] See also [[Stack Data Structure]] — the primary note built from your course materials (ADT, array/list implementation, bracket matching, postfix evaluation, infix→postfix).

### Content

![Introduction_To_Stack.pdf](/api/7ba2394e-4722-4bed-a035-8bf707944839/files/23fa3906-eca3-4bc2-bc25-c5bc3ebee32c/thumbnail)

pdf

### stack.zip

zip

Session: 6% · resets in 4h 50m

Weekly: 3% · resets in 4d 16h