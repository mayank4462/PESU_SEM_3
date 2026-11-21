#ifndef HEAP_H
#define HEAP_H

#define MAXSIZE 200

struct heap
{
    int key[MAXSIZE + 1];
    int n;
};

void init(struct heap *ptr_heap);
void deinit(struct heap *ptr_heap);
void disp(struct heap *ptr_heap);

void insert(struct heap *ptr_heap, int key);
int delete_max(struct heap *ptr_heap);
int peek_max(struct heap *ptr_heap);

void build_heap(struct heap *ptr_heap, int a[], int n);
void build_heap_top_down(struct heap *ptr_heap, int a[], int n);

#endif