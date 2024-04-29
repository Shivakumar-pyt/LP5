#include <bits/stdc++.h>
#include <chrono>
#include <omp.h>
using namespace std;


int find_min(vector<int> arr) {
	int mini = arr[0];

	#pragma omp parallel for reduction(min: mini)
	for(int i=1;i<arr.size();i++) {
		mini = min(mini,arr[i]);
	}

	return mini;
}

int find_max(vector<int> arr) {
	int maxi = arr[0];

	#pragma omp parallel for reduction(max: maxi)
	for(int i=1;i<arr.size();i++) {
		maxi = max(maxi,arr[i]);
	}

	return maxi;
}

int find_sum(vector<int> arr) {
	int sum=0;

	#pragma omp parallel for reduction(+ : sum)
	for(int i=0;i<arr.size();i++) {
		sum += arr[i];
	}

	return sum;
}

int find_avg(vector<int> arr) {
	return find_sum(arr) / arr.size();
}

int main() {
	int size;
	cout<<"Enter size of array: ";
	cin>>size;

	vector<int> vec(size);

	for(int i=0;i<size;i++) {
		vec[i] = rand() % 1000;
		cout<<vec[i]<<" ";
	}

	int mini = find_min(vec), maxi = find_max(vec);
	cout<<"\nMinimum: "<<mini<<", Maximum: "<<maxi;
}