---
title: 05 — Delete Maximum and Peek Maximum
tags: [unit-3, c, code]
---

# 05 — Delete Maximum and Peek Maximum

**Source file:** `Original Source Files/heap.c`  
**Purpose:** Removes the maximum at the root or reads it without removal.

## Explanation

After deletion, the last element replaces the root and is sifted down to restore heap order.

## Code

```c
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

## Related files

- [[05 - Heap and Queue/00 - Complete Walkthrough]]
- [[05 - Heap and Queue/Original Source Files/README - Source Files]]
