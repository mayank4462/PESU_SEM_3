---
title: Callbacks Flexible Heaps and Running Median
tags: [unit-3, c, function-pointers, callbacks, heap, running-median]
---

# Callbacks, Flexible Heaps and Running Median

## 1. What this project demonstrates

This combined program has three menu-driven parts:

1. **Map / Filter / Reduce** using function pointers (callbacks).
2. **Flexible heap** that can be configured as a max-heap or min-heap using a comparator callback.
3. **Running median** using two heaps: a max-heap for the lower half and a min-heap for the upper half.

The heap uses **1-based indexing**. `key_[0]` is unused, and valid heap positions are `1` through `n_`.

## 2. Callback functions and function pointers

A function pointer lets a function receive another function as an argument. In this project:

- `int (*op)(int)` represents a callback that accepts one integer and returns an integer.
- `int (*op)(int, int)` represents a callback that accepts two integers and returns an integer.
- `mymap` applies a transformation to every element.
- `myfilter` copies only elements for which the callback returns non-zero.
- `myreduce` combines all elements into a single accumulated result.

### Expected callback example

For `a = {1, 2, 3, 4, 5}`:

| Operation | Callback | Result |
|---|---|---|
| Map | `sq(x) = x*x` | `1 4 9 16 25` |
| Filter | `is_even(x)` | `2 4` |
| Reduce | `add(x,y) = x+y`, initial value `0` | `15` |

`myfilter` receives `&m` because it must return two things: the filtered array through `b[]`, and the number of valid output elements through `m`.

## 3. Flexible heap using a comparator

The structure stores a callback:

`int (*compare)(int, int);`

The callback defines which value has higher priority.

- `max(x, y)` returns true when `x > y`, producing a max-heap.
- `min(x, y)` returns true when `x < y`, producing a min-heap.

The same heap code can therefore implement both heap types. `heapify()` selects the higher-priority child and moves it upward into the current hole. `insert()` places a value at the end and percolates it upward.

**Naming caution:** `delete_max()` and `peek_max()` are historical names from the supplied code. With the min comparator, they delete or return the minimum element at the root.

### Heap operations

| Operation | Main idea | Time |
|---|---|---:|
| `insert` | Add at end, move toward root while higher priority than parent | `O(log n)` |
| `delete_max` | Save root, move last element to root, heapify down | `O(log n)` |
| `peek_max` | Return root | `O(1)` |
| `build_heap` | Copy values, heapify internal nodes from `n/2` down to `1` | `O(n)` |

## 4. Running median using two heaps

Maintain two partitions of the numbers seen so far:

- `left_half`: max-heap containing the lower half. Its root is the largest value in the lower half.
- `right_half`: min-heap containing the upper half. Its root is the smallest value in the upper half.

The invariants are:

1. Every value in `left_half` is less than or equal to every value in `right_half`.
2. `left_half` has either the same number of elements as `right_half`, or exactly one more.

### Insertion and rebalancing

1. If the left heap is empty or `x <= peek_max(left_half)`, insert `x` into the left heap.
2. Otherwise, insert `x` into the right heap.
3. If the left side has more than one extra element, move its root to the right side.
4. If the right side becomes larger, move its root (the smallest upper-half value) to the left side.

### Median calculation

- Odd total count: the left side has one extra element, so the median is `peek_max(left_half)`.
- Even total count: the median is the average of `peek_max(left_half)` and `peek_max(right_half)`. The second call returns the minimum upper-half value because `right_half` is configured as a min-heap.

The median is produced after each insertion, so the program displays the running median for every prefix of the input sequence.

## 5. Source code

The following listings include all source and build/documentation files from the experiential project.

### `function.h`

```c
#ifndef FUNCTION_H
#define FUNCTION_H

void mymap(int a[], int b[], int n, int (*op)(int));
void myfilter(int a[], int n, int b[], int *ptr_m, int (*op)(int));
int myreduce(int a[], int n, int init, int (*op)(int, int));

#endif
```
### `function.c`

