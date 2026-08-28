# Stack Data Structure

> [!abstract] TL;DR A **stack** is a _linear_ data structure where every operation (insert/delete) happens at only **one end**, called the **top**. It follows **LIFO — Last In, First Out**. Think of a stack of plates: you can only add or remove a plate from the top.

---

## 1. Concept of an ADT (quick revision)

- A **type** is an interface — it tells you _what_ you can do with it, not _how_ it's done internally.
- A **data structure** is an **Abstract Data Type (ADT)**: it defines the _operations_ it supports but does **not commit to any particular implementation**.
- The client only needs the interface; the same interface can be backed by different implementations (array vs linked list) without the client's code changing — worst case in C/C++, the client just recompiles.
- This separation is powerful: a developer can support many clients, and a client can swap implementations transparently.

---

## 2. Stack as an ADT

**Definition:** Stack is a linear data structure whose operations are performed in **LIFO (Last In, First Out)** order.

> [!example] Analogy A spring-loaded plate dispenser in a restaurant, or a basket where item **2** (which entered last) is the **first** one to come back out.
> 
> ```
>      ┌───┐   ← insertion / deletion happens only here (the "top")
>      │ 2 │
>      ├───┤
>      │ 1 │
>      └───┘
> ```

### Properties of Stack ADT

