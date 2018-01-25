/*
 Created By: Sumedha Gupta
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#define memorySize 1073741824 // memory of size 1GB
#define mb 1024*1024 //to convert throughput to MB/sec
#define micro 1000000 // to convert latency to microsecs
#define true 1
#define false 0

/* Output file*/
FILE *logFile;

int numThreads; //number of threads
char *source; //source memory
char *dest; //dest memory


void save_message(char *msg, int isDisplay);

//Read - Write Sequentially
void *readWriteSeq(void *blockSize){
  long size = *(long *) blockSize;
  long int number_iter= ((long)memorySize/size)/numThreads;
  int index=0;
  long i=0;
  for (i=0; i<number_iter;i++){
    memcpy(&dest[index], &source[index],size);
    index= (index+size)%memorySize; //reading sequentially
  }
  pthread_exit(NULL);
}

/*Read - Write Randomly*/
void *readWriteRand(void *blockSize){
  long size =*(long *) blockSize;
  long int number_iter= ((long)memorySize/size)/numThreads;
  int index=0;
  long i=0;
  for (i=0; i<number_iter;i++){
    index =  rand()% (memorySize- (size+1)); //pointing to a random loaction
    memcpy(&dest[index], &source[index],size);
  }
  pthread_exit(NULL);
}

/*Write Randomly*/
void *writeRand(void *blockSize){
  long size = *(long *)blockSize;
  long int number_iter= ((long)memorySize/size)/numThreads;
  int index=0;
  long i=0;
  for (i=0; i<number_iter;i++){
    index =  rand()% (memorySize- (size+1)); //pointing to a random loaction
    memset(&dest[index],'a',size);
  }
  pthread_exit(NULL);
}

/*Write Sequentially*/
void *writeSeq(void *blockSize){
  long size = *(long *)blockSize;
  long int number_iter= ((long)memorySize/size)/numThreads;
  int index=0;
  long i=0;
  for (i=0; i<number_iter;i++){
   memset(&dest[index],'a',size);
    index= (index+size)%memorySize; //writing sequentially
  }
  pthread_exit(NULL);
}

