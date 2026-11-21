#include <stdio.h>
#include <stdlib.h>
#include "heap.h"

// COMMON FUNCTIONS

void init(struct heap *ptr_heap)
{
    ptr_heap->n = 0;
}

void deinit(struct heap *ptr_heap)
{
    ptr_heap->n = 0;
}

void disp(struct heap *ptr_heap)
{
    printf("\nDisplaying the heap\n");

    for(int i = 1; i <= ptr_heap->n; ++i)
    {
        printf("%d ", ptr_heap->key[i]);
    }

    printf("\n");
}

static void swap(int *p, int *q)
{
    int temp = *p;
    *p = *q;
    *q = temp;
}


// ========================================
// TOP-DOWN APPROACH
// ========================================

// Insert using percolation upwards
void insert(struct heap *ptr_heap, int key)
{
    if(ptr_heap->n >= MAXSIZE)
    {
        printf("heap full\n");
        exit(1);
    }

    int pos = ++ptr_heap->n; // Increment the size of the heap and get the position for the new key

    ptr_heap->key[pos] = key; // Place the new key at the end of the heap

    while(pos > 1 && ptr_heap->key[pos] > ptr_heap->key[pos / 2]) // While the new key is greater than its parent, swap it with its parent
    {
        swap(&ptr_heap->key[pos], &ptr_heap->key[pos / 2]); // Swap the new key with its parent

        pos = pos / 2; // Move up to the parent's position
    }
}

// Build heap by inserting elements one by one
void build_heap_top_down(struct heap *ptr_heap, int a[], int n)
{
    if(n < 0 || n > MAXSIZE)
    {
        printf("invalid heap size\n");
        exit(1);
    }

    ptr_heap->n = 0;

    for(int i = 1; i <= n; ++i)
    {
        insert(ptr_heap, a[i]);
    }
}


// ========================================
// BOTTOM-UP APPROACH
// ========================================

// Heapify downwards
static void heapify(int a[], int n, int pos)
{
    int i = pos; // Start from the given position
    int j = 2 * i; // Left child of the current node
    int val = a[i]; // Store the value of the current node
    int is_heap = 0; // Flag to indicate if the heap property is satisfied

    while(j <= n && !is_heap) // While the left child exists and the heap property is not satisfied // less than or equal to n because 1 based indexing
    {
        if(j + 1 <= n && a[j + 1] > a[j]) // If the right child exists and is greater than the left child
            ++j; // Move to the right child

        if(a[j] > val) // If the child is greater than the current node, swap them
        {
            a[i] = a[j]; // Move the child up
            i = j; // Move down to the child's position
            j = 2 * i; // Update the left child index
        }
        else
        {
            is_heap = 1; // The heap property is satisfied, exit the loop
        }
    }

    a[i] = val; // Place the original value in its correct position
}

// Build heap using bottom-up heapification
void build_heap(struct heap *ptr_heap, int a[], int n)
{
    if(n < 0 || n > MAXSIZE)
    {
        printf("invalid heap size\n");
        exit(1);
    }

    ptr_heap->n = n;

    for(int i = 1; i <= n; ++i)
    {
        ptr_heap->key[i] = a[i];
    }

    for(int i = n / 2; i >= 1; --i)
    {
        heapify(ptr_heap->key, ptr_heap->n, i);
        disp(ptr_heap);
    }
}


// ========================================
// COMMON HEAP OPERATIONS
// ========================================

int delete_max(struct heap *ptr_heap)
{
    if(ptr_heap->n == 0)
    {
        printf("heap empty\n");
        exit(1);
    }

    int res = ptr_heap->key[1]; // Store the maximum value (root of the heap)

    ptr_heap->key[1] = ptr_heap->key[ptr_heap->n]; // Move the last element to the root

    --ptr_heap->n; // Decrease the size of the heap

    if(ptr_heap->n > 0)
        heapify(ptr_heap->key, ptr_heap->n, 1); // Restore the heap property by heapifying down from the root

    return res;
}

int peek_max(struct heap *ptr_heap)
{
    if(ptr_heap->n == 0)
    {
        printf("heap empty\n");
        exit(1);
    }

    return ptr_heap->key[1];
}