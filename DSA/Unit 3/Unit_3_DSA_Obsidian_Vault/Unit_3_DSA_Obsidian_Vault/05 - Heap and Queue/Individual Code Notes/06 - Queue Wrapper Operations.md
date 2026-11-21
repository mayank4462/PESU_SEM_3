---
title: 06 — Queue Wrapper Operations
tags: [unit-3, c, code]
---

# 06 — Queue Wrapper Operations

**Source file:** `Original Source Files/queue.c`  
**Purpose:** Wraps heap operations behind queue-style function names.

## Explanation

Important source behaviour: `dequeue()` calls `delete_max()`, so this is a max-priority queue, not a FIFO queue.

## Code

```c
void init_queue(struct queue *ptr_queue)
{
    init(&ptr_queue->q);
}

void deinit_queue(struct queue *ptr_queue)
{
    deinit(&ptr_queue->q);
}

void enqueue(struct queue *ptr_queue, int key)
{
    insert(&ptr_queue->q, key);
}

int dequeue(struct queue *ptr_queue)
{
    return delete_max(&ptr_queue->q);
}

int is_empty(struct queue *ptr_queue)
{
    return ptr_queue->q.n == 0;
}

int is_full(struct queue *ptr_queue)
{
    return ptr_queue->q.n == MAXSIZE;
}

void disp_queue(struct queue *ptr_queue)
{
    disp(&ptr_queue->q);
}
```

## Related files

- [[05 - Heap and Queue/00 - Complete Walkthrough]]
- [[05 - Heap and Queue/Original Source Files/README - Source Files]]
