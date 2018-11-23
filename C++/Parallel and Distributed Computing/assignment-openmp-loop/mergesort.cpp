/*
** Author : William Brown
** Date : March 28th, 2018
** Description : The program implements Merge Sort with for loops, using OpenMP to 
**               make parts of the program parallel.
*/
#include <omp.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <chrono>

//Instructor provided functions to generate the array to sort and to check the sorted array.
#ifdef __cplusplus
extern "C" {
#endif
  void generateMergeSortData (int* arr, size_t n);
  void checkMergeSortResult (int* arr, size_t n);
#ifdef __cplusplus
}
#endif

//Defining the functions used to accomplish Merge Sort
void mergeSort(int arr[], int start, int mid, int end, int size);
int min(int x, int y);

//Main driver function. 
int main (int argc, char* argv[]) {

  //Starts the clock to track the time the program takes. 
  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

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

  //Size of the array to be created and sorted
  int n = atoi(argv[1]);
  //openMP function call to set the number of threads to use.
  omp_set_num_threads(atoi(argv[2]));
  //Variable to be used to control the size of the parallel chunks
  int granularity = n/atoi(argv[2]);
  //openMP function call to set the runtime threads and schedule type. 
  omp_set_schedule(omp_sched_dynamic, granularity);  

  // get arr data
  int * arr = new int [n];
  generateMergeSortData (arr, n);
  
  //insert sorting code here.
  //Outer for controls the size of the arrays to merge
  for (int current = 1; current < n - 1; current *= 2){
    //Inner loop runs in parallel and calles the merge sort function
    #pragma omp parallel for schedule(runtime)
    for (int leftStart = 0; leftStart < n - 1; leftStart += 2 * current){
      //Variables to set the mid count for left end and right start
      int mid = leftStart + current - 1;
      //end variable that sets the end of the right array
      int end = min(leftStart + 2 * current - 1, n -1);
      //Call to mergeSort needs the arr, the start, mid, end, and n variable.
      mergeSort(arr, leftStart, mid, end, n);
    }
    
  }
  //Function call that checks that the array is sorted correctly. 
  checkMergeSortResult (arr, n);
  
  //Deletes the memory allocation for the arr variable.
  delete[] arr;

  //Calculates the end time form the start time and prints the diffence to the error logs for reporting
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end-start;
  std::cerr<<elapsed_seconds.count()<<std::endl;

  return 0;
}

//Merge Sort function, takes the start, end, and mid variables to create the subarrays
//then merges the subarrays together sorted in asending order.
void mergeSort(int arr[], int start, int mid, int end, int size){
    //index variables needed to track the progress of the loops that merges the arrays
    int i, j, k;
    //variables that set the size of the left - n1 and right - n2 arrays
    int n1 = mid - start + 1;
    int n2 = end - mid;

    //dynamic allocation of memory for the subarrays, needed to to the 
    //size of the arrays to be merged. 
    int * leftArr = (int *) malloc(sizeof(int) * n1);
    int * rightArr = (int *) malloc(sizeof(int) * n2);

    //For loop that puts the values in the left array
    for (i = 0; i < n1; i++){
      if(start + i <= size){
        leftArr[i] = arr[start + i];
      }
    }
    //For loops that puts the values in the right array
    for (j = 0; j < n2; j++){
      if(mid + 1 + j <= size){
        rightArr[j] = arr[mid + 1 + j];
      }
    }
    //Resets the index variables to aide the merge of the arrays
    i = 0;
    j = 0;
    //position of the main array
    k = start;
    //while loop that checks the values and merges the arrays
    //in asending order. 
    while (i < n1 && j < n2 && k < size){
        if (leftArr[i] <= rightArr[j]){
            arr[k] = leftArr[i];
            i++;
        } else {
            arr[k] = rightArr[j];
            j++;
        }
        k++;
    }

    //while loops the adds any remaining values in the left array
    //back to the main array
    while (i < n1 && k < size)
    {
        arr[k] = leftArr[i];
        i++;
        k++;
    }
    //while loops that adds and remaining values in the right array
    //back to the main array.
    while (j < n2 && k < size)
    {
        arr[k] = rightArr[j];
        j++;
        k++;
    }

    //deletes the allocated memory for the sudarrays to keep the memory 
    //allocation in check. 
    delete[] leftArr;
    delete[] rightArr;
}


//function to find minimum of two integers
int min(int x, int y){
    if(x < y){
        return x;
    } else {
        return y;
    }
}