/*
** Author : William Brown
** Date : March 28th, 2018
** Description : The program implements an array sumation using OpenMP to 
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


//instructor provided functions that generate the array to be summed. 
#ifdef __cplusplus
extern "C" {
#endif
  void generateReduceData (int* arr, size_t n);
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
  
  if (argc < 5) {
    std::cerr<<"Usage: "<<argv[0]<<" <n> <nbthreads> <scheduling> <granularity>"<<std::endl;
    return -1;
  }

  //Variable that stores the size of the array. 
  int n = atoi(argv[1]);
  //creates an array large enough to store the information.
  int * arr = new int [n];
  //openMP function call to set the number of threads to use.
  omp_set_num_threads(atoi(argv[2]));
  //Stores the char array as a string setting the schedule type. 
  std::string sched = argv[3]; 
  //stores the granularity to set the sizes of the chunks.
  int granularity = atoi(argv[4]);
  //global sum to store the running and final value. 
  int sum = 0;

  //nested if statments that set the schedule type and the granularity. 
  //sets schedule type static.
  if(sched == "static"){
    omp_set_schedule(omp_sched_static, granularity);
    //sets schedule type dynamic.
  } else if(sched == "dynamic"){
    omp_set_schedule(omp_sched_dynamic, granularity);
    //sets schedule type to guided.
  } else if(sched == "guided"){
    omp_set_schedule(omp_sched_guided, granularity);
    //sets the schedule type to automatic.
  } else {
    omp_set_schedule(omp_sched_auto, granularity);
  }
  

  //Generates the array data based off the size.
  generateReduceData (arr, atoi(argv[1]));

  //insert reduction code here
  //parallel call to compute the for loop in parallel basied of the information passed.
  #pragma omp parallel for schedule(runtime) reduction(+:sum)
    for(int i = 0; i < n; i++){
      //calculates the sum of the array. 
      sum += arr[i];
    }

    //prints the sum of the array to the comand prompt. 
    std::cout << sum << std::endl;

  //frees the allocated memory created. 
  delete[] arr;

  //computes the time from end - start then prints the answer to error output.
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end-start;
  std::cerr<<elapsed_seconds.count()<<std::endl;
  return 0;
}
