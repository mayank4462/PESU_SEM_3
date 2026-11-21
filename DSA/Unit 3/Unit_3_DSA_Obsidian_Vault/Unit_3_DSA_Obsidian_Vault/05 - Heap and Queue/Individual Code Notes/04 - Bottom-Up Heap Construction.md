---
title: 04 — Bottom-Up Heap Construction
tags: [unit-3, c, code]
---

# 04 — Bottom-Up Heap Construction

**Source file:** `Original Source Files/heap.c`  
**Purpose:** Builds a heap by copying the array and heapifying internal nodes from bottom to top.

## Explanation

The last internal node is at `n/2`. Bottom-up construction runs in O(n).

## Code

```c
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
```

## Related files

- [[05 - Heap and Queue/00 - Complete Walkthrough]]
- [[05 - Heap and Queue/Original Source Files/README - Source Files]]
