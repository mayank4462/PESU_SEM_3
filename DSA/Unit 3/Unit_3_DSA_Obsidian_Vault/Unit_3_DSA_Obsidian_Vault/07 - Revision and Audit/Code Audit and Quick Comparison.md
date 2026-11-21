---
title: Unit 3 Code Audit and Quick Comparison
tags: [unit-3, c, revision, audit]
---

# Unit 3 Code Audit and Quick Comparison

## Representation comparison

| Topic | Storage | Missing child marker | Main benefit / distinction |
|---|---|---|---|
| Array BST | Fixed array of nodes | Index `-1` | No per-node allocation; links are indices |
| Dynamic BST | Dynamically allocated nodes | `NULL` pointer | Flexible growth; explicit memory management |
| Threaded BST | Dynamically allocated nodes plus threads | Flags distinguish thread from child | Inorder traversal without recursion/stack |
| Heap | Array, 1-based | Not applicable | Complete binary tree; root has priority value |
| Expression tree | Dynamic nodes managed through pointer stack | `NULL` child pointers | Operators are internal nodes; operands are leaves |

## Critical function comparison

| Function | Purpose |
|---|---|
| `insert_recursive` (array/dynamic BST) | Find the BST insertion position recursively |
| `number_of_nodes_recursive` | Count current node plus both subtrees |
| `number_of_leaves_recursive` | Count nodes with no children |
| `are_same_recursive` | Compare corresponding keys and structure |
| `copy` | Copy array contents (array BST) or allocate a deep copy (dynamic BST) |
| `delete_key_recursive` | Delete a node while reconnecting its subtree |
| `heapify` | Restore heap order downward |
| `build_heap` | Construct a heap bottom-up |
| `push_operator` | Combine two expression subtrees under an operator |
| `eval_recursive` | Evaluate an expression tree recursively |
| `leftmost` | Find first inorder node in a threaded BST or successor subtree |
| `inorder` | Traverse threaded BST using successor threads |

## Common pitfalls

- Do not confuse array indices with pointers in the array BST.
- Inorder traversal of a BST is sorted, but preorder and postorder are not.
- `are_same()` tests both structure and values; same keys in a different shape are not the same tree.
- In dynamic BST deletion with two children, after copying the inorder successor's key, delete the successor node from the right subtree.
- In a max-heap, deleting the root means deleting the maximum, not the last inserted value.
- In the supplied `Heap/queue.c`, queue dequeue is maximum-priority removal, not FIFO removal.
- In postfix expression-tree construction, pop the right subtree before the left subtree.
- In threaded BST, check `lthread`/`rthread` before interpreting a pointer as a child.

## Final archive audit

All 25 archive files are present in the topic notes. The index lists every file by folder. The code blocks are generated from the contents of the uploaded ZIP, with only explanatory comments added at selected logic points. Build files and the README are included, not omitted.
