#ifndef HEAP_H
#define HEAP_H

#define MAXSIZE 200

struct heap
{
    int key_[MAXSIZE + 1];
    int n_;
    int (*compare)(int, int);
};
typedef struct heap heap_t;

void init(heap_t *ptr_heap, int (*compare)(int, int));
void deinit(heap_t *ptr_heap);
void disp(heap_t *ptr_heap);
void insert(heap_t *ptr_heap, int key);
int delete_max(heap_t *ptr_heap);
int peek_max(heap_t *ptr_heap);
void build_heap(heap_t *ptr_heap, int a[], int n);

#endif