```c
#include "function.h"

// Apply op to every input element and store the transformed values in b[].
void mymap(int a[], int b[], int n, int (*op)(int))
{
    for(int i = 0; i < n; ++i)
    {
        b[i] = op(a[i]);
    }
}

// ptr_m returns the number of elements copied into the filtered output.
void myfilter(int a[], int n, int b[], int *ptr_m, int (*op)(int))
{
    *ptr_m = 0;

    for(int i = 0; i < n; ++i)
    {
        if(op(a[i]))
        {
            b[*ptr_m] = a[i];
            ++(*ptr_m);
        }
    }
}

// Fold all elements into one result, starting from init.
int myreduce(int a[], int n, int init, int (*op)(int, int))
{
    for(int i = 0; i < n; ++i)
    {
        init = op(init, a[i]);
    }

    return init;
}
```
### `heap.h`

```c
#ifndef HEAP_H
#define HEAP_H

#define MAXSIZE 200

struct heap
{
    int key_[MAXSIZE + 1];
    int n_;
    int (*compare)(int, int);
};
typedef struct heap heap_t;

void init(heap_t *ptr_heap, int (*compare)(int, int));
void deinit(heap_t *ptr_heap);
void disp(heap_t *ptr_heap);
void insert(heap_t *ptr_heap, int key);
int delete_max(heap_t *ptr_heap);
int peek_max(heap_t *ptr_heap);
void build_heap(heap_t *ptr_heap, int a[], int n);

#endif
```
### `heap.c`

```c
#include <stdio.h>
#include <stdlib.h>
#include "heap.h"

void init(heap_t *ptr_heap, int (*compare)(int, int))
{
    if(compare == NULL)
    {
        printf("comparison function missing\n");
        exit(1);
    }

    ptr_heap->n_ = 0;
    ptr_heap->compare = compare;
}

void deinit(heap_t *ptr_heap)
{
    ptr_heap->n_ = 0;
    ptr_heap->compare = NULL;
}

void disp(heap_t *ptr_heap)
{
    printf("\n displaying the heap \n");

    for(int i = 1; i <= ptr_heap->n_; ++i)
    {
        printf("%d ", ptr_heap->key_[i]);
    }

    printf("\n");
}

static void heapify(int a[], int n, int pos,
                    int (*compare)(int, int))
{
    int i = pos;
    int j = 2 * i;
    int val = a[i];
    int is_heap = 0;

    while(j <= n && !is_heap)
    {
        // Select the child with higher priority.
        if(j + 1 <= n && compare(a[j + 1], a[j]))
            ++j;

        // Move that child up if it has higher priority than val.
        if(compare(a[j], val))
        {
            a[i] = a[j];
            i = j;
            j = 2 * i;
        }
        else
        {
            is_heap = 1;
        }
    }

    // Put the saved value into the final hole.
    a[i] = val;
}

static void swap(int *p, int *q)
{
    int temp = *p;
    *p = *q;
    *q = temp;
}

void insert(heap_t *ptr_heap, int key)
{
    if(ptr_heap->n_ == MAXSIZE)
    {
        printf("heap full\n");
        return;
    }

    // Add at the end, then percolate upwards.
    int pos = ++ptr_heap->n_;
    ptr_heap->key_[pos] = key;

    while(pos > 1 &&
          ptr_heap->compare(ptr_heap->key_[pos],
                            ptr_heap->key_[pos / 2]))
    {
        swap(&ptr_heap->key_[pos], &ptr_heap->key_[pos / 2]);
        pos = pos / 2;
    }
}

int delete_max(heap_t *ptr_heap)
{
    if(ptr_heap->n_ == 0)
    {
        printf("heap empty\n");
        exit(1);
    }

    int res = ptr_heap->key_[1];
    ptr_heap->key_[1] = ptr_heap->key_[ptr_heap->n_];
    --ptr_heap->n_;

    // Do not heapify an empty heap.
    if(ptr_heap->n_ > 0)
        heapify(ptr_heap->key_, ptr_heap->n_, 1, ptr_heap->compare);

    return res;
}

int peek_max(heap_t *ptr_heap)
{
    if(ptr_heap->n_ == 0)
    {
        printf("heap empty\n");
        exit(1);
    }

    return ptr_heap->key_[1];
}

void build_heap(heap_t *ptr_heap, int a[], int n)
{
    if(n < 0 || n > MAXSIZE)
    {
        printf("invalid heap size\n");
        return;
    }

    ptr_heap->n_ = n;

    // Input array a is 0-based; heap storage is 1-based.
    for(int i = 1; i <= n; ++i)
    {
        ptr_heap->key_[i] = a[i - 1];
    }

    // Bottom-up heap construction.
    for(int i = n / 2; i >= 1; --i)
    {
        heapify(ptr_heap->key_, ptr_heap->n_, i, ptr_heap->compare);
    }
}
```
### `main.c`

