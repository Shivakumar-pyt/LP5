#include <bits/stdc++.h>
// #include <cuda.h>
// #include <cuda_runtime.h>
using namespace std;

#define BLOCK_SIZE 256


void fill_array(int *arr, int size) {
	for(int i=0;i<size;i++) {
		arr[i] = rand() % 1000;
	}
}

void print_array(int *arr, int size) {
	for(int i=0;i<size;i++) cout<<arr[i]<<" ";
	cout<<endl;
}


void cpu_add(int *arr1, int *arr2, int *result, int size) {
	for(int i=0;i<size;i++) {
		result[i] = arr1[i]+arr2[i];
	}
}

__global__ void gpu_add(int *arr1, int *arr2, int *arr3,int size){
	int block_id = blockIdx.x * blockDim.x + threadIdx.x;
	if(block_id < size) {
		arr3[block_id] = arr1[block_id] + arr2[block_id];
	}
}

int main() {
	int *arr1_cpu, *arr2_cpu, *result_cpu;
	int *arr1_gpu, *arr2_gpu, *result_gpu;

	int size;

	cout<<"Enter size of array: ";
	cin>>size;
	cout<<endl;

	arr1_cpu = new int[size];
	arr2_cpu = new int[size];
	result_cpu = new int[size];

	fill_array(arr1_cpu,size);
	fill_array(arr2_cpu,size);

	cout<<"Array 1: ";
	print_array(arr1_cpu,size);

	cout<<"Array 2: ";
	print_array(arr2_cpu,size);


	cudaMallocManaged(&arr1_gpu, size * (sizeof(int)));
	cudaMallocManaged(&arr2_gpu, size * (sizeof(int)));
	cudaMallocManaged(&result_gpu, size * (sizeof(int)));

	cudaMemcpy(arr1_gpu,arr1_cpu,size * sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(arr2_gpu,arr2_cpu,size * sizeof(int), cudaMemcpyHostToDevice);


	cudaEvent_t start, stop;
	float elapsed_time;


	int threadsPerBlock = 256;
	int blocksPerGrid = (size + threadsPerBlock - 1) / threadsPerBlock;


	cudaEventCreate(&start);
	cudaEventCreate(&stop);
	cudaEventRecord(start,0);

	gpu_add<<<blocksPerGrid, threadsPerBlock>>>(arr1_gpu,arr2_gpu,result_gpu,size);

	cudaEventRecord(stop,0);
	cudaEventSynchronize(stop);
	cudaEventElapsedTime(&elapsed_time,start,stop);
	cudaEventDestroy(start);
	cudaEventDestroy(stop);
	cudaMemcpy(result_cpu,result_gpu,size * sizeof(int),cudaMemcpyDeviceToHost);

	cout<<"Result array after addition on gpu: ";
	print_array(result_cpu,size);

	cout<<"Elapsed Time: "<<elapsed_time<<" milliseconds"<<endl;



	cudaEventCreate(&start);
	cudaEventCreate(&stop);
	cudaEventRecord(start,0);

	cpu_add(arr1_cpu,arr2_cpu,result_cpu,size);

	cudaEventRecord(stop,0);
	cudaEventSynchronize(stop);
	cudaEventElapsedTime(&elapsed_time,start,stop);
	cudaEventDestroy(start);
	cudaEventDestroy(stop);

	cout<<"Result array after addition on cpu: ";
	print_array(result_cpu,size);

	cout<<"Elapsed Time: "<<elapsed_time<<" milliseconds"<<endl;

	cudaFree(arr1_gpu);
	cudaFree(arr2_gpu);
	cudaFree(result_gpu);

}