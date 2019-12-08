CUDA = /usr/local/cuda/

all:
	nvcc counting_sort.cu main.c
