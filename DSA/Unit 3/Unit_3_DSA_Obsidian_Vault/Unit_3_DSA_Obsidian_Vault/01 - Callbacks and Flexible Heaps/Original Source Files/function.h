#ifndef FUNCTION_H
#define FUNCTION_H

void mymap(int a[], int b[], int n, int (*op)(int));
void myfilter(int a[], int n, int b[], int *ptr_m, int (*op)(int));
int myreduce(int a[], int n, int init, int (*op)(int, int));

#endif
