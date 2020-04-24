#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int *a, int l, int r)
{
    int temp;
    int pivot = a[r];
    int i = (l - 1);

    for (int j = l; j <= r - 1; j++)
    {
        if (a[j] < pivot)
        {
            i++;
            swap(&a[i], &a[j]);
        }
    }
    swap(&a[i + 1], &a[r]);
    return (i + 1);
}

//parallel
void quicksort_p(int *a, int l, int r)
{
    if (l < r)
    {
        int p = partition(a, l, r);

#pragma omptask default(none) firstprivate(a, l, p)
        {
            quicksort_p(a, l, p - 1);
        }

#pragma omptask default(none) firstprivate(a, r, p)
        {
            quicksort_p(a, p + 1, r);
        }
    }
}

//seqential
void quickSort(int a[], int l, int r)
{
    if (l < r)
    {
        int pi = partition(a, l, r);

        quickSort(a, l, pi - 1);
        quickSort(a, pi + 1, r);
    }
}

void printArray(int a[], int n)
{
    int i;
    for (i = 0; i < 50; i++)
        printf("%d ", a[i]);
}

void main()
{
    int n = 1000000;
    int i, a[n], b[n];

    double time_spent = 0.0;
    for (i = 0; i < n; i++)
        a[i] = rand() % n;

    for (i = 0; i < n; i++)
        b[i] = a[i];

    clock_t begin = clock();
    //Beginning of parallel region
#pragma omp parallel default(none) shared(a, n)
    {
#pragma omp single nowait
        {
            quickSort(a, 0, n - 1);
        }
    }
    clock_t end = clock();
    // End of parallel region

    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
    printf("\n Time spent seq = %lf \n", time_spent);
    printArray(a, n);

    //sequential
    begin = clock();
    quicksort_p(b, 0, n - 1);
    end = clock();

    time_spent = 0.0;
    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
    printf("\n Time spent par = %lf \n", time_spent);
    printArray(b, n);
}