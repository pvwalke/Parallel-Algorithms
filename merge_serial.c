//merge sort - serial implemenation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void merge(int a[], int l, int m, int r)
{
	int temp[m-l+1], temp2[r-m];
	for(int i=0; i<(m-l+1); i++)
		temp[i]=a[l+i];
	for(int i=0; i<(r-m); i++)
		temp2[i]=a[m+1+i];
	int i=0, j=0, k=l;
	while(i<(m-l+1) && j<(r-m))
	{
		if(temp[i]<temp2[j])
			a[k++]=temp[i++];
		else
			a[k++]=temp2[j++];
	}

	while(i<(m-l+1))
		a[k++]=temp[i++];
	while(j<(r-m))
		a[k++]=temp2[j++];

}
void mergeSort(int a[], int l, int r)
{
	if(l<r)
	{
		int m=(l+r)/2;
		mergeSort(a,l,m);
		mergeSort(a,m+1,r);
		merge(a,l,m,r);
	}
}
void print(int a[], int n)
{
	for(int i=0; i<50; i++)
		printf("%d ", a[i] );
	printf("\n");
}

int main(int argc , char* argv[])
{
	if(argc == 2){
	int n = atoi(argv[1]); 
	clock_t t; 
	int a[n];
	for(int i=0; i<n; i++)
		a[i] = rand() % n;
	print(a,n);
	t = clock();
       //for(int i = 0; i<=9;i++){	
	mergeSort(a,0,n-1);
       //}
	t = clock() - t;
	double time_taken = ((double) (t)) / CLOCKS_PER_SEC;
	printf(" Time to execute serial mergesort: %f \n", time_taken);
	print(a,n);
	}
	else{
		printf("One argument needs to be passed");
	}
	return 0;
}
