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

//function declaration to calculate GIOPS
void *evaluate_iops(void *threadId);
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
  //time varibales
  clock_t start, end, ediff;
  double cpu_time_used;
  double iops, giops;

  logFile = fopen("cpu-benchmarking-giops.log", "a");
  
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
    pthread_create(&threads[itr], NULL, &evaluate_iops, threadId);
  }

  for(itr = 0; itr < numThreads; itr++) {
    pthread_join(threads[itr], NULL);
  }
  //stopping clock
  end = clock();
  ediff = (double) (end - start);
  cpu_time_used = (double) ediff / CLOCKS_PER_SEC;
  iops = (double) numThreads * 40 * ((NUM_ITERATIONS/numThreads)/cpu_time_used);
  giops = iops / 1000000000;

  fprintf(logFile, "\n\n\nGIOPS, 10^9 IOPS Result");
  fprintf(logFile, "\n***** Thread %d ******", numThreads);
  fprintf(logFile, "\nCPU Time Used: %lf secs", cpu_time_used);
  fprintf(logFile, "\nNumber of IOPS: %lf", iops);
  fprintf(logFile, "\nNumber of GIOPS: %lf\n", giops);
  
  fclose(logFile);
  pthread_exit(NULL);
  return 0;
  
}

// function definition to calculate GIOPS
void *evaluate_iops(void *threadId){
  int total=0;
  int itr;
  long numiter= NUM_ITERATIONS/numThreads;
  for(itr=0; itr < numiter; itr++){
    total = 566 + 7888 + 467 + 899
        + 569 + 7898 + 447 + 839
        + 566 + 7888 + 467 + 899
        + 569 + 7898 + 447 + 839
        + 566 + 7888 + 467 + 899
        + 569 + 7898 + 447 + 839
        + 566 + 7888 + 467 + 899
        + 569 + 7898 + 447 + 839
        + 566 + 7888 + 467 + 899
        + 569 + 7898 + 447;
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
