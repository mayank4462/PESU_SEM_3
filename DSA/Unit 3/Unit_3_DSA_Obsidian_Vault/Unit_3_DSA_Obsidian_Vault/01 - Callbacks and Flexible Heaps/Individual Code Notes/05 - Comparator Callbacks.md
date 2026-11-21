---
title: 05 — Comparator Callbacks
tags: [unit-3, c, code]
---

# 05 — Comparator Callbacks

**Source file:** `Original Source Files/main.c`  
**Purpose:** Defines priority comparators used to configure the same heap implementation as a max-heap or min-heap.

## Explanation

The comparator returns non-zero when its first argument has higher priority than its second.

## Code

```c
int max(int x, int y)
{
    return x > y;
}

int min(int x, int y)
{
    return x < y;
}
```

## Related files

- [[01 - Callbacks and Flexible Heaps/00 - Complete Walkthrough]]
- [[01 - Callbacks and Flexible Heaps/Original Source Files/README - Source Files]]