/* Program takes two arguments
* a. Number of threads
* b. Block Size in bytes e.g. 8,8192,8388608,83886080
*/
int main(int argc, char *argv[]) {

  if(argc != 3) {
    save_message("usage: ./a.out <number of threads> <block size>", true);
    return EXIT_FAILURE;
  }

  source = malloc(memorySize * sizeof(char)); //allocating source memory
  dest = malloc(memorySize * sizeof(char)); //allocating dest memory

  // handling command line arguments
  numThreads = atoi(argv[1]);
  long blockSize= atoi(argv[2]);

  //declaring threads
  pthread_t threads[numThreads];
  int itr; 
  //timing variables
  clock_t start, end, ediff;
  double cpu_time_used;
  double throughPut, latency;
  
  //output log file
  logFile = fopen("memory-benchmarking.log", "a");
  fprintf(logFile, "\n\n***** Thread %d ******", numThreads);

  if (logFile == NULL) {
    save_message("Error opening file", true);
  }
  
  // calculating number of iterations to facilitate strong scaling
  long int number_iter= ((long)memorySize/blockSize)/numThreads;

  /* Read and Write Sequentially*/
  //starting clock
  start = clock();
  for(itr = 0; itr <numThreads; itr++) {
    pthread_create(&threads[itr], NULL, &readWriteSeq, &blockSize);
  }

  for(itr = 0; itr <numThreads; itr++) { 
    pthread_join(threads[itr], NULL);
  }
  //ending clock
  end = clock();
  ediff = (double) (end - start);
  cpu_time_used = (double) ediff / CLOCKS_PER_SEC;
  throughPut = (double)(numThreads * number_iter * blockSize)/(cpu_time_used * mb);
  latency = (cpu_time_used*micro) / ((double)(numThreads) * (number_iter)* blockSize/mb);

  fprintf(logFile, "\n****Read and Write Sequentially***\n");  
  fprintf(logFile, "\nCPU Time Used: %lf secs", cpu_time_used);  
  fprintf(logFile, "\nBlock Size: %ld bytes", blockSize);
  fprintf(logFile, "\nThroughput: %lf MB/sec", throughPut);
  fprintf(logFile, "\nLatency: %lf microsecs\n", latency);

  /* Read and Write Randomly*/
  //starting clock
  start = clock();
  for(itr = 0; itr <numThreads; itr++) {
    pthread_create(&threads[itr], NULL, &readWriteRand, &blockSize);
  }

  for(itr = 0; itr <numThreads; itr++) { 
    pthread_join(threads[itr], NULL);
  }
  //ending clock
  end = clock();
  ediff = (double) (end - start);
  cpu_time_used = (double) ediff / CLOCKS_PER_SEC;
  throughPut = (double)(numThreads * number_iter * blockSize)/(cpu_time_used * mb);
  latency = (cpu_time_used*micro) / ((double)(numThreads) * (number_iter)* blockSize/mb);

  fprintf(logFile, "\n****Read and Write Randomly***\n");  
  fprintf(logFile, "\nCPU Time Used: %lf secs", cpu_time_used);  
  fprintf(logFile, "\nBlock Size: %ld bytes", blockSize);
  fprintf(logFile, "\nThroughput: %lf MB/sec", throughPut);
  fprintf(logFile, "\nLatency: %lf microsecs\n", latency);

  /* Write Sequentially*/
  //starting clock
  start = clock();
  for(itr = 0; itr <numThreads; itr++) {
    int *threadId = malloc(sizeof(*threadId));
    pthread_create(&threads[itr], NULL, &writeSeq, &blockSize);
  }
  for(itr = 0; itr <numThreads; itr++) { 
    pthread_join(threads[itr], NULL);
  }
  //ending clock
  end = clock();
  ediff = (double) (end - start);
  cpu_time_used = 1.0*(double) ediff / CLOCKS_PER_SEC;
  throughPut = (double)(numThreads * number_iter * blockSize)/(cpu_time_used * mb);
  latency = (cpu_time_used * micro) / ((double)(numThreads) * (number_iter)* blockSize/mb);
 
  fprintf(logFile, "\n****Write Sequentially***\n");  
  fprintf(logFile, "\nCPU Time Used: %lf secs", cpu_time_used);  
  fprintf(logFile, "\nBlock Size: %ld bytes", blockSize);
  fprintf(logFile, "\nThroughput: %lf MB/sec", throughPut);
  fprintf(logFile, "\nLatency: %lf microsecs\n", latency);

  /* Write Randomly*/
  //starting clock
  start = clock();
  for(itr = 0; itr <numThreads; itr++) {
    int *threadId = malloc(sizeof(*threadId));
    pthread_create(&threads[itr], NULL, &writeRand, &blockSize);
  }
  for(itr = 0; itr <numThreads; itr++) { 
    pthread_join(threads[itr], NULL);
  }
  //ending clock
  end = clock();
  ediff = (double) (end - start);
  cpu_time_used = 1.0*(double) ediff / CLOCKS_PER_SEC;
  throughPut = (double)(numThreads * number_iter * blockSize) /(cpu_time_used * mb);
  latency = (cpu_time_used*micro) / ((double)(numThreads) * (number_iter)* blockSize/mb);

  fprintf(logFile, "\n****Write Randomly***\n");  
  fprintf(logFile, "\nCPU Time Used: %lf secs", cpu_time_used);  
  fprintf(logFile, "\nBlock Size: %ld bytes", blockSize);
  fprintf(logFile, "\nThroughput: %lf MB/sec", throughPut);
  fprintf(logFile, "\nLatency: %lf microsecs\n", latency);
  fclose(logFile); //closing log file
  pthread_exit(NULL);
  return 0;
}
void save_message(char *msg, int isDisplay) {
  if(isDisplay == true) {
  fprintf(stderr, msg);
  fprintf(stderr, "\n");
  }
  fprintf(logFile, msg);
}
