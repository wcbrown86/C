/*
** Author - William Chad Brown
** Student ID - 800816688
** Date - 03/15/2018
*/
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

//Global Variables
float a, b, n, sum;
int functionid, intensity, nbthreads, granularity, count;
std::string sync;
pthread_mutex_t mut1, mut2, mut3;

//Function decleration
bool done();
void getNext(int* start, int* end);
void* loopChunk(void* arguments);
void* loopThread(void* arguments);
void* loopIter(void* arguments);
void calcChunk(int start, int end);
float calcThread(int start, int end);
void calcIter(int start, int end);

int main(int argc, char *argv[]){

  //Starting time stored in start variable
  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
  if (argc < 9){
    std::cerr << "usage: " << argv[0] << " <functionid> <a> <b> <n> <intensity> <nbthreads> <sync> <granularity>" << std::endl;
    return -1;
  }

  //setting variables values from arguments entered.
  functionid = atoi(argv[1]);
  a = atoi(argv[2]);
  b = atoi(argv[3]);
  n = atoi(argv[4]);
  intensity = atoi(argv[5]);
  nbthreads = atoi(argv[6]);
  sync = argv[7];
  granularity = atoi(argv[8]);
  sum = 0.0f;
  count = 0;

  //Thread array.
  pthread_t threads[nbthreads];

  //mutex init
  pthread_mutex_init(&mut1, NULL);
  pthread_mutex_init(&mut2, NULL);
  pthread_mutex_init(&mut3, NULL);
  //throw away veriable. 
  int *type;

  //If statment that selects the correct update type.
  //Chuck update type. updates after the calculation of the inner loop.
  if (sync == "chunk"){
    //for loop that creates the number of threads based off user input.
    for (int i = 0; i < nbthreads; i++){
      //pthread call to create new thread.
      pthread_create(&threads[i], NULL, loopChunk, type);
    }
    //Thread update type. updates after the thread is done.
  } else if (sync == "thread"){
    //for loop that creates the number of threads based off the user input.
    for (int i =0; i < nbthreads; i++){
      //pthread call to create new thread.
      pthread_create(&threads[i], NULL, loopThread, type);
    }
    //Iteration update type. updates after each loop of the calcuation for loop.
  } else {
    //for loop that creates the number of threads based off user input.
    for(int i = 0; i < nbthreads; i++){
      //pthread call to create new thread.
      pthread_create(&threads[i], NULL, loopIter, type);
    }

  }
  //for loop that loops threw all of the created threads and waits for each to complete.
  for (int i = 0; i < nbthreads; i++){
    //waits for each thread to complete and be closed. 
    pthread_join(threads[i], NULL);
  }
  //Closes out the mutexs 
  pthread_mutex_destroy(&mut1);
  pthread_mutex_destroy(&mut2);
  pthread_mutex_destroy(&mut3);

  //Prints out the sum to the standard out put.
  std::cout << sum << std::endl;

  //Calculates the final time and prints std::cerr
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end - start;
  std::cerr << elapsed_seconds.count() << std::endl;
  return 0;
}

//Function that checks if the work to be done is complete
bool done(){
  
  //Locks the count global veriable to prevent changes made be other threads
  //from changing and affecting the progess. 
  pthread_mutex_lock(&mut3);

  //checks to see if the count variable is eqaul to n 
  //if count is equal then no more work needs to be done.
  if (count == n){
    //unlocks count.
    pthread_mutex_unlock(&mut3);
    //returns true to kick out of the while loop
    return true;
  } else {
    //unlocks count if more work needs to be done. 
    pthread_mutex_unlock(&mut3);
    //returns false to keep the loop going. 
    return false;
  }
  
}

//Get next function uses the count variable and referances to start and end.
//count keeps track of the current location of the work that still needs to be done.
void getNext(int* start, int* end){
  
  //Locks the count veriable to prevent data race. 
  pthread_mutex_lock(&mut2);    
  //Checks to see if the work is at the last block. if so end is set to n to prevent 
  //the work from going over. 
  if ((count + granularity) >= n){
    //sets veriable to the last block that needs work. 
    *start = count;
    count = n;
    *end = n;
    //unlocks count for other threads to access.
    pthread_mutex_unlock(&mut2);
    //sets starty and end base of the next block of work that needs to be completed.
  } else{
    //sets the variables to the currnt block of work.
    *start = count;
    count += granularity;
    *end = count;
    //unlocks the count variable. 
    pthread_mutex_unlock(&mut2);
  }
}

//Chunk loop. this is the function that is called when the thread is created.
//This is the set of instructions that calls the correct functions to complete the problem
void* loopChunk(void* arguments){
  //While loop that continues until done() returns true. 
  while(!done()){
    //creates start and end veriables. 
    int start, end;

    //sets start and end based of count and granularity.
    getNext(&start, &end);
    //Function call that does the calculations needed to produce the correct output.
    calcChunk(start, end);
  }

  return 0;

}

