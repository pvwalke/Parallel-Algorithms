
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

//returns maximum value
int getMax(int a[], int n)
{
	int max = a[0];
	for (int i = 1; i < n; i++)
		if (a[i] > max)
			max = a[i];
	return max;
}

//prints array
void printArray(int a[], int size)
{
	int i;
	for (i = 0; i < 100; i++)
		printf("%d ", a[i]);
}

void countSort_p(int a[], int n, int base)
{
	int output[n];
	int i, count[10] = {0};

//Beginning of parallel region
#pragma omp parallel for reduction(+:count)
		for (i = 0; i < n; i++)
			count[(a[i] / base) % 10] = count[(a[i] / base) % 10] + 1;

		for (i = 1; i < 10; i++)
			count[i] += count[i - 1];

		for (i = n - 1; i >= 0; i--)
		{
			output[count[(a[i] / base) % 10] - 1] = a[i];
			count[(a[i] / base) % 10]--;
		}
#pragma omp parallel for	
		for (i = 0; i < n; i++)
			a[i] = output[i];
	
}

//sequential count sort
void countSort(int a[], int n, int base)
{
	int output[n];
	int i, count[10] = {0};

	for (i = 0; i < n; i++)
		count[(a[i] / base) % 10]++;

	for (i = 1; i < 10; i++)
		count[i] += count[i - 1];

	for (i = n - 1; i >= 0; i--)
	{
		output[count[(a[i] / base) % 10] - 1] = a[i];
		count[(a[i] / base) % 10]--;
	}

	for (i = 0; i < n; i++)
		a[i] = output[i];
}

void radixsort(int a[], int n)
{

	int m = getMax(a, n);

	for (int base = 1; m / base > 0; base *= 10)
		countSort(a, n, base);
}

void radixsort_p(int *a, int n)
{
	int m = getMax(a, n);

	for (int base = 1; m / base > 0; base *= 10)
		countSort_p(a, n, base);
}

void print(int a[], int n)
{
	for (int i = 0; i < 10; i++)
		printf("%d ", a[i]);
}

int main()
{
	int n = 60000;
	int i, a[n], b[n];

	double time_spent = 0.0;
	for (i = 0; i < n; i++)
		a[i] = rand() % 10000;

	for (i = 0; i < n; i++)
		b[i] = a[i];

	clock_t begin = clock();
	radixsort(a, n);
	clock_t end = clock();

	time_spent = 0.0;
	time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
	printf("\n Time spent seq = %lf \n", time_spent);
	printArray(a, n);

	begin = clock();
	radixsort_p(b, n);
	end = clock();

	time_spent = 0.0;
	time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
	printf("\n Time spent parallel = %lf \n", time_spent);
	printArray(b, n);


	return 0;
}
