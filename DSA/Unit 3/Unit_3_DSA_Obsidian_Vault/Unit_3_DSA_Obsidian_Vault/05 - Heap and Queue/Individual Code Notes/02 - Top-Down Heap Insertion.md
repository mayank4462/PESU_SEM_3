---
title: 02 — Top-Down Heap Insertion
tags: [unit-3, c, code]
---

# 02 — Top-Down Heap Insertion

**Source file:** `Original Source Files/heap.c`  
**Purpose:** Inserts a key at the next free position and percolates it upward.

## Explanation

A node at index `i` has parent `i/2` in a 1-based binary heap.

## Code

```c
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
```

## Related files

- [[05 - Heap and Queue/00 - Complete Walkthrough]]
- [[05 - Heap and Queue/Original Source Files/README - Source Files]]
