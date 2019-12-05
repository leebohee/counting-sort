#include <stdio.h>
#include <string.h>
#include <math.h>
#include <cuda.h>

__global__
void count(int* arr, int* hist, int size, int n, int max_val){
  int i;	
	int tid = blockDim.x*blockIdx.x + threadIdx.x;
	int limit = min(tid*size + size, n);

	for(i=tid*size; i<limit; i++){
		atomicAdd(&hist[arr[i]], 1);
	}
}
__host__
void counting_sort(int* arr, int n, int max_val){
	int *dev_hist, *dev_arr;
	int *hist;	
	int n_blk = 4 , n_tid = 32;
	int i, j, idx=0;
	double n_proc;

	cudaMalloc((void**)&dev_arr, n*sizeof(int));
	cudaMemcpy(dev_arr, arr, n*sizeof(int), cudaMemcpyHostToDevice);

	cudaMalloc((void**)&dev_hist, max_val*sizeof(int));
	hist = (int*)malloc(max_val*sizeof(int));
	memset(hist, 0, max_val*sizeof(int));
	cudaMemcpy(dev_hist, hist, max_val*sizeof(int), cudaMemcpyHostToDevice);

	n_proc = (double)n / (double)(n_blk*n_tid);
	count <<< n_blk, n_tid>>> (dev_arr, dev_hist, (int)ceil(n_proc), n, max_val);
	cudaDeviceSynchronize();
	cudaMemcpy(hist, dev_hist, max_val*sizeof(int), cudaMemcpyDeviceToHost);
	
	for(i=0; i<max_val; i++){
		for(j=0; j<hist[i]; j++){
			arr[idx++] = i;
		}
	}
	
	printf("%d ", hist[0]);
	cudaFree(dev_hist);
	cudaFree(dev_arr);
	free(hist);
}
