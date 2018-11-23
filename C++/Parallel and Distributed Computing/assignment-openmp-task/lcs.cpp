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

  void generateLCS(char* X, int m, char* Y, int n);
  void checkLCS(char* X, int m, char* Y, int n, int result);

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

  if (argc < 4) { std::cerr<<"usage: "<<argv[0]<<" <m> <n> <nbthreads>"<<std::endl;
    return -1;
  }

  //creating the variables that dictate the size of the arrys
  int m = atoi(argv[1]);
  int n = atoi(argv[2]);

  // get string data 
  char *X = new char[m];
  char *Y = new char[n];
  //creates the arrays to check
  generateLCS(X, m, Y, n);
  //sets the number of threads to use for the program
  omp_set_num_threads(atoi(argv[3]));
  //sets the schedule type and the granularity
  omp_set_schedule(omp_sched_dynamic, n/100);

  
  //insert LCS code here.
  int result = -1; // length of common subsequence

  //records the start time of the program. 
  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

  //creates the 2D array with dynamicly allocated memory. 
  int** length = new int*[m + 1];
  for(int k = 0; k <= m; k++){
    length[k] = new int[n + 1];
    length[k][0] = 0;
  }

  
  //nested loops to check every combination
  for(int i = 1; i <= m; i++){
    #pragma omp parallel for schedule(runtime)
    for(int j = 1; j <= n; j++){
      //if the chars match the current location is chaged to the value of the previous location plus 1
       if(X[i - 1] == Y[j - 1]){
        length[i][j] = length[i - 1][j - 1] + 1;
       } else {
        //if they do not match then the larger of two lenghts is carried forward. 
        length[i][j] = std::max(length[i - 1][j], length[i][j - 1]);
      }
    }
  }

  //records the longest lenght
  result = length[m][n];
  //std::cout << result << std::endl;

  

  //records the final time to print to the cerr. 
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end-start;
  std::cerr<<elapsed_seconds.count()<<std::endl;
  
  //frees the allocalled memory
  for(int i = 0; i <= m; i++){
    delete[] length[i];
  }
  delete[] length;

  //checks to see if the results is correct. 
  checkLCS(X, m, Y, n, result);


  return 0;
}
