---
title: 06 — Flexible Heap Insertion
tags: [unit-3, c, code]
---

# 06 — Flexible Heap Insertion

**Source file:** `Original Source Files/heap.c`  
**Purpose:** Inserts a value and restores heap order by moving it upwards.

## Explanation

The comparison callback controls whether the structure behaves as a max-heap or min-heap.

## Code

```c
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
```

## Related files

- [[01 - Callbacks and Flexible Heaps/00 - Complete Walkthrough]]
- [[01 - Callbacks and Flexible Heaps/Original Source Files/README - Source Files]]
