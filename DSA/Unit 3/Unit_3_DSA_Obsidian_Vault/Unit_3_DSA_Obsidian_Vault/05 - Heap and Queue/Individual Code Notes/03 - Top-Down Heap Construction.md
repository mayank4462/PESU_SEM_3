---
title: 03 — Top-Down Heap Construction
tags: [unit-3, c, code]
---

# 03 — Top-Down Heap Construction

**Source file:** `Original Source Files/heap.c`  
**Purpose:** Builds a heap by inserting input values one at a time.

## Explanation

Repeated insertion takes O(n log n) in the worst case.

## Code

```c
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
```

## Related files

- [[05 - Heap and Queue/00 - Complete Walkthrough]]
- [[05 - Heap and Queue/Original Source Files/README - Source Files]]
