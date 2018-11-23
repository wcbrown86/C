/*
** Author : William Brown
** Date : March 28th, 2018
** Description : The program implements prefix sum using OpenMP to 
**               make parts of the program parallel.
*/
#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <chrono>

//Instructor provided functions that generate the arr, then,
//verify the process is correct. 
#ifdef __cplusplus
extern "C" {
#endif
  void generatePrefixSumData (int* arr, size_t n);
  void checkPrefixSumResult (int* arr, size_t n);
#ifdef __cplusplus
}
#endif

//Driver function, all processes happen in the main function. 
int main (int argc, char* argv[]) {
  
  //records the start time for computation later. 
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
  
  if (argc < 3) {
    std::cerr<<"Usage: "<<argv[0]<<" <n> <nbthreads>"<<std::endl;
    return -1;
  }

  //Variable that stores the size of the array
  int n = atoi(argv[1]);
  //openMP fucntion call that sets the number of threads to use.
  omp_set_num_threads(atoi(argv[2]));
  //Calculates the granularity from the size of n and number of threads
  int granularity = n/atoi(argv[2]);
  //openMP function call that sets teh schedule to static and passes the granularity. 
  omp_set_schedule(omp_sched_static, granularity);

  //Creates the arr to the size needed. 
  int * arr = new int [n];
  //Generates the array data based off the size.
  generatePrefixSumData (arr, n);

  //Creates the second array that is once size larger for the final calulation
  int * pr = new int [n+1];
  //insert prefix sum code here
  //copies the information in the crated array to the second array. 
  pr[0] = arr[0];
  //openMP call that creates the multiple threads. and allows parallel processing.
  #pragma omp parallel
  { 
  //parallel for that sets the schedule from the informaiton passed above. 
  //first for loop calculates the prefix sum in chunks, this will be off by
  //the last value of the previous chunk.
  #pragma omp parallel for schedule(runtime)
  for(int i = 1; i < n; i++){
    pr[i+1] = pr[i] + arr[i];
  }
  }
  //creates a variable to store the value that the calcuation is off by.
  int adjus = 0;
  //outerfor loop tracks the offset of the currnt plus the previous values.
  for(int i = granularity; i < n; i += granularity){
    adjus += pr[i];
    //inner loop runs in parallel, this loop adds the missing amount to that chunk of the array.
    #pragma omp parallel for
    for(int j = i; j < j + granularity; j++){
      pr[i] += adjus;
    }
  }
  //function call that checks to see if the array is correct.   
  checkPrefixSumResult(pr, n);

  //frees the allocated memory crated for the array.
  delete[] arr;
  //calculates the time the program took to run end - start and prints the time to error. 
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end-start;
  std::cerr<<elapsed_seconds.count()<<std::endl;

  return 0;
}
