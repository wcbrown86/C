#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <chrono>
#include <cmath>
#include <pthread.h>
#include <ctime>

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

//Declairing functions
void* calcIter(void* startEnd);
void* calcThread(void* startEnd);

//Declaring global variables
int start, functionid, a, b, n, intensity, nbthreads, interAmount;
std::string sync; 
float total;
pthread_mutex_t mut1;


int main (int argc, char* argv[]) {
  //Checking for the correct number of arguments provided
  if (argc < 8) {
    std::cerr<<"usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity> <nbthreads> <sync>"<<std::endl;
    return -1;
  }

  //setting variable values from arguments entered, and starting the time
  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
  functionid = atoi(argv[1]);
  a = atoi(argv[2]);
  b = atoi(argv[3]);
  n = atoi(argv[4]);
  intensity = atoi(argv[5]);
  nbthreads = atoi(argv[6]);
  sync = argv[7];

  //mutex init
  pthread_mutex_init(&mut1, NULL);
  
  //Setting up starting value of total
  total = 0.0f;
  //setting up the size of each iteration based off the number of threads 
  //and number of interations needed total. 
  interAmount = n/nbthreads;

  //Array that will hold the PID of each thread
  pthread_t threads[nbthreads];

  //if statment that will either call the Iteration calculation function or
  //the thread calcuation function.
  if(sync == "iteration"){
    //Loop that creates the number of threads needed.
    for(int i = 0; i < nbthreads; i++){      
     //creates a new memory location for the currnt i for calcuations in the function
     int *j = (int*) malloc(sizeof(*j));
     //stores the value of i in the new memory location
     *j = i;
     //creates the new thread and calls the iteration function.
     pthread_create(&threads[i], NULL, calcIter, j);
   }
  } else {
     for(int i = 0; i < nbthreads; i++){
      //creates a new memory location for the currnt i for calcuations in the function
      int *j = (int*) malloc(sizeof(*j));
      //stores the value of i in the new memory location
      *j = i;
      //Creates the new thread and calls the thread calculation function
      pthread_create(&threads[i], NULL, calcThread, j);
   }
  }
  
  //Loop that waits for the threads to be complete to close out the threads
  for(int i = 0; i < nbthreads; i++){
    pthread_join(threads[i], NULL);
  }

  //prints out the total to the stdout output.
  std::cout << total << std::endl;
  
  //Prints the time to the stderr output
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end-start;
  std::cerr<<elapsed_seconds.count()<<std::endl;

  //Closes out the mutex needed to protect the global varable. 
  pthread_mutex_destroy(&mut1);
  
  return 0;
}

void* calcIter(void* startEnd){

  //setting up the information passed to the function
  const int *val = (int*) startEnd;
  //Calculates the start and end point for the thread based off the thread number
  //or index
  int start = val[0] * interAmount;
  int end = (val[0] + 1) * interAmount;

  //Loop that calculates the intregration
  for (int i = start; i < end; i++)
  {
    //Local variables.
    float bminusa = b - a;
    float division = bminusa / n;
    float iplus = i + .5;
    float mulit = iplus * division;
    float x = a + mulit;
    float f = 0.0f;

    //switch that chooses the correct f function to call from the users input
    switch (functionid)
    {
    case 1:
      f = f1(x, intensity);
      break;
    case 2:
      f = f2(x, intensity);
      break;
    case 3:
      f = f3(x, intensity);
      break;
    case 4:
      f = f4(x, intensity);
      break;
    }

    //final calcuation need to complete the math
    float finalA = (f * division);

    //locks the global variable to protect the information being added in
    //each iteration of the for loop.
    pthread_mutex_lock(&mut1);
    //adds to the total
    total += finalA;
    //unlocks the variable to relases to another thread.
    pthread_mutex_unlock(&mut1);
  }

  return 0;
}

void* calcThread(void* startEnd){

  //setting up the information passed to the function
  const int *val = (int*) startEnd;
  //Calculates the start and end point for the thread based off the thread number
  //or index
  int start = val[0] * interAmount;
  int end = (val[0] + 1) * interAmount;

  //creates local variable that stores the value of the loop.
  float runningTotal = 0.0f;

  //Loop that calculates the intregration
  for (int i = start; i < end; i++)
  {
    //Local variables.
    float bminusa = b - a;
    float division = bminusa / n;
    float iplus = i + .5;
    float mulit = iplus * division;
    float x = a + mulit;
    float f = 0.0f;

    //switch that chooses the correct f function to call from the users input
    switch (functionid)
    {
    case 1:
      f = f1(x, intensity);
      break;
    case 2:
      f = f2(x, intensity);
      break;
    case 3:
      f = f3(x, intensity);
      break;
    case 4:
      f = f4(x, intensity);
      break;
    }

    //final calcuation need to complete the math
    float finalA = (f * division);
    //keeps the running total to be added to the global total at the end.
    runningTotal += finalA;
  }
    //locks the global variable to protect the information being added in
    //each iteration of the for loop.
    pthread_mutex_lock(&mut1);
    //Adds value of the loop total to the global total.
    total += runningTotal;
    //unlocks the variable to relases to another thread.
    pthread_mutex_unlock(&mut1);

    return 0;
}



