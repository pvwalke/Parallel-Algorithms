#include <stdio.h> 
#include <stdlib.h>
#include <time.h>
#include <omp.h>



// interchange the elements
void swap(int i, int j, int *a)
{
	int temp;
	temp = a[i];
	a[i] = a[j];
	a[j] = temp;
}


// sorting direction in dir and the codition matches it interchange the value by calling swawp function
void compAndSwap(int a[], int i, int j, int dir)
{
	if(dir == (a[i] > a[j]))
		swap(i, j , a);
}

// sorts the bitonic sequence based on the sorting direction(dir)
void bitonicMerge(int a[], int low, int cnt, int dir)
{
	if(cnt>1)
	{
		int k = cnt/2;
		for(int i = low; i<low+k; i++)
		{
			compAndSwap(a,i,i+k,dir);
		}
		bitonicMerge(a, low, k, dir);
		bitonicMerge(a, low+k, k, dir);
	}
}

// create bitonic sequence by recursively sorting into two halves in opposite sorting orders (Linear version)
void bitonicSortLinear(int a[], int low, int cnt, int dir)
{
	
	if(cnt > 1)
	{
		int k = cnt/2;
		bitonicSortLinear(a, low, k, 1);
		// setting sorting order to descending and making recursive call to bitonicSortLinear
		bitonicSortLinear(a, low+k, k, 0);

		bitonicMerge(a,low,cnt,dir);
	}		
	
}

// create bitonic sequence by recursively sorting into two halves in opposite sorting orders (Parallel version)
void bitonicSortParallel(int a[], int low, int cnt, int dir)
{
	
	if(cnt > 1)
	{
		int k = cnt/2;
		#pragma omp parallel
		{
			#pragma omp sections
			{
				#pragma omp section
				{
					bitonicSortParallel(a, low, k, 1);
				}
				#pragma omp section
				{
					// setting sorting order to descending and making recursive call to bitonicSortParallel
					bitonicSortParallel(a, low+k, k, 0);
				}
			}
		}
		
		bitonicMerge(a,low,cnt,dir);
	}		
	
}


// call linear bitonicSort to sort array of N elements in ASCENDING order
void sortLiner(int a[], int N, int asc)
{
	bitonicSortLinear(a, 0, N, asc);
}

// call parallel bitonicSort to sort array of N elements in ASCENDING order
void sortParallel(int a[], int N, int asc)
{
	
	bitonicSortParallel(a, 0, N, asc);
}


// display functin to display time taken by parallel bitonicSort
void parallelTime(double start, double end, int a[], int size) 
{
	int sorted = 1;
	int i;

	printf("Time to execute parallel bitonic: %f\n", end-start);

	for (i = 0; i < size-1; ++i) 
	{
		sorted &= (a[i] <= a[i+1]);
	}

}

// print function to print sorted array
void print(int a[], int n)
{
	for(int i=0; i<n; i++)
	{
		printf("%d ", a[i]);
	}
	printf("\n");
}

// main function
int main()
{

	int *a;
	int *b;
	int num;
	int sp;
	

	printf("Provide length of sequence(should be power of 2): ");
	scanf("%d", &num);

	a = (int *) malloc (num * sizeof(int));
	b = (int *) malloc (num * sizeof(int));

	//generation random values for sequence
	for(int i = 0; i < num; i++)
	{
		a[i] = rand() % num;
	}

	
	// duplicating array for parallel sorting
	for(int j = 0; j < num; j++)
	{
		b[j] = a[j];
	}
	
	// setting default sorting order in ascending
	int asc = 1;
	clock_t t;
	
	// Linear bitonicSort 
	t = clock();
	sortLiner(a,num,asc);
	t = clock() - t;
	double t_taken = ((double) (t)) /  CLOCKS_PER_SEC;
	printf("Time taken by linear: %f \n",t_taken);
	//print(a, num);

	// Parallel bitonicSort
	double start, end;
    	start = omp_get_wtime();
    	sortParallel(b, num, asc);
    	end = omp_get_wtime();
    	parallelTime(start, end, b, num);
	//print(b, num);
	
}
