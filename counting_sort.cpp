#include <stdlib.h>
#include <stdio.h>

#define N 1000000
#define MAX_VAL 1000000

void counting_sort(int* arr, int size, int max_val){
  int i, j, idx = 0;
	int hist[max_val] = {0, };
	
	for(i=0; i<size; i++){
		hist[arr[i]]++;
	}
	for(i=0; i<max_val; i++){
		for(j=0; j<hist[i]; j++){
			arr[idx++] = i;
		}
	}
}

void print(int* arr, int size){
	int i;
	for(i=0; i<size; i++) printf("%d ", arr[i]);
	printf("\n");
}

int main()
{
  int array[N];

  for(int i=0;i<N;i++){
      array[i] = rand()%MAX_VAL;
  }
	/*
	printf("==== before ====\n");
	print(array, N);
  */
	counting_sort(array, N, MAX_VAL);
	/*
	printf("==== after ====\n");
	print(array, N);
  */
	for(int i=0;i<N-1;i++){
      if( array[i] > array[i+1]){
          printf("Not sorted\n");
          exit(1);
      }
  }
  printf("Sorted\n");
}
