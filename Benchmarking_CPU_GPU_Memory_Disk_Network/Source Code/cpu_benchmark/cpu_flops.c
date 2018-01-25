/*
 Created By: Aditya Jadhav
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_ITERATIONS 1000000000
//#define EXIT_FAILURE -1
#define true 1
#define false 0

//output file
FILE *logFile;

// function declaration to calculate GFLOPS
void *evaluate_flops(void *threadId);
void save_message(char *msg, int isDisplay);
int numThreads;

int main(int argc, char *argv[]) {

  if(argc != 2) {
    save_message("usage: ./a.out <number of threads>", true);
    return EXIT_FAILURE;
  }

  //taking number of threads as command line arguments
  numThreads = atoi(argv[1]);
  pthread_t threads[numThreads];
  int itr; 
  // clock varibales
  clock_t start, end, ediff;
  double cpu_time_used;
  double flops, gflops;

  logFile = fopen("cpu-benchmarking-gflops.log", "a");
  
  if (logFile == NULL) {
    save_message("Error opening file", true);
  }
  //starting clock
  start = clock();
  for(itr = 0; itr < numThreads; itr++) {  
    int *threadId = malloc(sizeof(*threadId));
    *threadId = itr + 1;
   /* Defination: create a new thread
    * int pthread_create(pthread_t *thread, const pthread_attr_t *attr,void *(*start_routine) (void *), void *arg);                          
    */  
    pthread_create(&threads[itr], NULL, &evaluate_flops, threadId);
  }

  for(itr = 0; itr < numThreads; itr++) {   
    pthread_join(threads[itr], NULL);
  }
  //stopping clock
  end = clock();
  
  ediff = (double) (end - start);
  cpu_time_used = (double) ediff / CLOCKS_PER_SEC;
  flops = (double) numThreads * 40 * ((NUM_ITERATIONS/numThreads)/cpu_time_used);
  gflops = flops / 1000000000;
  fprintf(logFile, "\n\n\nGFLOPS, 10^9 FLOPS Result");
  fprintf(logFile, "\n\n***** Thread %d ******", numThreads);
  fprintf(logFile, "\nCPU Time Used: %lf secs", cpu_time_used);
  fprintf(logFile, "\nNumber of FLOPS: %lf", flops);
  fprintf(logFile, "\nNumber of GFLOPS: %lf", gflops);
  fclose(logFile);
  pthread_exit(NULL);
  return 0; 
}

// function definition to calculate GIOPS
void *evaluate_flops(void *threadId) {
  int itr;
  float total=0;
  long numiter= NUM_ITERATIONS/numThreads;
  for(itr = 0; itr < numiter; itr++) {
    total = 5.66 + 7.888 + 4.67 + 8.99
        + 5.69 + 7.898 + 4.47 + 8.39
        + 5.66 + 7.888 + 4.67 + 8.99
        + 5.69 + 7.898 + 4.47 + 8.39
        + 5.66 + 7.888 + 4.67 + 8.99
        + 5.66 + 7.888 + 4.67 + 8.99
        + 5.69 + 7.898 + 4.47 + 8.39
        + 5.66 + 7.888 + 4.67 + 8.99
        + 5.69 + 7.898 + 4.47 + 8.39
        + 5.66 + 7.888 + 4.67 + 8.99;
  }
  pthread_exit(NULL);
}

void save_message(char *msg, int isDisplay) {
  if(isDisplay == true) {
    fprintf(stderr, msg);
    fprintf(stderr, "\n");
  }
  fprintf(logFile, msg);
}