```c
#include <stdio.h>
#include "function.h"
#include "heap.h"

/* STEP 1: callback functions */
int sq(int x)
{
    return x * x;
}

int is_even(int x)
{
    return x % 2 == 0;
}

int add(int x, int y)
{
    return x + y;
}

void disp_array(int a[], int n)
{
    for(int i = 0; i < n; ++i)
    {
        printf("%d ", a[i]);
    }
    printf("\n");
}

void run_callbacks(void)
{
    int a[] = {1, 2, 3, 4, 5};
    int n = 5;
    int b[5];
    int m;

    printf("\nOriginal array : ");
    disp_array(a, n);

    mymap(a, b, n, sq);
    printf("After map (square) : ");
    disp_array(b, n);

    myfilter(a, n, b, &m, is_even);
    printf("After filter (even numbers) : ");
    disp_array(b, m);

    int sum = myreduce(a, n, 0, add);
    printf("After reduce (sum) : %d\n", sum);
}

/* STEP 2: comparison functions decide the heap type */
int max(int x, int y)
{
    return x > y;
}

int min(int x, int y)
{
    return x < y;
}

void run_flexible_heap(void)
{
    heap_t h;
    int heap_type;
    int opt;
    int key;

    printf("\n1: max heap  2: min heap : ");
    if(scanf("%d", &heap_type) != 1)
        return;

    if(heap_type == 1)
    {
        init(&h, max);
    }
    else if(heap_type == 2)
    {
        init(&h, min);
    }
    else
    {
        printf("invalid heap type\n");
        return;
    }

    printf("opt : 1 insert; 2 : delete; 0 : exit : ");
    if(scanf("%d", &opt) != 1)
    {
        deinit(&h);
        return;
    }

    while(opt != 0)
    {
        switch(opt)
        {
            case 1:
                printf("enter key : ");
                if(scanf("%d", &key) != 1)
                {
                    deinit(&h);
                    return;
                }
                insert(&h, key);
                disp(&h);
                break;

            case 2:
                if(h.n_ == 0)
                {
                    printf("heap empty\n");
                }
                else
                {
                    key = delete_max(&h);
                    printf("deleted root : %d\n", key);
                    disp(&h);
                }
                break;

            default:
                printf("invalid option\n");
                break;
        }

        printf("opt : 1 insert; 2 : delete; 0 : exit : ");
        if(scanf("%d", &opt) != 1)
            break;
    }

    deinit(&h);
}

/* STEP 3: Running median using two flexible heaps */
static void insert_number(heap_t *left_half,
                          heap_t *right_half, int x)
{
    // Left is a Max Heap; right is a Min Heap.
    if(left_half->n_ == 0 || x <= peek_max(left_half)) // if left is empty or x <= max of left
    {
        insert(left_half, x); // insert into left half
    }
    else
    {
        insert(right_half, x); // insert into right half
    }

    // Left half may have at most one extra element.
    if(left_half->n_ > right_half->n_ + 1) // if left has more than one extra element
    {
        int temp = delete_max(left_half);
        insert(right_half, temp);
    }

    // If right has more elements, move its minimum to left.
    if(right_half->n_ > left_half->n_)
    {
        int temp = delete_max(right_half);
        insert(left_half, temp);
    }
}

static double get_median(heap_t *left_half, heap_t *right_half)
{
    // This branch is used when the total number of elements is odd.
    if(left_half->n_ > right_half->n_)
    {
        return (double)peek_max(left_half);
    }

    // Equal sizes: average the largest lower-half value and
    // the smallest upper-half value.
    return ((double)peek_max(left_half) +
            (double)peek_max(right_half)) / 2.0;
}

void run_running_median(void)
{
    heap_t left_half;
    heap_t right_half;
    int n;
    int x;

    init(&left_half, max);
    init(&right_half, min);

    printf("\nnumber of elements (1-%d) : ", MAXSIZE);
    if(scanf("%d", &n) != 1 || n < 1 || n > MAXSIZE)
    {
        printf("invalid number of elements\n");
        deinit(&left_half);
        deinit(&right_half);
        return;
    }

    for(int i = 0; i < n; ++i)
    {
        printf("enter number : ");
        if(scanf("%d", &x) != 1)
        {
            printf("invalid input\n");
            deinit(&left_half);
            deinit(&right_half);
            return;
        }

        insert_number(&left_half, &right_half, x);

        printf("running median : %.1f\n",
               get_median(&left_half, &right_half));
    }

    deinit(&left_half);
    deinit(&right_half);
}

int main(void)
{
    int choice;

    do
    {
        printf("\n========== U3 EXPERIENTIAL LEARNING ==========\n");
        printf("1. Map / Filter / Reduce\n");
        printf("2. Flexible Heap (Max / Min)\n");
        printf("3. Running Median\n");
        printf("0. Exit\n");
        printf("Enter choice : ");

        if(scanf("%d", &choice) != 1)
            return 1;

        switch(choice)
        {
            case 1:
                run_callbacks();
                break;

            case 2:
                run_flexible_heap();
                break;

            case 3:
                run_running_median();
                break;

            case 0:
                printf("exiting...\n");
                break;

            default:
                printf("invalid choice\n");
                break;
        }

    } while(choice != 0);

    return 0;
}
```
### `Makefile`

