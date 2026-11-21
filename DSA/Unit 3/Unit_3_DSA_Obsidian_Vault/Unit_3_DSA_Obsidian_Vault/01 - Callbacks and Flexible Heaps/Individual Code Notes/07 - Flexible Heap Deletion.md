---
title: 07 — Flexible Heap Deletion
tags: [unit-3, c, code]
---

# 07 — Flexible Heap Deletion

**Source file:** `Original Source Files/heap.c`  
**Purpose:** Removes the root, moves the final element to the root, then restores heap order downward.

## Explanation

The function name `delete_max` is retained from the supplied source. With the min comparator, it actually removes the minimum.

## Code

```c
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
```

## Related files

- [[01 - Callbacks and Flexible Heaps/00 - Complete Walkthrough]]
- [[01 - Callbacks and Flexible Heaps/Original Source Files/README - Source Files]]
