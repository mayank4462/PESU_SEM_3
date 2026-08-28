# Implementing Stack using Arrays

- Fixed size array creation ✓
- Top Element ✓

## Structure of Stack

```c
struct stack {
    int size;
    int top;
    int *arr;
};
```
## Creating the Stack

```c
struct stack s;

s.size = 80;
s.top = -1;
s.arr = (int *)malloc(s.size * sizeof(int));
```

## Stack Representation

```
        ↑ top
        │
      ┌───┐
      │ 4 │
      ├───┤
      │ 3 │
      ├───┤
      │ 2 │
      ├───┤
      │ 1 │
      └───┘
```

- `top` points to the **top element** of the stack.
- Initially, `top = -1` because the stack is empty.


# Stack Using Array in C

## Structure of Stack

```c
struct stack {
    int size;
    int top;
    int *arr;
};
```

- `size` → stores the maximum size of the stack.
    
- `top` → stores the index of the top element.
    
- `arr` → pointer to dynamically allocated array.
    
- Initially, `top = -1` because the stack is empty.
    

---

## Check if Stack is Empty

```c
int isEmpty(struct stack *ptr) {
    if (ptr->top == -1) {
        return 1;
    }
    else {
        return 0;
    }
}
```

### Condition

```c
ptr->top == -1
```

- If `top == -1` → Stack is **empty** → returns `1`.
    
- Otherwise → Stack is **not empty** → returns `0`.
    

---

## Check if Stack is Full

```c
int isFull(struct stack *ptr) {
    if (ptr->top == ptr->size - 1) {
        return 1;
    }
    else {
        return 0;
    }
}
```

### Condition

```c
ptr->top == ptr->size - 1
```

- Array indexing starts from `0`.
    
- Therefore, the last valid index is `size - 1`.
    
- If `top == size - 1` → Stack is **full**.
    

---

## Push Operation

The **push** operation is used to insert an element into the stack.

```c
void push(struct stack *ptr, int val) {
    if (isFull(ptr)) {
        printf("Stack Overflow\n");
    }
    else {
        ptr->top++;
        ptr->arr[ptr->top] = val;
    }
}
//Nothing is returned
```

### Push Logic

```text
Before Push:

top → 2
       ↓
     ┌───┐
     │ 3 │
     ├───┤
     │ 2 │
     ├───┤
     │ 1 │
     └───┘

Push(4)

1. top++
2. arr[top] = 4

After Push:

top → 3
       ↓
     ┌───┐
     │ 4 │
     ├───┤
     │ 3 │
     ├───┤
     │ 2 │
     ├───┤
     │ 1 │
     └───┘
```

---

## Pop Operation

The **pop** operation removes the top element from the stack.

```c
int pop(struct stack *ptr) {
    if (isEmpty(ptr)) {
        printf("Stack Underflow\n");
        return -1;
    }
    else {
        int val = ptr->arr[ptr->top];
        //store the topmost value of the stack
        ptr->top--;
        //ptr->top=ptr->top-1;
        return val;
    }
}
```

### Pop Logic

```text
Before Pop:

top → 3
       ↓
     ┌───┐
     │ 4 │  ← removed
     ├───┤
     │ 3 │
     ├───┤
     │ 2 │
     ├───┤
     │ 1 │
     └───┘

1. Store arr[top] in val
2. top--
3. Return val
```

---

## Peek Operation

The **peek** operation is used to see the top element without removing it.

```c
int peek(struct stack *ptr) {
    if (isEmpty(ptr)) {
        printf("Stack is empty\n");
        return -1;
    }
    else {
        return ptr->arr[ptr->top];
    }
}
```

---

# Complete Program

```c
#include <stdio.h>
#include <stdlib.h>

struct stack {
    int size;
    int top;
    int *arr;
};

int isEmpty(struct stack *ptr) {
    if (ptr->top == -1) {
        return 1;
    }
    else {
        return 0;
    }
}

int isFull(struct stack *ptr) {
    if (ptr->top == ptr->size - 1) {
        return 1;
    }
    else {
        return 0;
    }
}

void push(struct stack *ptr, int val) {
    if (isFull(ptr)) {
        printf("Stack Overflow\n");
    }
    else {
        ptr->top++;
        ptr->arr[ptr->top] = val;
    }
}

int pop(struct stack *ptr) {
    if (isEmpty(ptr)) {
        printf("Stack Underflow\n");
        return -1;
    }
    else {
        int val = ptr->arr[ptr->top];
        ptr->top--;
        return val;
    }
}

int peek(struct stack *ptr) {
    if (isEmpty(ptr)) {
        printf("Stack is empty\n");
        return -1;
    }
    else {
        return ptr->arr[ptr->top];
    }
}

int main() {

    struct stack *s;

    s = (struct stack *)malloc(sizeof(struct stack));

    s->size = 80;
    s->top = -1;
    s->arr = (int *)malloc(s->size * sizeof(int));

    // Check if stack is empty
    if (isEmpty(s)) {
        printf("The stack is empty\n");
    }
    else {
        printf("The stack is not empty\n");
    }

    // Pushing elements
    push(s, 7);
    push(s, 8);
    push(s, 9);
    push(s, 10);

    printf("Top element: %d\n", peek(s));

    // Popping an element
    printf("Popped element: %d\n", pop(s));

    printf("Top element after pop: %d\n", peek(s));

    // Check if stack is empty
    if (isEmpty(s)) {
        printf("The stack is empty\n");
    }
    else {
        printf("The stack is not empty\n");
    }

    // Free allocated memory
    free(s->arr);
    free(s);

    return 0;
}
```