```makefile
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -pedantic

all: client.exe

client.exe: main.o function.o heap.o
	$(CC) $(CFLAGS) main.o function.o heap.o -o client.exe

main.o: main.c function.h heap.h
	$(CC) $(CFLAGS) -c main.c

function.o: function.c function.h
	$(CC) $(CFLAGS) -c function.c

heap.o: heap.c heap.h
	$(CC) $(CFLAGS) -c heap.c

clean:
	rm -f *.o client.exe
```
### `README.txt`

```text
U3 EXPERIENTIAL LEARNING - COMBINED PROJECT
===========================================

This project combines the three professor-provided steps into one C program.

FILES
-----
main.c      Main menu, callback examples, max/min callback functions,
            and running-median algorithm.
function.h  Callback function declarations.
function.c  Map, filter and reduce implementations.
heap.h      Shared flexible heap structure and declarations.
heap.c      Comparator-driven heap operations.
Makefile    Builds the complete project as client.exe.

BUILD
-----
make

RUN
---
./client.exe

MENU
----
1. Map / Filter / Reduce
2. Flexible Heap (Max / Min)
3. Running Median
0. Exit

NOTES
-----
1. The heap uses 1-based indexing, so key_ has MAXSIZE + 1 entries.
2. build_heap() accepts a normal 0-based input array a[0] through a[n-1].
3. delete_max() and peek_max() retain the names from the provided course code.
   With a Min Heap, they operate on its root, which is the minimum element.
4. The Step 1 starter files had a naming mismatch: function.h declared
   filter/reduce, while the TODO signatures in function.c used myfilter/
   myreduce. This combined project consistently uses myfilter/myreduce.
5. Running median follows the supplied slides: left_half is a Max Heap,
   right_half is a Min Heap; left has equal size or one extra element.
```
## 6. Build and run

From the directory containing the `Makefile`:

```bash
make
./client.exe
```

To remove object files and the executable:

```bash
make clean
```

## 7. Exam checkpoints

- A callback is passed by its function address; the receiving function invokes it through the function pointer.
- A filter needs an output count because the number of retained elements is not known in advance.
- In a max-heap, the parent is greater than or equal to its children; in a min-heap, the parent is less than or equal to its children.
- For a 1-based heap, parent index is `i/2`, left child is `2*i`, and right child is `2*i+1`.
- Bottom-up heap construction starts at `n/2`, the last internal node; leaves already satisfy the heap property.
- Running median uses opposite heap types so the two roots are the boundary values around the median.
