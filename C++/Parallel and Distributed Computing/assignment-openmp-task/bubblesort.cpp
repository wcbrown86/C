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
  void checkMergeSortResult (const int* arr, size_t n);

#ifdef __cplusplus
}
#endif

//function that preforms the swap indexs in the array
void swap(int index, int * array);

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
  
  if (argc < 3) { std::cerr<<"usage: "<<argv[0]<<" <n> <nbthreads>"<<std::endl;
    return -1;
  }

  int n = atoi(argv[1]);
  
  // get arr data
  int * arr = new int [n];
  //sets up the number of threads to use
  omp_set_num_threads(atoi(argv[2]));
  //sets up the schedule type and granularity. 
  omp_set_schedule(omp_sched_dynamic, n/100);
  generateMergeSortData (arr, n);

  //insert sorting code here.
  //records the time the program starts
  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

  //Loop that controls the over all progress. 
  for(int i = 0; i < n; i++){

    //to make the program prallel I split the process. one sorts even the other sorts odd
    if(i % 2 == 0){
      //for loop that sorts even
      #pragma omp parallel for schedule(runtime)
      for(int j = 2; j < n; j += 2){
        if(arr[j-1] > arr[j])
          swap(j, arr);
      }
    } else {
      //the for loop that sorts odd.
      #pragma omp parallel for schedule(runtime)
      for(int j = 1; j < n; j += 2){
        if(arr[j-1] > arr[j])
          swap(j, arr);
      }
    }
  }


  //checks the array for correctness.
  checkMergeSortResult (arr, n);
  
  //frees allocated memory
  delete[] arr;
  //records the final time and prints to cerr. 
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end-start;
  std::cerr<<elapsed_seconds.count()<<std::endl;
  return 0;
}
//Swap fucntion that preforms the swap of two elements in an array.
void swap(int index, int * array){
  int temp = array[index - 1];
  array[index - 1] = array[index];
  array[index] = temp;

}
