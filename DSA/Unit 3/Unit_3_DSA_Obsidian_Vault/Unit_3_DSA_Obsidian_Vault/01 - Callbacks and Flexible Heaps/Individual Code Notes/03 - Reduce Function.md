---
title: 03 — Reduce Function
tags: [unit-3, c, code]
---

# 03 — Reduce Function

**Source file:** `Original Source Files/function.c`  
**Purpose:** Combines array elements into one accumulated result.

## Explanation

The `init` value is the starting accumulator. Each callback call combines the current accumulator with the next array element.

## Code

```c
int myreduce(int a[], int n, int init, int (*op)(int, int))
{
    for(int i = 0; i < n; ++i)
    {
        init = op(init, a[i]);
    }

    return init;
}
```

## Related files

- [[01 - Callbacks and Flexible Heaps/00 - Complete Walkthrough]]
- [[01 - Callbacks and Flexible Heaps/Original Source Files/README - Source Files]]
