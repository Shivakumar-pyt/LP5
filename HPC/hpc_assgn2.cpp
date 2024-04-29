#include <bits/stdc++.h>
#include <omp.h>

using namespace std;


vector<int> bubbleSort(int n,vector<int> arr) {
	for(int i=0;i<n-1;i++) {

		#pragma omp parallel for
		for(int j=0;j<n-i-1;j++) {
			if(arr[j] > arr[j+1]) {
				swap(arr[j],arr[j+1]);
			}
		}
	}

	return arr;
}


void merge(vector<int> &arr, int l, int m, int r) {
	int n1 = m-l+1, n2 = r-m;

	vector<int> left(n1), right(n2);

	for(int i=0;i<n1;i++) {
		left[i] = arr[l+i];
	}

	for(int i=0;i<n2;i++) {
		right[i] = arr[m+i+1];
	}

	int i = 0, j = 0, k = l;

	while(i < n1 && j < n2) {
		if(left[i] < right[j]) {
			arr[k] = left[i];
			i++;
		}

		else {
			arr[k] = right[j];
			j++;
		}

		k++;
	}

	while(i < n1) {
		arr[k] = left[i];
		i++;
		k++;
	}

	while (j < n2)
    {
        arr[k] = right[j];
        j++;
        k++;
    }
}



void mergeSort(int l, int r, vector<int> &arr) {
	if(l < r) {
		int m = (l+r)/2;

		#pragma omp parallel sections
		{
			#pragma omp section
			mergeSort(l,m,arr);

			#pragma omp section
			mergeSort(m+1,r,arr);
		}

		merge(arr, l, m, r);
	}
}

int main() {
	int size;
	cout<<"Enter size of array: ";
	cin>>size;

	vector<int> arr(size);

	for(int i=0;i<size;i++) {
		arr[i] = rand() % 1000;
	}

	double start = omp_get_wtime();

	mergeSort(0,size-1,arr);

	double end = omp_get_wtime();

	cout<<"Time taken: "<<end-start;
	
}

