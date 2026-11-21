---
title: Unit 3 C Programs - Index
aliases: [U3 Codes, Unit 3 Code Notes]
tags: [unit-3, c, dsa, code, index]
---

# Unit 3 C Programs — Obsidian Index

These notes cover every source, header, build file and README found in `U3Codes.zip`. The original program logic is retained. A small number of explanatory comments have been inserted into code listings where they clarify a non-obvious operation; no algorithm has been silently replaced.

## Topic notes

1. [[01 - Callbacks Flexible Heaps and Running Median]] — function pointers, map/filter/reduce, comparator-driven heaps and running median.
2. [[02 - Array Representation of BST]] — BST stored in an array, insertion, traversals, counting, copying and comparison.
3. [[03 - Dynamic BST]] — pointer-based BST, traversals, counting, copying, comparison, deletion and deallocation.
4. [[04 - Expression Tree from Postfix]] — stack of tree-node pointers, construction from postfix and recursive evaluation.
5. [[05 - Heap Construction and Queue Wrapper]] — max-heap insertion/deletion, top-down and bottom-up construction, and the queue wrapper.
6. [[06 - Threaded Binary Search Tree]] — predecessor/successor threads, insertion, stack-free inorder traversal and deallocation.
7. [[07 - Code Audit and Quick Comparison]] — representation comparison, function comparison and common exam pitfalls.

## Archive coverage checklist

| Folder | Files included |
|---|---|
| `U3_Experiential_Combined/U3_Experiential_Combined` | `function.c`, `function.h`, `heap.c`, `heap.h`, `main.c`, `Makefile`, `README.txt` |
| `BST_Array` | `client.c`, `tree_array.c`, `tree_array.h` |
| `DynamicBST` | `client.c`, `tree.c`, `tree.h` |
| `expression_tree` | `client.c`, `makefile`, `stack.c`, `stack.h` |
| `Heap` | `client.c`, `heap.c`, `heap.h`, `queue.c`, `queue.h` |
| `ThreadedBST` | `client.c`, `tree.c`, `tree.h` |

**Total: 25 files** (including build instructions and project README).

## Important source observations

- The `Heap/queue.c` wrapper uses `insert()` and `delete_max()`. It therefore removes the maximum value, not the earliest enqueued value. Its behaviour is a max-priority queue, not a conventional FIFO queue.
- In `expression_tree/stack.h`, `deinit_stack` is declared without a parameter, but `stack.c` defines it with a `stack_t *` parameter. The header uses an old-style declaration with an unspecified parameter list. It is better practice to declare the pointer parameter explicitly so it matches the definition.
- `expression_tree/client.c` uses `gets()`, which is unsafe and removed from modern C standards. Use `fgets()` in a corrected implementation.
- The expression-tree `deinit_stack()` only resets `top_`; it does not free the allocated expression-tree nodes. The current code therefore leaks memory.
- The flexible heap in the experiential project uses a comparator. Its `delete_max()` and `peek_max()` names are retained even when configured as a min-heap; in that configuration they operate on the minimum root.

## How to use

Open this folder as an Obsidian vault, or copy the Markdown notes into an existing vault. Start with this index and open topic notes using the wiki links above.
