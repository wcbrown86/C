/*
** Author : William Brown
** Date : March 28th, 2018
** Description : The program implements numerical integration using OpenMP to 
**               make the program parallel.
*/
#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <chrono>

//Instructor provided functions that compute the function of x and intensity
//adds complexity to the function to make it take longer.
#ifdef __cplusplus
extern "C" {
#endif

float f1(float x, int intensity);
float f2(float x, int intensity);
float f3(float x, int intensity);
float f4(float x, int intensity);

#ifdef __cplusplus
}
#endif

//The main function that drives the program, all functions happen in main.
int main(int argc, char *argv[]){

  //records the start time, for computation later to deturmine that time the program took
  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
//forces openmp to create the threads beforehand
#pragma omp parallel
  {
    int fd = open(argv[0], O_RDONLY);
    if (fd != -1)
    {
      close(fd);
    }
    else
    {
      std::cerr << "something is amiss" << std::endl;
    }
  }

  if (argc < 9){
    std::cerr << "Usage: " << argv[0] << " <functionid> <a> <b> <n> <intensity> <nbthreads> <scheduling> <granularity>" << std::endl;
    return -1;
  }

  //insert code here
  //Variables that hold the required information to compute the intergal
  //function ID controls the f function to be called
  int functionid = atoi(argv[1]);
  //A and B are needed to compute the function
  float a = atoi(argv[2]);
  float b = atoi(argv[3]);
  //number of sumations needed to compute the function 
  int n = atoi(argv[4]);
  //variable information to make the funtion take longer. 
  int intensity = atoi(argv[5]);
  //function call to set the number of threads that the system will use
  omp_set_num_threads(atoi(argv[6]));
  //stores the char arr as a string for comparison, deturmins the schedule type.
  std::string sched = argv[7]; 
  //the size of the chunks that will be computed by each thread. 
  int granularity = atoi(argv[8]);
  //global sum variable that stores the computed sum. 
  float sum = 0;

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

  //openMP call that uses multiple threads to compute the problem 
  #pragma omp parallel for schedule(runtime) reduction(+:sum)
  for(int i = 0; i < n; i++){
    //Computation that calculates x.
    float x = (a + (i + 0.5) * ((b - a) / (float)n));
    //local variable that stores the output of the funtions. 
    float f = 0.0f;

    //switch statment that calls the correct f function as set be the user. 
    //function calls requires computed x and intesity. divided by b-a / n.
    switch (functionid){
      case 1:
        f = f1(x, intensity) * ((b - a) / (float)n);
        break;
      case 2:
        f = f2(x, intensity) * ((b - a) / (float)n);
        break;
      case 3:
        f = f3(x, intensity) * ((b - a) / (float)n);
        break;
      case 4:
        f = f4(x, intensity) * ((b - a) / (float)n);
        break;
      default:
        break;
    }
    //updates global sum variable. 
    sum += f;
  }

  //prints the final answer to the comand prompt. 
  std::cout << sum << std::endl;
  //computes the time from end - start then prints the answer to error output.
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end-start;
  std::cerr<<elapsed_seconds.count()<<std::endl;

  return 0;
}
