#include <stdio.h>
#include <stdlib.h>
#include "heap.h"

void init(heap_t *ptr_heap, int (*compare)(int, int))
{
    if(compare == NULL)
    {
        printf("comparison function missing\n");
        exit(1);
    }

    ptr_heap->n_ = 0;
    ptr_heap->compare = compare;
}

void deinit(heap_t *ptr_heap)
{
    ptr_heap->n_ = 0;
    ptr_heap->compare = NULL;
}

void disp(heap_t *ptr_heap)
{
    printf("\n displaying the heap \n");

    for(int i = 1; i <= ptr_heap->n_; ++i)
    {
        printf("%d ", ptr_heap->key_[i]);
    }

    printf("\n");
}

static void heapify(int a[], int n, int pos,
                    int (*compare)(int, int))
{
    int i = pos;
    int j = 2 * i;
    int val = a[i];
    int is_heap = 0;

    while(j <= n && !is_heap)
    {
        // Select the child with higher priority.
        if(j + 1 <= n && compare(a[j + 1], a[j]))
            ++j;

        // Move that child up if it has higher priority than val.
        if(compare(a[j], val))
        {
            a[i] = a[j];
            i = j;
            j = 2 * i;
        }
        else
        {
            is_heap = 1;
        }
    }

    // Put the saved value into the final hole.
    a[i] = val;
}

static void swap(int *p, int *q)
{
    int temp = *p;
    *p = *q;
    *q = temp;
}

void insert(heap_t *ptr_heap, int key)
{
    if(ptr_heap->n_ == MAXSIZE)
    {
        printf("heap full\n");
        return;
    }

    // Add at the end, then percolate upwards.
    int pos = ++ptr_heap->n_;
    ptr_heap->key_[pos] = key;

    while(pos > 1 &&
          ptr_heap->compare(ptr_heap->key_[pos],
                            ptr_heap->key_[pos / 2]))
    {
        swap(&ptr_heap->key_[pos], &ptr_heap->key_[pos / 2]);
        pos = pos / 2;
    }
}

int delete_max(heap_t *ptr_heap)
{
    if(ptr_heap->n_ == 0)
    {
        printf("heap empty\n");
        exit(1);
    }

    int res = ptr_heap->key_[1];
    ptr_heap->key_[1] = ptr_heap->key_[ptr_heap->n_];
    --ptr_heap->n_;

    // Do not heapify an empty heap.
    if(ptr_heap->n_ > 0)
        heapify(ptr_heap->key_, ptr_heap->n_, 1, ptr_heap->compare);

    return res;
}

int peek_max(heap_t *ptr_heap)
{
    if(ptr_heap->n_ == 0)
    {
        printf("heap empty\n");
        exit(1);
    }

    return ptr_heap->key_[1];
}

void build_heap(heap_t *ptr_heap, int a[], int n)
{
    if(n < 0 || n > MAXSIZE)
    {
        printf("invalid heap size\n");
        return;
    }

    ptr_heap->n_ = n;

    // Input array a is 0-based; heap storage is 1-based.
    for(int i = 1; i <= n; ++i)
    {
        ptr_heap->key_[i] = a[i - 1];
    }

    // Bottom-up heap construction.
    for(int i = n / 2; i >= 1; --i)
    {
        heapify(ptr_heap->key_, ptr_heap->n_, i, ptr_heap->compare);
    }
}
