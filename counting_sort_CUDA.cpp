#include <stdio.h>
#include <string.h>
#include <math.h>
#include <cuda.h>

__global__
void count(int* arr, int* hist, int size, int n){
  	int i;	
	int tid = blockDim.x*blockIdx.x + threadIdx.x;
	int limit = min(tid*size + size, n);

	for(i=tid*size; i<limit; i++){
		atomicAdd(&hist[arr[i]], 1);
	}
}

__global__
void sort(int* arr, int* hist, int size, int max_val){
  	int i=0, j, idx;
	int tid = blockDim.x*blockIdx.x + threadIdx.x;
	int limit = min(tid*size + size, max_val);

	if(tid == 0) idx = 0;
	else idx = hist[tid*size-1];
	for(i=tid*size; i<limit; i++){
		if(i==0) j=0;
		else j=hist[i-1];
		for(; j<hist[i]; j++){
			arr[idx++] = i;
		}
	}
}

__host__
void counting_sort(int* arr, int n, int max_val){
	int *dev_hist, *dev_arr;	
	int n_blk = 1000 , n_tid = 1000;
	int i, size;
	int* hist = (int*)malloc(max_val*sizeof(int));
	memset(hist, 0, max_val*sizeof(int));

	cudaMalloc((void**)&dev_arr, n*sizeof(int));
	cudaMalloc((void**)&dev_hist, max_val*sizeof(int));
	cudaMemcpy(dev_arr, arr, n*sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(dev_hist, hist, max_val*sizeof(int), cudaMemcpyHostToDevice);

	size = (int)ceil((double)n / (double)(n_blk*n_tid));
	count <<< n_blk, n_tid>>> (dev_arr, dev_hist, size, n);
	cudaDeviceSynchronize();
	cudaMemcpy(hist, dev_hist, max_val*sizeof(int), cudaMemcpyDeviceToHost);
	
	for(i=1; i<max_val; i++) hist[i] += hist[i-1];
	cudaMemcpy(dev_hist, hist, max_val*sizeof(int), cudaMemcpyHostToDevice);

	size = (int)ceil((double)max_val / (double)(n_blk*n_tid));
	sort <<< n_blk, n_tid>>> (dev_arr, dev_hist, size, max_val);
	cudaDeviceSynchronize();
	
	cudaMemcpy(arr, dev_arr, n*sizeof(int), cudaMemcpyDeviceToHost);
	
	cudaFree(dev_hist);
	cudaFree(dev_arr);
	free(hist);
}

