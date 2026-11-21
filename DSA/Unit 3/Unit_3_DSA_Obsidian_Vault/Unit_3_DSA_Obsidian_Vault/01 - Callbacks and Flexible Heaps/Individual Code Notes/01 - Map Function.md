---
title: 01 — Map Function
tags: [unit-3, c, code]
---

# 01 — Map Function

**Source file:** `Original Source Files/function.c`  
**Purpose:** Applies a one-argument callback to every input element and stores each result in the output array.

## Explanation

The loop preserves the input order. The callback performs the transformation; `mymap()` controls iteration.

## Code

```c
void mymap(int a[], int b[], int n, int (*op)(int))
{
    for(int i = 0; i < n; ++i)
    {
        b[i] = op(a[i]);
    }
}
```

## Related files

- [[01 - Callbacks and Flexible Heaps/00 - Complete Walkthrough]]
- [[01 - Callbacks and Flexible Heaps/Original Source Files/README - Source Files]]
