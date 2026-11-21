---
title: 09 — Running Median
tags: [unit-3, c, code]
---

# 09 — Running Median

**Source file:** `Original Source Files/main.c`  
**Purpose:** Maintains a running median with two heaps: a max-heap for the lower half and a min-heap for the upper half.

## Explanation

The left heap may contain one extra element. Its root is the median for odd-sized prefixes; the mean of both roots is used for even-sized prefixes.

## Code

```c
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
```

## Related files

- [[01 - Callbacks and Flexible Heaps/00 - Complete Walkthrough]]
- [[01 - Callbacks and Flexible Heaps/Original Source Files/README - Source Files]]
