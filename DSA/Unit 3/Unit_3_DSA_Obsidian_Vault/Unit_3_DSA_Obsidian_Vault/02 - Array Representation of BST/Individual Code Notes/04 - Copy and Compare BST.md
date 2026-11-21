---
title: 04 — Copy and Compare BST
tags: [unit-3, c, code]
---

# 04 — Copy and Compare BST

**Source file:** `Original Source Files/tree_array.c`  
**Purpose:** Copies the source tree representation and checks whether two trees have identical shape and keys.

## Explanation

Equality requires both corresponding keys and child structure to match.

## Code

```c
void copy(struct tree *ptr_src, struct tree *ptr_dst)
{
    ptr_dst->root = ptr_src->root;
    ptr_dst->count = ptr_src->count;

    for(int i = 0; i < ptr_src->count; ++i)
    {
        ptr_dst->elem[i] = ptr_src->elem[i];
    }
}

static int are_same_recursive(struct node elem1[], int root1,
                              struct node elem2[], int root2)
{
    if(root1 == -1 && root2 == -1)
    {
        return 1;
    }
    else if(root1 == -1 || root2 == -1)
    {
        return 0;
    }
    else
    {
        return elem1[root1].key == elem2[root2].key &&
               are_same_recursive(elem1, elem1[root1].left,
                                  elem2, elem2[root2].left) &&
               are_same_recursive(elem1, elem1[root1].right,
                                  elem2, elem2[root2].right);
    }
}

int are_same(struct tree *ptr_tree1, struct tree *ptr_tree2)
{
    return are_same_recursive(ptr_tree1->elem, ptr_tree1->root,
                              ptr_tree2->elem, ptr_tree2->root);
}
```

## Related files

- [[02 - Array Representation of BST/00 - Complete Walkthrough]]
- [[02 - Array Representation of BST/Original Source Files/README - Source Files]]
