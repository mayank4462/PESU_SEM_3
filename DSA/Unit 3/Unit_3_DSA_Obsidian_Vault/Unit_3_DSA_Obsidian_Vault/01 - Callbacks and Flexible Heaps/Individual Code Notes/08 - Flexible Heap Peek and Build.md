---
title: 08 — Flexible Heap Peek and Build
tags: [unit-3, c, code]
---

# 08 — Flexible Heap Peek and Build

**Source file:** `Original Source Files/heap.c`  
**Purpose:** Reads the root without removal and constructs a heap bottom-up.

## Explanation

Bottom-up construction starts from the last internal node and heapifies towards the root.

## Code

```c
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

## Related files

- [[01 - Callbacks and Flexible Heaps/00 - Complete Walkthrough]]
- [[01 - Callbacks and Flexible Heaps/Original Source Files/README - Source Files]]