- It is a data structure.
- Has zero or more components.
- Can operate at **only one end** — normally called the **stack top**.
- Supports **LIFO**: what goes in last, comes out first.
- Does **NOT** support the concept of _traversal_ (you can't just walk through the middle elements — only the top is accessible).

### Interface (operations)

|Operation|Meaning|
|---|---|
|`create`|Create a new stack|
|`destroy`|Destroy the stack|
|`push()`|Add an element at the top|
|`pop()`|Remove and return the topmost element|
|`peek(index)`|Return the value at a given position without removing it|
|`is_empty()`|Check whether the stack is empty|
|`is_full()`|Check whether the stack is full|

push(x)TOPpop()...rest of stack...

A stack can be **implemented** using:

1. An **array**
2. A **linked list**

The interface stays identical either way — the client doesn't need to know or care which one is used underneath.

---

## 3. Applications of Stack

1. **Function calls / stack frame (activation record)** — when a function is called, an activation record is created holding parameters, local variables, compiler-generated temporaries, and the return address. If that function calls another, a new activation record is pushed on top. When a function returns, its record is popped off.
    - _Think about it:_ Would every stack frame be the same size? (No — depends on the function's locals/params.)
2. **Redo-undo operations** — e.g. `Ctrl+Z` in an editor. The system must track operations applied and undo the _last_ one first → needs a stack.
3. **Parentheses / bracket matching** — checking whether brackets in an expression are well-formed (used by compilers).
4. **Infix → Postfix conversion** (and other similar conversions), and **expression evaluation**.
5. Other applications you'll meet later in your course:
    - Depth First Search (DFS) of a graph
    - Backtracking
    - Branch and bound
    - Simulating recursive functions

> [!note] Formal language connection Stack-based languages belong to the class of **context-free languages**, recognized by a **push-down automaton**. Almost all programming-language syntax falls into this category — you'll study this formally later.

---

## 4. Stack Implementation

We implement the stack **twice** — once with an array, once with a linked list — while keeping the **exact same interface**, to prove the ADT principle: implementation can change without the client caring.

### 4.1 Array-based implementation

**`stack.h`**

c

```c
#define MAXSIZE 5

struct stack
{
    char key_[MAXSIZE];
    int  top_;
};
typedef struct stack stack_t;

void init_stack(stack_t *ptr_stack);
void deinit_stack(stack_t *ptr_stack);
void push(stack_t *ptr_stack, char ch);
char pop(stack_t *ptr_stack);
int  is_empty(stack_t *ptr_stack);
int  is_full(stack_t *ptr_stack);
```

- `key_[MAXSIZE]` — the underlying array holding the stack's characters (here a stack of `char`, but any type could be used).
- `top_` — index of the **current top element** (NOT the next free slot). This design decision affects every function.

**`stack.c`**

c

```c
void init_stack(stack_t *ptr_stack)
{
    ptr_stack->top_ = -1;      // -1 = invalid index = empty stack
}

void deinit_stack(stack_t *ptr_stack)
{
    ptr_stack->top_ = -1;      // conceptually empties the stack
}

void push(stack_t *ptr_stack, char ch)
{
    if (!is_full(ptr_stack))
    {
        ptr_stack->key_[++ptr_stack->top_] = ch;   // pre-increment, then store
    }
    else
    {
        printf("stack full; cannot push\n");
    }
}

char pop(stack_t *ptr_stack)
{
    if (!is_empty(ptr_stack))
    {
        return ptr_stack->key_[ptr_stack->top_--]; // return, then decrement
    }
    else
    {
        printf("stack empty; cannot pop\n");
        return '\0';
    }
}

int is_empty(stack_t *ptr_stack)
{
    return ptr_stack->top_ == -1;
}

int is_full(stack_t *ptr_stack)
{
    return ptr_stack->top_ + 1 == MAXSIZE;
}
```

> [!tip] Why `top_ = -1`? Every variable should have a clearly defined meaning. Here `top_` means **"index of the last element added"**, not "index of the next free slot." That single decision shapes every function above (`++top_` before storing on push, `top_--` after reading on pop).

**Client program — reverse a string using the array stack (`client.c`)**

c

```c
#include <stdio.h>
#include "stack.h"

// reverse a string using a stack
int main()
{
    char str[MAXSIZE + 1]; char *s = str;
    scanf("%s", str);
    printf("input string : :%s:\n", s);

    stack_t mystack;
    init_stack(&mystack);

    char ch;
    while ((ch = *s++) != '\0')
    {
        push(&mystack, ch);
    }

    printf("Reversed string : ");
    while (!is_empty(&mystack))
    {
        putchar(pop(&mystack));
    }
    deinit_stack(&mystack);
}
```

**Idea:** push every character of the input string, then pop them all — since pop reverses insertion order (LIFO), the output is the string reversed.

> [!example] Worked example Input: `hello` Push order: `h e l l o` → stack (top→bottom): `o l l e h` Pop order: `o l l e h` → printed: **`olleh`**

---

### 4.2 Linked-list-based implementation

Same interface, different internal structure — the client's code does **not need to change** (at most, recompile).

**`stack.h`**

c

```c
#define MAXSIZE 100

struct node
{
    char key_;
    struct node* next_;
};
typedef struct node node_t;

struct stack
{
    node_t* top_;
};
typedef struct stack stack_t;

void init_stack(stack_t *ptr_stack);
void deinit_stack(stack_t *ptr_stack);
void push(stack_t *ptr_stack, char ch);
char pop(stack_t *ptr_stack);
int  is_empty(stack_t *ptr_stack);
int  is_full(stack_t *ptr_stack);
```

- `node_t` is a **self-referential structure**: each node holds a `key_` and a pointer `next_` to the node below it.
- `stack_t` now only needs to remember `top_`, a pointer to the topmost node.

**`stack.c`**

c

```c
void init_stack(stack_t *ptr_stack)
{
    ptr_stack->top_ = NULL;         // empty stack = NULL top
}

void deinit_stack(stack_t *ptr_stack)
{
    while (!is_empty(ptr_stack))    // pop everything to free memory
    {
        pop(ptr_stack);
    }
    ptr_stack->top_ = NULL;
}

void push(stack_t *ptr_stack, char ch)
{
    if (!is_full(ptr_stack))
    {
        node_t* temp = (node_t*)malloc(sizeof(node_t));
        temp->key_  = ch;
        temp->next_ = ptr_stack->top_;   // new node points to old top
        ptr_stack->top_ = temp;          // top_ now points to new node
    }
    else
    {
        printf("stack full; cannot push\n");
    }
}

char pop(stack_t *ptr_stack)
{
    if (!is_empty(ptr_stack))
    {
        char ch;
        node_t* temp = ptr_stack->top_;  // 1. point to current top
        ch = temp->key_;                 // (copy key first)
        ptr_stack->top_ = temp->next_;   // 2. move top to next node
        free(temp);                      // 3. free the old top node
        return ch;                       // 4. return the saved key
    }
    else
    {
        printf("stack empty; cannot pop\n");
        return '\0';
    }
}

int is_empty(stack_t *ptr_stack)
{
    return ptr_stack->top_ == NULL;
}

int is_full(stack_t *ptr_stack)
{
    // No simple way to know if heap has memory — try allocating.
    node_t* temp = (node_t*)malloc(sizeof(node_t));
    if (temp == NULL)
    {
        return 1;      // allocation failed → stack effectively full
    }
    free(temp);        // must free — otherwise memory leak!
    return 0;
}
```

> [!warning] Order matters In `push`, the new node's `next_` **must** be set to the old `top_` **before** `top_` is updated to point to the new node — otherwise you lose the rest of the stack. In `pop`, the sequence is: save the key → save the top pointer → advance `top_` → `free()` the old node → return the saved key. Freeing before saving/advancing would cause undefined behaviour.

**Client program (identical logic to the array version, just relinked to the new `stack.h`)**

c

```c
#include <stdio.h>
#include "stack.h"

// reverse a string using a stack
int main()
{
    char str[MAXSIZE + 1]; char *s = str;
    scanf("%s", str);
    printf("input string : :%s:\n", s);

    stack_t mystack;
    init_stack(&mystack);

    char ch;
    while ((ch = *s++) != '\0')
    {
        push(&mystack, ch);
    }

    printf("Reversed string : ");
    while (!is_empty(&mystack))
    {
        putchar(pop(&mystack));
    }
    deinit_stack(&mystack);
}
```

### 4.3 Array vs Linked List — comparison

|Aspect|Array implementation|Linked-list implementation|
|---|---|---|
|Fixed size|Yes (`MAXSIZE` compile-time constant)|No — grows dynamically until heap runs out|
|`is_full()` check|Simple index compare|Tricky — must attempt `malloc` and check for failure|
|Memory overhead|None extra|Extra pointer (`next_`) per element|
|`top_` meaning|Index of top element|Pointer to top node|
|Client interface|Same|Same — this is the whole point of the ADT!|

---

## 5. Application 1: Well-Formed Parentheses (single bracket type)

**Problem:** Given an expression, determine whether its parentheses are well-formed — ignoring everything except `(` and `)`.

- Well formed: `(3 + 4)` → `()`, `((5+6)*(7+8))` → `(())()`... etc.
- Not well formed: `(()`, `())`, `)(`, `())())`

Just counting left vs right parentheses is **not enough** — order matters too (`)(` has equal counts but is invalid). This is exactly what a stack captures.

### Algorithm

1. Initialize an empty stack.
2. Read the expression into a string.
3. Walk through it character by character:
    - On `(` → push a placeholder (order matters — we've "opened" a bracket).
    - On `)` → if the stack is non-empty, pop; if the stack **is** empty, we hit a `)` with no matching `(` → invalid, stop.
4. After processing, the expression is well-formed **iff** the stack is empty.

c

```c
stack_t mystack;
init_stack(&mystack);

int is_valid = 1;
char ch; char *s = str;
while (is_valid && (ch = *s++) != '\0')
{
    if (ch == '(')
    {
        push(&mystack, 'x');           // placeholder
    }
    else if (ch == ')')
    {
        if (!is_empty(&mystack))
            pop(&mystack);
        else
            is_valid = 0;              // ')' with nothing to match
    }
}

if (is_valid && is_empty(&mystack))
    printf("Well formed expression\n");
else
    printf("not well formed expression\n");
```

> [!example] Trace: `(())()`
> 
> |char|action|stack (top→bottom)|
> |---|---|---|
> |`(`|push|`x`|
> |`(`|push|`x x`|
> |`)`|pop|`x`|
> |`)`|pop|_(empty)_|
> |`(`|push|`x`|
> |`)`|pop|_(empty)_|
> 
> Stack ends empty and `is_valid` stayed 1 → **well formed**.

> [!example] Trace: `())(` `(` → push `x` → stack `[x]` `)` → pop → stack `[]` `)` → stack empty → `is_valid = 0` → **not well formed** (correctly rejected, even though `(` and `)` counts are equal!)

---

## 6. Application 2: Well-Formed Brackets (multiple bracket types)

Now handle **4 kinds of brackets conceptually** (vinculum can't be typed) — `()`, `{}`, `[]` — and expressions can mix them.

- Well formed: `(){}[]`, `([]{})`, `([{}])`
- Not well formed: `([)]`, `([]])`, `(][)`

A single generic placeholder is no longer enough — we must remember **which** bracket type was opened, so that when a closing bracket appears we can check it matches the most recently opened one.

**Trick used in the code:** instead of pushing the opening bracket itself, push its **corresponding closing bracket**. Then, on encountering an actual closing bracket, just compare it directly with `pop()`.

### Algorithm

1. On `(` → push `)`. On `[` → push `]`. On `{` → push `}`.
2. On a closing bracket (`)`, `]`, `}`):
    - If the stack is empty → invalid (nothing to match).
    - Else pop the top; if it doesn't equal the current character → invalid (wrong type of bracket closed).
3. At the end, valid iff the stack is empty.

c

```c
char str[MAXSIZE + 1];
char *s = str;
int  is_valid = 1;
char ch;

while (is_valid && (ch = *s++) != '\0')
{
    if (ch == '(')      push(&mystack, ')');
    else if (ch == '[') push(&mystack, ']');
    else if (ch == '{') push(&mystack, '}');
    else if (ch == ')' || ch == ']' || ch == '}')
    {
        if (!is_empty(&mystack))
        {
            if (ch != pop(&mystack))
                is_valid = 0;          // matched wrong bracket type
        }
        else
            is_valid = 0;
    }
}

if (is_valid && is_empty(&mystack))
    printf("Well formed expression\n");
else
    printf("not well formed expression\n");
```

> [!example] Trace: `([{}])`
> 
> |char|action|stack pushed (top→bottom)|
> |---|---|---|
> |`(`|push `)`|`)`|
> |`[`|push `]`|`] )`|
> |`{`|push `}`|`} ] )`|
> |`}`|pop → `}` ✓ matches|`] )`|
> |`]`|pop → `]` ✓ matches|`)`|
> |`)`|pop → `)` ✓ matches|_(empty)_|
> 
> Stack empty at the end → **well formed**.

> [!example] Trace: `([)]` `(` → push `)` → `[)]`... stack: `)` `[` → push `]` → stack: `] )` `)` → pop → gives `]`, but current char is `)` → **mismatch** → `is_valid = 0` → **not well formed** (this catches "crossed" brackets, which counting alone never could).

---

## 7. Application 3: Infix vs Postfix Expressions

Normal expressions like `3 + 4` write the **operator between** operands — this is called **infix**. Evaluating infix directly is complicated because of:

- **BODMAS / operator precedence** (Brackets → Orders → Division → Multiplication → Addition → Subtraction),
- **associativity** rules,
- **nested parentheses**.

A **postfix** expression places the operator **after** its operands, e.g.:

|Infix|Postfix|
|---|---|
|`3 + 4`|`3 4 +`|
|`4 * 5 + 6`|`4 5 * 6 +`|
|`4 + 5 * 6`|`4 5 6 * +`|
|`5 * 6 + 7 * 8`|`5 6 * 7 8 * +`|

In postfix, **every operator is at the same "precedence" and left-associative**, so it can be evaluated in a single left-to-right pass with a stack of operands — no need to worry about precedence or brackets at evaluation time.

---

## 8. Application 4: Evaluating a Postfix Expression

**Idea:** Use a stack of _operands_ (integers). Scan left to right:

- On seeing a number → push it.
- On seeing an operator → pop the two most recent operands, apply the operator, push the result back.
- At the end, exactly one value remains on the stack — the result.

> [!important] Order of pops matters for `-` and `/` `val1 = pop()` (this was pushed **second**, i.e. the right operand), then `val2 = pop()` (the **left** operand). The result is computed as `val2 <op> val1`, e.g. `val2 - val1`, **not** `val1 - val2`.

c

```c
stack_t mystack;
init_stack(&mystack);

char *s = str;
char ch;
int val1, val2;
int val = 0;
int is_num = 0;

while ((ch = *s++) != '\0')
{
    if (ch == '+' || ch == '-' || ch == '*' || ch == '/')
    {
        val1 = pop(&mystack);      // right operand (pushed last)
        val2 = pop(&mystack);      // left operand
        switch (ch)
        {
            case '+': push(&mystack, val2 + val1); break;
            case '-': push(&mystack, val2 - val1); break;
            case '*': push(&mystack, val2 * val1); break;
            case '/': push(&mystack, val2 / val1); break;
        }
    }
    else if (ch >= '0' && ch <= '9')
    {
        is_num = 1;
        val = val * 10 + ch - '0';     // accumulate multi-digit numbers
    }
    else if (ch == ' ' && is_num)
    {
        push(&mystack, val);           // number finished (hit a space) → push it
        val = 0;
        is_num = 0;
    }
}
printf("result : %d\n", pop(&mystack));
```

> [!note] Assumption Operands and operators are separated by whitespace, which simplifies detecting where a (possibly multi-digit) number ends.

> [!example] Trace: `3 4 +`
> 
> - `3` → digit, `val=3`
> - → push 3 → stack `[3]`
> - `4` → digit, `val=4`
> - `+` → pop `4` (val1), pop `3` (val2) → push `3+4=7` → stack `[7]`
> - Result: **7**

> [!example] Trace: `5 6 * 7 8 * +`
> 
> - push 5, push 6 → stack `[5,6]`
> - `*` → pop 6, pop 5 → push `5*6=30` → stack `[30]`
> - push 7, push 8 → stack `[30,7,8]`
> - `*` → pop 8, pop 7 → push `7*8=56` → stack `[30,56]`
> - `+` → pop 56, pop 30 → push `30+56=86` → stack `[86]`
> - Result: **86**

---

## 9. Application 5: Infix → Postfix Conversion

**Idea:** Use a stack of _operators_. Scan the infix expression left to right:

- **Operand** → send straight to output.
- **Operator** →
    - if the stack is empty, push it;
    - else, while the top of the stack has **higher or equal precedence** (and is left-associative), pop it to output, then push the current operator.
- At the end, pop all remaining operators to the output.

### Simple examples (no parentheses)

|Infix|Steps|Postfix|
|---|---|---|
|`3 + 4`|output 3; push `+`; output 4; pop `+`|`3 4 +`|
|`4 * 5 + 6`|output 4; push `*`; output 5; see `+` (lower prec than `*` on stack) → pop `*`, push `+`; output 6; pop `+`|`4 5 * 6 +`|
|`4 + 5 * 6`|output 4; push `+`; output 5; see `*` (higher prec than `+` on stack) → push `*` (don't pop `+`); output 6; pop `*`, pop `+`|`4 5 6 * +`|
|`5 * 6 + 7 * 8`|output 5; push `*`; output 6; see `+` → pop `*`, push `+`; output 7; see `*` → push `*`; output 8; pop `*`, pop `+`|`5 6 * 7 8 * +`|

### Handling parentheses

Parentheses need special treatment since postfix has none:

- `(` is always **pushed** immediately — give it a very **high** precedence _while it's still in the input string_ (so it always wins and gets pushed), but once it's **sitting in the stack**, treat it as having very **low** precedence (so it never gets popped by an ordinary operator — everything else piles on top of it).
- `)` triggers popping everything **up to and including** the matching `(` out to the output (the `(` itself is discarded, not output).

> [!example] Trace: `3 * (4 + 5) * 6` → `3 4 5 + * 6 *`
> 
> 1. output `3`
> 2. push `*`
> 3. `(` has high precedence in-string → push `(`
> 4. output `4`
> 5. `(` in stack now has low precedence → push `+`
> 6. output `5`
> 7. `)` encountered → pop until `(`: pop `+` to output, discard `(`
> 8. see `*` (equal precedence, left-associative) vs `*` on stack → pop stack's `*` to output, push new `*`
> 9. output `6`
> 10. end of string → pop remaining `*` to output
> 
> Result: `3 4 5 + * 6 *`

### Full algorithm (code)

c

```c
int prec_in_str(char ch)     // precedence while still in the input
{
    switch (ch)
    {
        case '(': return 3;      // highest — always gets pushed
        case '*':
        case '/': return 2;
        case '+':
        case '-': return 1;
    }
}

int prec_in_stack(char ch)   // precedence once sitting in the stack
{
    switch (ch)
    {
        case '(': return 0;      // lowest — never popped by an operator
        case '*':
        case '/': return 2;
        case '+':
        case '-': return 1;
    }
}

char in[MAXSIZE + 1];  char *s = in;
char post[MAXSIZE + 1]; char *p = post;
gets(in);

stack_t mystack;
init_stack(&mystack);

char ch, ch1;
while ((ch = *s++) != '\0')
{
    if (ch == ')')
    {
        while ((ch1 = pop(&mystack)) != '(')
        {
            *p++ = ' ';
            *p++ = ch1;
        }
    }
    else if (ch == '(' || ch == '+' || ch == '-' || ch == '*' || ch == '/')
    {
        if (is_empty(&mystack))
        {
            push(&mystack, ch);
        }
        else
        {
            while (!is_empty(&mystack) &&
                   prec_in_str(ch) < prec_in_stack(peek(&mystack)))
            {
                *p++ = ' ';
                *p++ = pop(&mystack);
            }
            push(&mystack, ch);
        }
    }
    else
    {
        *p++ = ch;      // operand — output directly
    }
}
while (!is_empty(&mystack))    // drain remaining operators
{
    *p++ = ' ';
    *p++ = pop(&mystack);
}
*p = '\0';

printf("infix : %s\n", in);
printf("postfix : %s\n", post);
```

> [!question] Why a `while` loop instead of a single `if` when popping by precedence? Because multiple operators of equal-or-higher precedence might be stacked up (e.g. after several `*` in a row) — you need to pop _all_ of them that outrank the incoming operator, not just the very top one.

---

## 10. Summary — why this all matters

- The stack is a **type**, just like `int`. You don't need to know whether `int` addition happens via ripple-carry or some other circuit inside the CPU to use `+`. Likewise, you don't need to know whether the stack is array- or list-based to `push()`/`pop()`.
- **Same interface (`stack.h`), two implementations (`stack.c` array vs. list)** — the applications above (`client.c` files) never change regardless of which `stack.c` they're linked against. This _is_ the point of an ADT.
- One data structure (stack) elegantly solves four very different-looking problems: reversing a string, bracket matching, postfix evaluation, and infix→postfix conversion — all because each problem has an underlying **LIFO** structure.

---

## 11. Quick self-test

- [ ]  Implement `peek(index)` for the array-based stack.
- [ ]  Modify the list-based `deinit_stack` to also print each freed character (trace memory cleanup).
- [ ]  Extend well-formed-bracket checking to also validate the vinculum-equivalent — e.g. treat a custom symbol.
- [ ]  Handle multi-digit **and negative** numbers in postfix evaluation.
- [ ]  Extend infix→postfix to support `^` (exponentiation, right-associative) — note right-associativity changes the pop condition from `<` to `<=`.

> [!info] Source Notes consolidated from handwritten "Introduction to Stack Data Structure" notes plus course material/code: `L8_stack_array_list`, `L9_stack_appln` (bracket matching), `L10_stack_appln1` (postfix evaluation & infix-to-postfix conversion).
