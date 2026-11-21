---
title: 01 — Heap Initialisation and Display
tags: [unit-3, c, code]
---

# 01 — Heap Initialisation and Display

**Source file:** `Original Source Files/heap.c`  
**Purpose:** Provides basic max-heap initialisation, reset, display and swapping.

## Explanation

The heap is 1-based: index 1 contains the root, and index 0 is unused.

## Code

```c
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
```

## Related files

- [[05 - Heap and Queue/00 - Complete Walkthrough]]
- [[05 - Heap and Queue/Original Source Files/README - Source Files]]
