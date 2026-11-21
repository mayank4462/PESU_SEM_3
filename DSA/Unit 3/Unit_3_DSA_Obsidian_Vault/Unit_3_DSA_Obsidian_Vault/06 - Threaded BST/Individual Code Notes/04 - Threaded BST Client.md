---
title: 04 — Threaded BST Client
tags: [unit-3, c, code]
---

# 04 — Threaded BST Client

**Source file:** `Original Source Files/client.c`  
**Purpose:** Demonstrates threaded BSTs with mixed insertion order, sorted insertion order and an empty tree.

## Explanation

Sorted insertion creates a skewed BST; the threaded traversal still proceeds without recursion.

## Code

```c
int main()
{
    // case 1
    struct tree t1;
    init(&t1);

    int a1[] = {40, 20, 60, 10, 70, 30, 50};
    int n1 = 7;

    for(int i = 0; i < n1; ++i)
    {
        insert(&t1, a1[i]);
    }

    disp(&t1);
    deinit(&t1);

    // case 2
    struct tree t2;
    init(&t2);

    int a2[] = {10, 20, 30, 40, 50, 60, 70};
    int n2 = 7;

    for(int i = 0; i < n2; ++i)
    {
        insert(&t2, a2[i]);
    }

    disp(&t2);
    deinit(&t2);

    // case 3
    struct tree t3;
    init(&t3);

    disp(&t3);
    deinit(&t3);

    return 0;
}
```

## Related files

- [[06 - Threaded BST/00 - Complete Walkthrough]]
- [[06 - Threaded BST/Original Source Files/README - Source Files]]
