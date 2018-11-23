#include <iostream>
#include <cmath>
#include <cstdlib>
#include <chrono>
#include <mpi.h>

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

//Global variables, used by all processes. 
int functionid, n, intensity;
float a, b, sum;

  
int main (int argc, char* argv[]) {
  
  if (argc < 6) {
    std::cerr<<"usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity>"<<std::endl;
    return -1;
  }

  //MPI init To start the program on multiple machines. 
  MPI_Init(&argc, &argv);

  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

  //Variables used to store the needed values to calcuate the funciton. 
  functionid = atoi(argv[1]);
  a = atoi(argv[2]);
  b = atoi(argv[3]);
  n = atoi(argv[4]);
  intensity = atoi(argv[5]);
 

  //Gets the number of the machines being used. This value is needed to split the work
  int size; 
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  //Gets the current machine number. This is used to know what part of the work to do. 
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  //local sum that each machine uses. then updates the global sum. 
  float localSum = 0;
  int startI = rank * (n/size);
  int endI =  (rank + 1) * (n/size);

  //Integration loop. 
  for(int i = startI; i < endI; i++){
      float f = 0.0f;

      switch (functionid){
        case 1:
          f = f1((a + (i + 0.5) * ((b - a) / (float)n)), intensity) * ((b - a) / (float)n);
          break;
        case 2:
          f = f2((a + (i + 0.5) * ((b - a) / (float)n)), intensity) * ((b - a) / (float)n);
          break;
        case 3:
          f = f3((a + (i + 0.5) * ((b - a) / (float)n)), intensity) * ((b - a) / (float)n);
          break;
        case 4:
          f = f4((a + (i + 0.5) * ((b - a) / (float)n)), intensity) * ((b - a) / (float)n);
          break;
        default:
          break;
      }

      localSum += f;
  }

  //sums all of the local sums to the global sum in process 0 
  MPI_Allreduce(&localSum, &sum, 1, MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);

  //prints the sum from process 0
  if(rank == 0){
    std::cout << sum << std::endl;
    
  }
  
  //each procees prints the time it took to run to stderr. 
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end-start;
  std::cerr<<elapsed_seconds.count()<<std::endl;
  
  //Closes out MPI
  MPI_Finalize();

  return 0;
}
