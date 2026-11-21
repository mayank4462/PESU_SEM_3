U3 EXPERIENTIAL LEARNING - COMBINED PROJECT
===========================================

This project combines the three professor-provided steps into one C program.

FILES
-----
main.c      Main menu, callback examples, max/min callback functions,
            and running-median algorithm.
function.h  Callback function declarations.
function.c  Map, filter and reduce implementations.
heap.h      Shared flexible heap structure and declarations.
heap.c      Comparator-driven heap operations.
Makefile    Builds the complete project as client.exe.

BUILD
-----
make

RUN
---
./client.exe

MENU
----
1. Map / Filter / Reduce
2. Flexible Heap (Max / Min)
3. Running Median
0. Exit

NOTES
-----
1. The heap uses 1-based indexing, so key_ has MAXSIZE + 1 entries.
2. build_heap() accepts a normal 0-based input array a[0] through a[n-1].
3. delete_max() and peek_max() retain the names from the provided course code.
   With a Min Heap, they operate on its root, which is the minimum element.
4. The Step 1 starter files had a naming mismatch: function.h declared
   filter/reduce, while the TODO signatures in function.c used myfilter/
   myreduce. This combined project consistently uses myfilter/myreduce.
5. Running median follows the supplied slides: left_half is a Max Heap,
   right_half is a Min Heap; left has equal size or one extra element.
