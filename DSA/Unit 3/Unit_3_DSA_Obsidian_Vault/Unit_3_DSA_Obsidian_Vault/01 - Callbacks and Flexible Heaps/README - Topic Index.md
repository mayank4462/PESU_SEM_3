---
title: 01 - Callbacks and Flexible Heaps
aliases: [01 - Callbacks and Flexible Heaps]
tags: [unit-3, c, dsa]
---

# 01 - Callbacks and Flexible Heaps

This folder is organised so each important implementation can be opened independently.

## Individual code notes

- [[Individual Code Notes/01 - Map Function|01 — Map Function]] — Applies a one-argument callback to every input element and stores each result in the output array.
- [[Individual Code Notes/02 - Filter Function|02 — Filter Function]] — Copies only those input elements for which the callback returns a non-zero value.
- [[Individual Code Notes/03 - Reduce Function|03 — Reduce Function]] — Combines array elements into one accumulated result.
- [[Individual Code Notes/04 - Callback Examples|04 — Callback Examples]] — Defines the callbacks passed to map, filter and reduce.
- [[Individual Code Notes/05 - Comparator Callbacks|05 — Comparator Callbacks]] — Defines priority comparators used to configure the same heap implementation as a max-heap or min-heap.
- [[Individual Code Notes/06 - Flexible Heap Insertion|06 — Flexible Heap Insertion]] — Inserts a value and restores heap order by moving it upwards.
- [[Individual Code Notes/07 - Flexible Heap Deletion|07 — Flexible Heap Deletion]] — Removes the root, moves the final element to the root, then restores heap order downward.
- [[Individual Code Notes/08 - Flexible Heap Peek and Build|08 — Flexible Heap Peek and Build]] — Reads the root without removal and constructs a heap bottom-up.
- [[Individual Code Notes/09 - Running Median|09 — Running Median]] — Maintains a running median with two heaps: a max-heap for the lower half and a min-heap for the upper half.

## Complete walkthrough

- [[00 - Complete Walkthrough]] — detailed explanation and full source listings for this topic.

## Original source files

Open `Original Source Files/` for the exact `.c`, `.h`, Makefile and README files from the uploaded archive. These are preserved verbatim and can be used to compile the original program.
