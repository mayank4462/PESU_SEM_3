---
title: 02 — Filter Function
tags: [unit-3, c, code]
---

# 02 — Filter Function

**Source file:** `Original Source Files/function.c`  
**Purpose:** Copies only those input elements for which the callback returns a non-zero value.

## Explanation

The output count is returned through `ptr_m`. The caller must use that count when displaying or processing the filtered array.

## Code

```c
void myfilter(int a[], int n, int b[], int *ptr_m, int (*op)(int))
{
    *ptr_m = 0;

    for(int i = 0; i < n; ++i)
    {
        if(op(a[i]))
        {
            b[*ptr_m] = a[i];
            ++(*ptr_m);
        }
    }
}
```

## Related files

- [[01 - Callbacks and Flexible Heaps/00 - Complete Walkthrough]]
- [[01 - Callbacks and Flexible Heaps/Original Source Files/README - Source Files]]