## Important Points

- **Stack follows LIFO** → Last In, First Out.
    
- `top = -1` → Stack is initially empty.
    
- `top++` → Used before inserting an element.
    
- `top--` → Used after removing an element.
    
- `isEmpty()` checks whether the stack is empty.
    
- `isFull()` checks whether the stack is full.
    
- **Push** → Insert an element.
    
- **Pop** → Remove the top element.
    
- **Peek** → View the top element without removing it.
    
- **Stack Overflow** → Trying to push into a full stack.
    
- **Stack Underflow** → Trying to pop from an empty stack.

# Stack Operations While Implementing Using Arrays

## Stack Using Array

Consider an array of size **6**:

```
┌───┬───┬───┬───┬───┬───┐
│   │   │   │   │   │   │
└───┴───┴───┴───┴───┴───┘
  0   1   2   3   4   5
```

### Important Conditions

- **Cannot Push if Stack is Full**
- **Cannot Pop if Stack is Empty**

---

## Structure of Stack

```c
struct stack {
    int size;
    int top;
    int *arr;
};
```

### Members

- `size` → Size of the stack
- `top` → Stores the index of the top element
- `arr` → Pointer to the array used to store stack elements

---

## Creating a Stack

There are two ways to create a stack.

### 1. Creating a Stack Variable

```c
struct stack s;
```

Access the members using the **dot (`.`) operator**:

```c
s.size
s.top
s.arr
```


Continuation/Separate main method for the above code 

## Main Function

```c
int main() {
    struct stack *sp = (struct stack *) malloc(sizeof(struct stack));
    sp->size = 10;
    sp->top = -1;
    sp->arr = (int *) malloc(sp->size * sizeof(int));
    printf("Stack has been created successfully\n");

    printf("Before pushing, Full: %d\n", isFull(sp));
    printf("Before pushing, Empty: %d\n", isEmpty(sp));

    push(sp, 1);
    push(sp, 23);
    push(sp, 99);
    push(sp, 75);
    push(sp, 3);
    push(sp, 64);
    push(sp, 57);
    push(sp, 46);
    push(sp, 89);
    push(sp, 6); // --> Pushed 10 values

    // push(sp, 46); // Stack Overflow since the size of the stack is 10

    printf("After pushing, Full: %d\n", isFull(sp));
    printf("After pushing, Empty: %d\n", isEmpty(sp));

    printf("Popped %d from the stack\n", pop(sp)); // --> Last in first out!
    printf("Popped %d from the stack\n", pop(sp)); // --> Last in first out!
    printf("Popped %d from the stack\n", pop(sp)); // --> Last in first out!

    return 0;
}
```



# Peek Operation in Stack Using Array

## Stack Representation

```text
Array Index:    0     1     2     3     4     5
              ┌─────┬─────┬─────┬─────┬─────┬─────┐
              │  7  │  8  │ 12  │     │     │     │
              └─────┴─────┴─────┴─────┴─────┴─────┘
                              ↑
                             Top
```

Here:

```text
top = 2
```

The stack contains:

```text
Position 1 → 12
Position 2 → 8
Position 3 → 7
```

> **Position is counted from the top of the stack, not from the beginning of the array.**

---

## Peek Operation

The `peek()` operation is used to access an element at a particular **position from the top** without removing it.

### Formula

```text
Array Index = top - i + 1
```

Where:

- `top` → index of the top element
- `i` → position from the top
- `+1` → because positions start from `1`

### Example

If:

```text
top = 2
```

Then:

| Position (`i`) | Array Index | Element |
|---:|---:|---:|
| 1 | `2 - 1 + 1 = 2` | 12 |
| 2 | `2 - 2 + 1 = 1` | 8 |
| 3 | `2 - 3 + 1 = 0` | 7 |

---

## `peek()` Function

```c
int peek(struct stack *sp, int i) {
    if (sp->top - i + 1 < 0) {
        printf("Not a valid position\n");
        return -1;
    }
    else {
        return sp->arr[sp->top - i + 1];
    }
}
```

---

## Understanding the Condition

```c
sp->top - i + 1 < 0
```

This checks whether the requested position is **invalid**.

For example, if:

```text
top = 2
i = 4
```

Then:

```text
top - i + 1
= 2 - 4 + 1
= -1
```

Since the array index becomes negative, the position is invalid.

Therefore:

```text
Not a valid position
```

is printed and the function returns:

```c
-1
```

---

## Example

```c
peek(sp, 1);
```

Returns the **top element**.

```c
peek(sp, 2);
```

Returns the **second element from the top**.

```c
peek(sp, 3);
```

Returns the **third element from the top**.

### Remember

> `peek(sp, 1)` → Top element

> `peek(sp, 2)` → Second element from top

> `peek(sp, 3)` → Third element from top

The general formula is:

```text
Position from top → Array index

        i  →  top - i + 1
```