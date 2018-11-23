#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <chrono>


#ifdef __cplusplus
extern "C" {
#endif

  void generateReduceData (int* arr, size_t n);

#ifdef __cplusplus
}
#endif


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
  
  if (argc < 3) {
    std::cerr<<"usage: "<<argv[0]<<" <n> <nbthreads>"<<std::endl;
    return -1;
  }

  //variable that sets the size of the array to sum
  int n = atoi(argv[1]);
  //sets the number of threads to use.
  omp_set_num_threads(atoi(argv[2]));
  //creates the granularity variable.
  int granularity = 0;
  //sets the size of the granularity basied of the size of the problem. 
  if(n < 1000){
    granularity = n/1;
  } else {
    granularity = n/100;
  }
  //allocated the memory needed for the array.
  int * arr = new int [n];
  
  omp_set_num_threads(16);

  //creates the array to sum
  generateReduceData (arr, atoi(argv[1]));
  //records the starting time of the program.
  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
  //insert reduction code here
  #pragma omp parallel
  {
    //only runs the outer loop once. 
    #pragma omp single
    {
      //creates outer sum veriable
      int sum = 0;
      //inner loop splits the problem into smaller parts to let seperate task run different parts in parallel.
      for(int i = 0; i < n; i += granularity){
        #pragma omp task
        {
          //local sum to reduce wait time. 
          int localSum = 0;
          //std::cout << omp_get_thread_num() << std::endl;
          for(int j = i; j < (i + granularity) && j < n; j++){
            localSum += arr[i];
          }
          //updates outer sum to track the grand total.
          sum += localSum;
        } 
      }
      //all task must finish before printing final answer. 
      #pragma omp taskwait
      //prints total to cout.
      std::cout << sum << std::endl;
    } 
  }
  
  
  //frees allocated memory.
  delete[] arr;

  //records the final time and prints to cerr. 
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end-start;
  std::cerr<<elapsed_seconds.count()<<std::endl;

  return 0;
}
