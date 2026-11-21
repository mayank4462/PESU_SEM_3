---
title: Heap Construction and Queue Wrapper
tags: [unit-3, c, heap, max-heap, priority-queue, queue]
---

# Heap Construction and Queue Wrapper

## 1. Max-heap representation

The heap is stored in an array using **1-based indexing**. `key[0]` is unused.

For a node at index `i`:

- Parent: `i / 2`.
- Left child: `2 * i`.
- Right child: `2 * i + 1`.

A max-heap satisfies `parent >= children`; therefore the maximum value is always at index `1`.

`struct heap` stores `key[MAXSIZE + 1]` and `n`, the number of elements currently in the heap.

## 2. Top-down heap construction

Top-down construction inserts elements one at a time:

1. Increase heap size and place the new value at the end.
2. While it is greater than its parent, swap it with the parent.
3. Stop when the root is reached or the parent is already greater/equal.

`build_heap_top_down()` resets `n` to zero and calls `insert()` for every input element.

Time complexity: `O(n log n)` in the worst case for building the heap by repeated insertion.

## 3. Bottom-up heap construction

Bottom-up construction first copies all input values into the heap array. It then calls `heapify()` for each internal node, starting from `n/2` and moving down to `1`.

Why start at `n/2`? In a 1-based binary heap, every index greater than `n/2` is a leaf. A leaf already satisfies the heap property.

### `heapify()` logic

1. Save the current node's value in `val` and treat its position as a hole.
2. Choose the larger of the existing children.
3. If that child is larger than `val`, move the child into the hole and continue downward.
4. Otherwise, the correct position has been found.
5. Place `val` in the final hole.

The implementation uses a hole-moving technique instead of repeatedly swapping the saved value at every level.

Time complexity: `O(n)` for the full bottom-up build.

**Observation from this source:** `build_heap()` calls `disp()` after each `heapify()`, so intermediate heap states are printed during construction. This is useful for tracing but can be removed for a quiet build.

## 4. Common heap operations

- `insert()`: percolates upward.
- `delete_max()`: stores the root, moves the final element to index `1`, decrements `n`, then heapifies downward.
- `peek_max()`: returns `key[1]` without deleting it.
- `disp()`: prints array order; it does not print a sorted sequence.
- `init()` and `deinit()`: reset the element count to zero.

## 5. Queue wrapper — important behaviour mismatch

The `queue` structure contains a `struct heap`. Its functions delegate directly to heap operations:

- `enqueue()` calls `insert()`.
- `dequeue()` calls `delete_max()`.

Therefore, this implementation returns the **largest key** first. It does not preserve insertion order and is **not a FIFO queue**. It behaves as a max-priority queue.

Example:

- Enqueue: `10`, `20`, `5`.
- The heap root becomes `20`.
- First dequeue returns `20`, not `10`.

This distinction is important in an exam question asking whether the implementation is FIFO or priority-based.

## 6. Client menu

The client menu exposes:

1. Heap insert.
2. Heap delete max.
3. Heap peek max.
4. Display heap.
5. Build heap bottom-up.
6. Build heap top-down.
7. Queue enqueue.
8. Queue dequeue.
9. Display queue.
0. Exit.

For heap construction, input is read into `a[1]` through `a[n]`, so the heap build functions expect a **1-based input array** in this project. This differs from the experiential flexible heap's `build_heap()`, which accepts a 0-based input array.

## 7. Source code

### `heap.h`

```c
#ifndef HEAP_H
#define HEAP_H

#define MAXSIZE 200

struct heap
{
    int key[MAXSIZE + 1];
    int n;
};

void init(struct heap *ptr_heap);
void deinit(struct heap *ptr_heap);
void disp(struct heap *ptr_heap);

void insert(struct heap *ptr_heap, int key);
int delete_max(struct heap *ptr_heap);
int peek_max(struct heap *ptr_heap);

void build_heap(struct heap *ptr_heap, int a[], int n);
void build_heap_top_down(struct heap *ptr_heap, int a[], int n);

#endif
```
### `heap.c`

