#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <algorithm>
#include <chrono>

#ifdef __cplusplus
extern "C" {
#endif

  void generateMergeSortData (int* arr, size_t n);
  void checkMergeSortResult (int* arr, size_t n);
  
#ifdef __cplusplus
}
#endif


void merge(int * arr, int l, int mid, int r) {
  
  int i, j, k;
  int n = mid - l + 1;
  int m = r - mid;
  
  // declare and init temp arrays
  int *L = new int[n];
  int *R = new int[m];
  for (i=0; i<n; ++i)
    L[i] = arr[l+i];
  for (j=0; j<m; ++j)
    R[j] = arr[mid+1+j];

  // init indicies
  i = 0;
  j = 0;
  k = l;

  // fill arr with least value at current position in tempL or tempR
  while (i<n && j<m) {
    arr[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];
  }

  // ensure tempL was competely copied
  while (i<n) {
    arr[k++] = L[i++];
  }

  // ensure tempR was competely copied
  while (j<m) {
    arr[k++] = R[j++];
  }

  // de-allocate structs used
  delete[] L;
  delete[] R;

}


void mergesort(int * arr, int l, int r) {

  if (l < r) {
    int mid = (l+r)/2;
    mergesort(arr, l, mid);
    mergesort(arr, mid+1, r);
    merge(arr, l, mid, r);
  }

}




int main (int argc, char* argv[]) {

  if (argc < 2) { std::cerr<<"Usage: "<<argv[0]<<" <n>"<<std::endl;
    return -1;
  }

  
  // command line parameter
  int n;
  n = atoi(argv[1]);

  // get arr data
  int * arr = new int [n];
  generateMergeSortData (arr, n);


  // begin timing
  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
  
  // sort
  mergesort(arr, 0, n-1);

  // end timing
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  std::chrono::duration<double> elpased_seconds = end-start;

  // display time to cerr
  std::cerr<<elpased_seconds.count()<<std::endl;
  checkMergeSortResult (arr, n);
  
  delete[] arr;

  return 0;
}
