#include <stdio.h>
#include "function.h"
#include "heap.h"

/* STEP 1: callback functions */
int sq(int x)
{
    return x * x;
}

int is_even(int x)
{
    return x % 2 == 0;
}

int add(int x, int y)
{
    return x + y;
}

void disp_array(int a[], int n)
{
    for(int i = 0; i < n; ++i)
    {
        printf("%d ", a[i]);
    }
    printf("\n");
}

void run_callbacks(void)
{
    int a[] = {1, 2, 3, 4, 5};
    int n = 5;
    int b[5];
    int m;

    printf("\nOriginal array : ");
    disp_array(a, n);

    mymap(a, b, n, sq);
    printf("After map (square) : ");
    disp_array(b, n);

    myfilter(a, n, b, &m, is_even);
    printf("After filter (even numbers) : ");
    disp_array(b, m);

    int sum = myreduce(a, n, 0, add);
    printf("After reduce (sum) : %d\n", sum);
}

/* STEP 2: comparison functions decide the heap type */
int max(int x, int y)
{
    return x > y;
}

int min(int x, int y)
{
    return x < y;
}

void run_flexible_heap(void)
{
    heap_t h;
    int heap_type;
    int opt;
    int key;

    printf("\n1: max heap  2: min heap : ");
    if(scanf("%d", &heap_type) != 1)
        return;

    if(heap_type == 1)
    {
        init(&h, max);
    }
    else if(heap_type == 2)
    {
        init(&h, min);
    }
    else
    {
        printf("invalid heap type\n");
        return;
    }

    printf("opt : 1 insert; 2 : delete; 0 : exit : ");
    if(scanf("%d", &opt) != 1)
    {
        deinit(&h);
        return;
    }

    while(opt != 0)
    {
        switch(opt)
        {
            case 1:
                printf("enter key : ");
                if(scanf("%d", &key) != 1)
                {
                    deinit(&h);
                    return;
                }
                insert(&h, key);
                disp(&h);
                break;

            case 2:
                if(h.n_ == 0)
                {
                    printf("heap empty\n");
                }
                else
                {
                    key = delete_max(&h);
                    printf("deleted root : %d\n", key);
                    disp(&h);
                }
                break;

            default:
                printf("invalid option\n");
                break;
        }

        printf("opt : 1 insert; 2 : delete; 0 : exit : ");
        if(scanf("%d", &opt) != 1)
            break;
    }

    deinit(&h);
}

/* STEP 3: Running median using two flexible heaps */
static void insert_number(heap_t *left_half,
                          heap_t *right_half, int x)
{
    // Left is a Max Heap; right is a Min Heap.
    if(left_half->n_ == 0 || x <= peek_max(left_half)) // if left is empty or x <= max of left
    {
        insert(left_half, x); // insert into left half
    }
    else
    {
        insert(right_half, x); // insert into right half
    }

    // Left half may have at most one extra element.
    if(left_half->n_ > right_half->n_ + 1) // if left has more than one extra element
    {
        int temp = delete_max(left_half);
        insert(right_half, temp);
    }

    // If right has more elements, move its minimum to left.
    if(right_half->n_ > left_half->n_)
    {
        int temp = delete_max(right_half);
        insert(left_half, temp);
    }
}

static double get_median(heap_t *left_half, heap_t *right_half)
{
    // This branch is used when the total number of elements is odd.
    if(left_half->n_ > right_half->n_)
    {
        return (double)peek_max(left_half);
    }

    // Equal sizes: average the largest lower-half value and
    // the smallest upper-half value.
    return ((double)peek_max(left_half) +
            (double)peek_max(right_half)) / 2.0;
}

void run_running_median(void)
{
    heap_t left_half;
    heap_t right_half;
    int n;
    int x;

    init(&left_half, max);
    init(&right_half, min);

    printf("\nnumber of elements (1-%d) : ", MAXSIZE);
    if(scanf("%d", &n) != 1 || n < 1 || n > MAXSIZE)
    {
        printf("invalid number of elements\n");
        deinit(&left_half);
        deinit(&right_half);
        return;
    }

    for(int i = 0; i < n; ++i)
    {
        printf("enter number : ");
        if(scanf("%d", &x) != 1)
        {
            printf("invalid input\n");
            deinit(&left_half);
            deinit(&right_half);
            return;
        }

        insert_number(&left_half, &right_half, x);

        printf("running median : %.1f\n",
               get_median(&left_half, &right_half));
    }

    deinit(&left_half);
    deinit(&right_half);
}

int main(void)
{
    int choice;

    do
    {
        printf("\n========== U3 EXPERIENTIAL LEARNING ==========\n");
        printf("1. Map / Filter / Reduce\n");
        printf("2. Flexible Heap (Max / Min)\n");
        printf("3. Running Median\n");
        printf("0. Exit\n");
        printf("Enter choice : ");

        if(scanf("%d", &choice) != 1)
            return 1;

        switch(choice)
        {
            case 1:
                run_callbacks();
                break;

            case 2:
                run_flexible_heap();
                break;

            case 3:
                run_running_median();
                break;

            case 0:
                printf("exiting...\n");
                break;

            default:
                printf("invalid choice\n");
                break;
        }

    } while(choice != 0);

    return 0;
}