```c
#include <stdio.h>
#include <stdlib.h>
#include "heap.h"

// COMMON FUNCTIONS

void init(struct heap *ptr_heap)
{
    ptr_heap->n = 0;
}

void deinit(struct heap *ptr_heap)
{
    ptr_heap->n = 0;
}

void disp(struct heap *ptr_heap)
{
    printf("\nDisplaying the heap\n");

    for(int i = 1; i <= ptr_heap->n; ++i)
    {
        printf("%d ", ptr_heap->key[i]);
    }

    printf("\n");
}

static void swap(int *p, int *q)
{
    int temp = *p;
    *p = *q;
    *q = temp;
}


// ========================================
// TOP-DOWN APPROACH
// ========================================

// Insert using percolation upwards
void insert(struct heap *ptr_heap, int key)
{
    if(ptr_heap->n >= MAXSIZE)
    {
        printf("heap full\n");
        exit(1);
    }

    int pos = ++ptr_heap->n; // Increment the size of the heap and get the position for the new key

    ptr_heap->key[pos] = key; // Place the new key at the end of the heap

    while(pos > 1 && ptr_heap->key[pos] > ptr_heap->key[pos / 2]) // While the new key is greater than its parent, swap it with its parent
    {
        swap(&ptr_heap->key[pos], &ptr_heap->key[pos / 2]); // Swap the new key with its parent

        pos = pos / 2; // Move up to the parent's position
    }
}

// Build heap by inserting elements one by one
void build_heap_top_down(struct heap *ptr_heap, int a[], int n)
{
    if(n < 0 || n > MAXSIZE)
    {
        printf("invalid heap size\n");
        exit(1);
    }

    ptr_heap->n = 0;

    for(int i = 1; i <= n; ++i)
    {
        insert(ptr_heap, a[i]);
    }
}


// ========================================
// BOTTOM-UP APPROACH
// ========================================

// Heapify downwards
static void heapify(int a[], int n, int pos)
{
    int i = pos; // Start from the given position
    int j = 2 * i; // Left child of the current node
    int val = a[i]; // Store the value of the current node
    int is_heap = 0; // Flag to indicate if the heap property is satisfied

    while(j <= n && !is_heap) // While the left child exists and the heap property is not satisfied // less than or equal to n because 1 based indexing
    {
        if(j + 1 <= n && a[j + 1] > a[j]) // If the right child exists and is greater than the left child
            ++j; // Move to the right child

        if(a[j] > val) // If the child is greater than the current node, swap them
        {
            a[i] = a[j]; // Move the child up
            i = j; // Move down to the child's position
            j = 2 * i; // Update the left child index
        }
        else
        {
            is_heap = 1; // The heap property is satisfied, exit the loop
        }
    }

    a[i] = val; // Place the original value in its correct position
}

// Build heap using bottom-up heapification
void build_heap(struct heap *ptr_heap, int a[], int n)
{
    if(n < 0 || n > MAXSIZE)
    {
        printf("invalid heap size\n");
        exit(1);
    }

    ptr_heap->n = n;

    for(int i = 1; i <= n; ++i)
    {
        ptr_heap->key[i] = a[i];
    }

    for(int i = n / 2; i >= 1; --i)
    {
        heapify(ptr_heap->key, ptr_heap->n, i);
        disp(ptr_heap);
    }
}


// ========================================
// COMMON HEAP OPERATIONS
// ========================================

int delete_max(struct heap *ptr_heap)
{
    if(ptr_heap->n == 0)
    {
        printf("heap empty\n");
        exit(1);
    }

    int res = ptr_heap->key[1]; // Store the maximum value (root of the heap)

    ptr_heap->key[1] = ptr_heap->key[ptr_heap->n]; // Move the last element to the root

    --ptr_heap->n; // Decrease the size of the heap

    if(ptr_heap->n > 0)
        heapify(ptr_heap->key, ptr_heap->n, 1); // Restore the heap property by heapifying down from the root

    return res;
}

int peek_max(struct heap *ptr_heap)
{
    if(ptr_heap->n == 0)
    {
        printf("heap empty\n");
        exit(1);
    }

    return ptr_heap->key[1];
}
```
### `queue.h`

```c
#ifndef QUEUE_H
#define QUEUE_H

#include "heap.h"

struct queue
{
    struct heap q;
};

void init_queue(struct queue *ptr_queue);
void deinit_queue(struct queue *ptr_queue);

void enqueue(struct queue *ptr_queue, int key);
int dequeue(struct queue *ptr_queue);

int is_empty(struct queue *ptr_queue);
int is_full(struct queue *ptr_queue);

void disp_queue(struct queue *ptr_queue);

#endif
```
### `queue.c`