//Calcuation function for the chunk update type. the global sum variable is updated once the loop is complete
void calcChunk(int start, int end){
  //creates local variable that stores the runing sum.
  float total = 0.0f;
  //loops that calcualtes the function.
  for (int i = start; i < end; i++)
  {
    //calcualtes x with the provided formula.
    float x = (a + (i + 0.5) * ((b - a) / n));
    //variable that stores the output from the funciton call to F(x).
    float f = 0.0f;
    //Switch that calls the correct f(x) function with the provied intensity.
    switch (functionid)
    {
    case 1:
      f = f1(x, intensity) * ((b - a) / n);
      break;
    case 2:
      f = f2(x, intensity) * ((b - a) / n);
      break;
    case 3:
      f = f3(x, intensity) * ((b - a) / n);
      break;
    case 4:
      f = f4(x, intensity) * ((b - a) / n);
      break;
    }
    //updates local running variable.
    total += f; 
  }
  //locks the global sum variable before updating
  pthread_mutex_lock(&mut1);
  //updates the local sum with the running sum.
  sum += total;  
  //unlocks the global sum for other threads to accress.
  pthread_mutex_unlock(&mut1);
  
}

//Thread loop. this is the function that is called when the thread is created.
//This is the set of instructions that calls the correct functions to complete the problem
void* loopThread(void* arguments){

  //creates local variable the stores the running sum.
  float localSum = 0.0f;
  //While loop that continues until done() returns true.
  while(!done()){
    //creates start and end veriables.
    int start, end;
    //sets start and end based of count and granularity.
    getNext(&start,&end);
    //Function call that does the calculations needed to produce the correct output.
    //this function return the float to be stored and will update sum when all work is complete.
    float returnVal = calcThread(start, end);
    //udates local variable that stores the running sum.
    localSum += returnVal;

  }
  //locks the global sum variable before updating
  pthread_mutex_lock(&mut1);
  //updates the local sum with the running sum.
  sum += localSum;
  //unlocks the global sum for other threads to accress.
  pthread_mutex_unlock(&mut1);

  return 0;
}

//Calcuation function for the thread update type. The function returns the local sum to be 
//updated to the global sum once all work is complete.
float calcThread(int start, int end){
  //creates local variable that stores the runing sum.
  float total = 0.0f;
  //loops that calcualtes the function.
  for (int i = start; i < end; i++){
    //calcualtes x with the provided formula.
    float x = (a + (i + 0.5) * ((b - a) / n));
    //variable that stores the output from the funciton call to F(x).
    float f = 0.0f;
    //Switch that calls the correct f(x) function with the provied intensity.
    switch (functionid)
    {
    case 1:
      f = f1(x, intensity) * ((b - a) / n);
      break;
    case 2:
      f = f2(x, intensity) * ((b - a) / n);
      break;
    case 3:
      f = f3(x, intensity) * ((b - a) / n);
      break;
    case 4:
      f = f4(x, intensity) * ((b - a) / n);
      break;
    }
    
    //updates local running sum
    total += f;
  }
  //returns the running sum to be stored and the thread level before update to global variable.
  return total;
  
}

//Iteration loop. this is the function that is called when the thread is created.
//This is the set of instructions that calls the correct functions to complete the problem
void* loopIter(void* arguments){

  //While loop that continues until done() returns true.
  while(!done()){
    //creates start and end veriables.
    int start, end;
    //sets start and end based of count and granularity.
    getNext(&start, &end);
    //Function call that does the calculations needed to produce the correct output.
    calcIter(start, end);
  }

  return 0;
}

//Calcuation function for the iteration update type. The global sum is updated after each 
//calcuation no local sum variable. 
void calcIter(int start, int end){
  //loops that calcualtes the function.
  for (int i = start; i < end; i++){
    //calcualtes x with the provided formula.
    float x = (a + (i + 0.5) * ((b - a) / n));
    //variable that stores the output from the funciton call to F(x).
    float f = 0.0f;
    //Switch that calls the correct f(x) function with the provied intensity.
    switch (functionid)
    {
    case 1:
      f = f1(x, intensity) * ((b - a) / n);
      break;
    case 2:
      f = f2(x, intensity) * ((b - a) / n);
      break;
    case 3:
      f = f3(x, intensity) * ((b - a) / n);
      break;
    case 4:
      f = f4(x, intensity) * ((b - a) / n);
      break;
    }
    //locks the global sum variable before updating
    pthread_mutex_lock(&mut1);
    //updates the local sum with the running sum.
    sum += f; 
    //unlocks the global sum for other threads to accress.
    pthread_mutex_unlock(&mut1);
  }
}
