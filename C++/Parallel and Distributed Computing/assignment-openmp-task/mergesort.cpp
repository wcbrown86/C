#include <omp.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <chrono>

#ifdef __cplusplus
extern "C" {
#endif
  void generateMergeSortData (int* arr, size_t n);
  void checkMergeSortResult (int* arr, size_t n);
#ifdef __cplusplus
}
#endif


void merge(int* arr, int start, int end);
void mergeSort(int* arr, int start, int end);


int main (int argc, char* argv[]) {

  

  //forces openmp to create the threads beforehand
#pragma omp parallel
  {
    int fd = open (argv[0], O_RDONLY);
    if (fd != -1) {
      close (fd);
    }
    else {
      std::cerr<<"something is amiss"<<std::endl;
    }
  }
  
  if (argc < 3) { std::cerr<<"Usage: "<<argv[0]<<" <n> <nbthreads>"<<std::endl;
    return -1;
  }

  int n = atoi(argv[1]);
  
  // get arr data
  int * arr = new int [n];
  generateMergeSortData (arr, n);
  omp_set_num_threads(16);

  //insert sorting code here.
  //records the starting time of the program. 
  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

  //starts the mergesort process by calling the function mergesort.
  mergeSort(arr, 0, n - 1); 

  //Records the final time to print cerr
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end-start;
  std::cerr<<elapsed_seconds.count()<<std::endl; 
  
  //checks the array for correctness 
  checkMergeSortResult (arr, n - 1);
  
  //frees allocated memory
  delete[] arr;



  return 0;
}

//Mergesort function recurssily calls mergesort to sort the array. 
void mergeSort(int* arr, int start, int end){

  //break condition. if start is not smaller then end then more work needs to be done
  if(start < end){
    
    //deturmins the mid value to split the arrays.
    int mid = (start + end) / 2;

    //Only makes new tasks for large parts of the problem, all other happens on the main thread. 
    if(end - start > 100000){
      #pragma omp parallel
      {
        #pragma omp single
        {
          //creates a new task for both the left and right parts of the array. 
          #pragma omp task
          mergeSort(arr, start, mid);

          #pragma omp task
          mergeSort(arr, mid + 1, end);

          //waits for the task to complete, if this was not here the sort would be incorrect.
          #pragma omp taskwait

        }
      }
      //all other calls happen on the main thread to limit the number of tasks created. 
    } else {
      mergeSort(arr, start, mid);
      mergeSort(arr, mid + 1, end);
    }

    merge(arr, start, end);

  }
}

//merge fucntion, sorts the two arrays as it merges them back together. 
void merge(int* arr, int start, int end){

  //deturmins the mid value to use later. 
  int mid = (start + end) / 2;
  //index needed to trak progess on the temp array
  int index = 0; 

  //creates the temp array.
  int* tempArr = new int[end - start + 1];

  //tracks the progress of the left and right sides of the array.
  int left = start;
  int right = mid + 1;

  //compares the two side of the array to sort as they are placed into the temp array.
  while((left <= mid) && (right <= end)){

    if(arr[left] < arr[right]){
      tempArr[index] = arr[left]; 
      index++;
      left++;
    } else {
      tempArr[index] = arr[right];
      index++;
      right++;
    }
  }

  //adds any left over frop mthe left side to the temp array
  while(left <= mid){
    tempArr[index] = arr[left];
    index++;
    left++;
  } 

  //adds any left over in the right side of the array to the temp array.
  while(right <= end){
    tempArr[index] = arr[right];
    index++;
    right++;
  }

  //places the sorted temp array back into the main array. 
  for(int i = start; i <= end; i++){
    arr[i] = tempArr[i - start];
  }

  //frees allocated memory. 
  delete[] tempArr;

}
    