```c
#include <stdio.h>
#include "queue.h"

void init_queue(struct queue *ptr_queue)
{
    init(&ptr_queue->q);
}

void deinit_queue(struct queue *ptr_queue)
{
    deinit(&ptr_queue->q);
}

// This wrapper inserts into a max-heap; it is not FIFO queue insertion.
void enqueue(struct queue *ptr_queue, int key)
{
    insert(&ptr_queue->q, key);
}

// delete_max removes the largest value, not the oldest inserted value.
int dequeue(struct queue *ptr_queue)
{
    return delete_max(&ptr_queue->q);
}

int is_empty(struct queue *ptr_queue)
{
    return ptr_queue->q.n == 0;
}

int is_full(struct queue *ptr_queue)
{
    return ptr_queue->q.n == MAXSIZE;
}

void disp_queue(struct queue *ptr_queue)
{
    disp(&ptr_queue->q);
}
```
### `client.c`

```c
#include <stdio.h>
#include "heap.h"
#include "queue.h"

int main()
{
    struct heap h;
    struct queue q;

    init(&h);
    init_queue(&q);

    int opt;
    int key;
    int n;
    int a[MAXSIZE + 1];

    printf("\n1 : Heap insert");
    printf("\n2 : Heap delete max");
    printf("\n3 : Heap peek max");
    printf("\n4 : Display heap");
    printf("\n5 : Build heap bottom-up");
    printf("\n6 : Build heap top-down");
    printf("\n7 : Queue enqueue");
    printf("\n8 : Queue dequeue");
    printf("\n9 : Display queue");
    printf("\n0 : Exit");

    printf("\n\nEnter option : ");
    scanf("%d", &opt);

    while(opt != 0)
    {
        switch(opt)
        {
            case 1:
                printf("Enter key : ");
                scanf("%d", &key);

                insert(&h, key);
                disp(&h);
                break;

            case 2:
                if(h.n == 0)
                    printf("heap empty\n");
                else
                {
                    key = delete_max(&h);
                    printf("deleted : %d\n", key);
                    disp(&h);
                }
                break;

            case 3:
                if(h.n == 0)
                    printf("heap empty\n");
                else
                    printf("maximum : %d\n", peek_max(&h));
                break;

            case 4:
                disp(&h);
                break;

            case 5:
            case 6:
                printf("Enter number of elements : ");
                scanf("%d", &n);

                if(n < 0 || n > MAXSIZE)
                {
                    printf("invalid heap size\n");
                    break;
                }

                printf("Enter elements : ");

                for(int i = 1; i <= n; ++i)
                    scanf("%d", &a[i]);

                if(opt == 5)
                {
                    build_heap(&h, a, n);
                    printf("Bottom-up heap constructed\n");
                }
                else
                {
                    build_heap_top_down(&h, a, n);
                    printf("Top-down heap constructed\n");
                }

                disp(&h);
                break;

            case 7:
                if(is_full(&q))
                    printf("queue full\n");
                else
                {
                    printf("Enter key : ");
                    scanf("%d", &key);

                    enqueue(&q, key);
                    disp_queue(&q);
                }
                break;

            case 8:
                if(is_empty(&q))
                    printf("queue empty\n");
                else
                {
                    key = dequeue(&q);
                    printf("dequeue : %d\n", key);
                    disp_queue(&q);
                }
                break;

            case 9:
                disp_queue(&q);
                break;

            default:
                printf("invalid option\n");
        }

        printf("\nEnter option : ");
        scanf("%d", &opt);
    }

    deinit(&h);
    deinit_queue(&q);

    return 0;
}
```
## 8. Complexity and exam checkpoints

| Operation | Time complexity |
|---|---:|
| Peek max | `O(1)` |
| Insert | `O(log n)` |
| Delete max | `O(log n)` |
| Top-down build using repeated insert | `O(n log n)` |
| Bottom-up build | `O(n)` |
| Display | `O(n)` |

Remember:

- A heap is a complete binary tree, but its array representation is not sorted.
- The root of a max-heap is the maximum; the root of a min-heap is the minimum.
- `heapify()` restores the heap property downward from a node.
- In bottom-up construction, only internal nodes need heapification.
- A queue must remove the oldest item to be FIFO. This wrapper does not do that.
