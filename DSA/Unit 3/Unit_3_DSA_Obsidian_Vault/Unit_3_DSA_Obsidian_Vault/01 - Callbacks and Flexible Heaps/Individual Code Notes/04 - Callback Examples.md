---
title: 04 — Callback Examples
tags: [unit-3, c, code]
---

# 04 — Callback Examples

**Source file:** `Original Source Files/main.c`  
**Purpose:** Defines the callbacks passed to map, filter and reduce.

## Explanation

`sq` transforms; `is_even` acts as a predicate; `add` combines two values.

## Code

```c
int sq(int x)
{
    return x * x;
}

int is_even(int x)
{
    return x % 2 == 0;
}

int add(int x, int y)
{
    return x + y;
}
```

## Related files

- [[01 - Callbacks and Flexible Heaps/00 - Complete Walkthrough]]
- [[01 - Callbacks and Flexible Heaps/Original Source Files/README - Source Files]]
