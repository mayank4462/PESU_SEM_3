#include "function.h"

void mymap(int a[], int b[], int n, int (*op)(int))
{
    for(int i = 0; i < n; ++i)
    {
        b[i] = op(a[i]);
    }
}

void myfilter(int a[], int n, int b[], int *ptr_m, int (*op)(int))
{
    *ptr_m = 0;

    for(int i = 0; i < n; ++i)
    {
        if(op(a[i]))
        {
            b[*ptr_m] = a[i];
            ++(*ptr_m);
        }
    }
}

int myreduce(int a[], int n, int init, int (*op)(int, int))
{
    for(int i = 0; i < n; ++i)
    {
        init = op(init, a[i]);
    }

    return init;
}
