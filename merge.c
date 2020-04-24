#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>


//performs the bottom up combine operation
void merge(int a[], int l, int m, int r)
{
	int temp[m-l+1], temp2[r-m];
	//copy first part to temporary array
	for(int i=0; i<(m-l+1); i++)
		temp[i]=a[l+i];
	//copy second part to temporary arrat]y
	for(int i=0; i<(r-m); i++)
		temp2[i]=a[m+1+i];
	int i=0, j=0, k=l;
	//combine the arrays on the basis of order
	while(i<(m-l+1) && j<(r-m))
	{
		if(temp[i]<temp2[j])
			a[k++]=temp[i++];
		else
			a[k++]=temp2[j++];
	}
	//to combine the remainder of the two arrays
	while(i<(m-l+1))
		a[k++]=temp[i++];
	while(j<(r-m))
		a[k++]=temp2[j++];

}

void mergeSortSerial(int aux[], int left, int right){
    if (left < right){
        int middle = (left + right)/2;
        mergeSortSerial(aux,left,middle); //call 1
        mergeSortSerial(aux,middle+1,right); //call 2
        merge(aux,left,middle,right);
    }
}



/*
   This function is the main hotspot of the algorithm which implements the logic of parallelism
   The calls labelled call 1 and call 2 are essentially independent since they are operating on distinct
   parts of the array and hence can be called in parallel.
   */

void mergeSort(int arrTosort[], int l, int r)
{
	if(l<r)
	{
		if((r-l) > 1000){
		int m=(l+r)/2;
		
			#pragma omp task firstprivate (arrTosort,l,m)
			{
				mergeSort(arrTosort,l,m); //call 1
			}
			#pragma omp task firstprivate (arrTosort,m,r)
			{
				mergeSort(arrTosort,m+1,r); //call 2
			}
			#pragma omp taskwait
			merge(arrTosort,l,m,r); //this function waits until all tasks are done || acts like reduction concept 
		}
		else{
			mergeSortSerial(arrTosort, l, r);
		}
	}
}

//a simple print function to print all the elements
void print(int a[], int n)
{
	for(int i=0; i<50; i++)
		printf("%d ", a[i] );
	printf("\n");
}

int main(int argc, char* argv[])
{

	if(argc == 2){
	int n = atoi(argv[1]);
	double startTime, endTime;	
	int arrTosort[n];
	for(int i=0; i<n; i++)
		arrTosort[i] = rand() % n;

	printf("\nUnsorted array: ");
	print(arrTosort,n);
	//call should be to a wrapper function that calls the mergeSort function
	startTime = omp_get_wtime();
	#pragma omp parallel
   	{
		//for(int i = 0;i<=9;i++){
			#pragma omp single
			mergeSort(arrTosort,0,n-1);
		//}
		
		//printf("Number of threads : %d\n",omp_get_num_threads());
	}
	endTime = omp_get_wtime();


	printf("\nSorted array: ");
	print(arrTosort,n);
	printf("Time to execute parallel mergesort: %f\n", endTime-startTime);
	}else{
		printf("One argument expected.\n");
	}
	return 0;
}
