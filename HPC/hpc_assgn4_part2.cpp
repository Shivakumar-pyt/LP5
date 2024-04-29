#include <bits/stdc++.h>
// #include <cuda.h>
// #include <cuda_runtime.h>
using namespace std;


void fill_array(int *arr,int size) {
	for(int i=0;i<size;i++) {
		arr[i] = rand() % 1000;
	}
}

void print_array(int *arr,int size) {
	for(int i=0;i<size;i++) cout<<arr[i]<<" ";

	cout<<endl;
}


__global__ void gpu_multiply(int *arr1, int *arr2, int *arr3, int s) {
	int block_id_x = blockIdx.y*blockDim.y + threadIdx.y;
	int block_id_y = blockIdx.x*blockDim.x + threadIdx.x;

	if(block_id_x < s && block_id_y < s) {
		int sum=0;
		for(int i=0;i<s;i++) {
			sum += arr1[i + (block_id_x*s)] + arr2[(i*s) + block_id_y];
		}
		arr3[(block_id_x*s) + block_id_y] = sum;
	}
}

void cpu_multiply(int *arr1, int *arr2, int *arr3, int common,int s,int arr3_rows,int arr3_cols) {
	for(int i=0;i<s;i++) {
		for(int j=0;j<s;j++) {
			int sum=0;
			for(int k=0;k<common;k++) {
				sum +=  arr1[i*common + k] * arr2[k*arr3_cols + j];
			}

			arr3[i*arr3_cols + j] = sum;
		}
	}
}



int main() {
	int *arr1_cpu, *arr2_cpu, *result_cpu;
	int *arr1_gpu, *arr2_gpu, *result_gpu;

	int s;
	cout<<"Enter size of array: ";
	cin>>s;
	cout<<endl;

	int size=s*s;

	arr1_cpu = new int[size];
	arr2_cpu = new int[size];
  result_cpu = new int[size];

  fill_array(arr1_cpu,size);
  fill_array(arr2_cpu,size);

	cudaMallocManaged(&arr1_gpu,size*(sizeof(int)));
	cudaMallocManaged(&arr2_gpu,size*(sizeof(int)));
	cudaMallocManaged(&result_gpu,size*(sizeof(int)));

	cudaMemcpy(arr1_gpu,arr1_cpu,size * (sizeof(int)), cudaMemcpyHostToDevice);
	cudaMemcpy(arr2_gpu,arr2_cpu,size * (sizeof(int)), cudaMemcpyHostToDevice);

	cudaEvent_t start,stop;
	float elapsed_time;

	int THREADS = 2;
	int BLOCKS = s / THREADS;

	dim3 threads(THREADS,THREADS);
	dim3 blocks(BLOCKS,BLOCKS);

	cudaEventCreate(&start);
	cudaEventCreate(&stop);

	cudaEventRecord(start,0);

	gpu_multiply<<<blocks,threads>>>(arr1_gpu,arr2_gpu,result_gpu,s);

	cudaEventRecord(stop,0);
	cudaEventSynchronize(stop);
	cudaEventElapsedTime(&elapsed_time, start, stop);
	cudaEventDestroy(start);
	cudaEventDestroy(stop);
	cudaMemcpy(result_cpu,result_gpu,size*sizeof(int),cudaMemcpyDeviceToHost);

	cout<<"Result gpu multiplication: ";
	print_array(result_cpu,size);
	cout<<"Elapsed Time (GPU): "<<elapsed_time<<endl;



  cudaEventCreate(&start);
	cudaEventCreate(&stop);
	cudaEventRecord(start,0);

	cpu_multiply(arr1_cpu,arr2_cpu,result_cpu,s,s,s,s);

	cudaEventRecord(stop,0);
	cudaEventSynchronize(stop);
	cudaEventElapsedTime(&elapsed_time, start, stop);
	cudaEventDestroy(start);
	cudaEventDestroy(stop);

	cout<<"Result cpu multiplication: ";
	print_array(result_cpu,size);
	cout<<"Elapsed Time (CPU): "<<elapsed_time<<endl;
}