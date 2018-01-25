/*
 Created By: Sumedha Gupta
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>

#define FILE_SIZE 1073741824 //file size of 1 GB
#define MB 1024*1024 // to convert throughput to MB/sec
#define milli 1000 //to convert latency to millisecs
#define true 1
#define false 0

/* Output file*/
FILE *logFile;

int numThreads; //number of threads
int blockSize; //block Size in bytes e.g. 8,8192,8388608,83886080

char *fileName;// fileName to be read from user

void save_message(char *msg, int isDisplay);

//Read - Write Sequentially
void *readWriteSeq(void *bufferSize) {
	long int numIter = ((long)FILE_SIZE / blockSize) / numThreads;
	char *buf = malloc(blockSize * sizeof(char));
	long i;
	FILE *fileRead = fopen (fileName, "r"); //open file in read mode
	for (i = 0; i < numIter; i++) {
		fread(buf, blockSize, 1, fileRead);
	}
	free(buf); //free buffer
	fclose(fileRead); //close the file
}

/*Read - Write Randomly*/
void *readWriteRand(void *bufferSize) {
	long int numIter = ((long)FILE_SIZE / blockSize) / numThreads;
	char *buf = malloc(blockSize * sizeof(char));
	long i;
	FILE *fileRead = fopen (fileName, "r"); //open file in read mode
	for (i = 0; i < numIter; i++) {
		//pointing to a ramdom location
		fseek(fileRead, (rand() % sizeof(buf)), SEEK_SET); 
		fread(buf, blockSize, 1, fileRead);
	}
	free(buf); //free buffer
	fclose(fileRead); //close the file
}

void *writeSeq(void *bufferSize) {
	long int numIter = ((long)FILE_SIZE / blockSize) / numThreads;
	char *buf = malloc(blockSize * sizeof(char));
	long i;
	FILE *fileWrite = fopen (fileName, "a");//open file in append mode
	for (i = 0; i < numIter; i++) {
		fwrite(buf, blockSize, 1, fileWrite);
	}
	free(buf);
	fclose(fileWrite);
}

void *writeRand(void *bufferSize) {
	long int numIter = ((long)FILE_SIZE / blockSize) / numThreads;
	char *buf = malloc(blockSize * sizeof(char));
	long i;
	FILE *fileWrite = fopen (fileName, "a");//open file in append mode
	for (i = 0; i < numIter; i++) {
		//pointing to a ramdom location
		fseek(fileWrite, (rand() % sizeof(buf)), SEEK_SET);
		fwrite(buf, blockSize, 1, fileWrite);
	}
	free(buf);
	fclose(fileWrite);
}

/* Program takes three arguments
* a. Number of threads
* b. Block Size in bytes e.g. 8,8192,8388608,83886080
* c. Name of the file to be read and written
*/

int main(int argc, char *argv[]) {

	if (argc != 4) {
		printf("usage: ./a.out <number of threads> <block size> <filename>");
		return EXIT_FAILURE;
	}

	// handling command line arguments
	numThreads = atoi(argv[1]);
	blockSize = atoi(argv[2]);
	fileName = malloc(strlen(argv[3]) + 1);
	strcpy(fileName, argv[3]);

	//initiliazing file
	FILE *fp = fopen(fileName, "w");
	fseek(fp, 1073741824, SEEK_SET);
	fputc('\0', fp);
	fclose(fp);

	//timing varibles
	clock_t start, end, ediff;
	double cpu_time_used;
	double throughPut, latency;

	//output log file
  	logFile = fopen("disk-benchmarking.log", "a");
  	fprintf(logFile, "\n\n***** Thread %d ******", numThreads);

  	if (logFile == NULL) {
    save_message("Error opening file", true);
  	}

	//declaring threads
	pthread_t threads[numThreads];
	int itr;

	/* Read and Write Sequentially*/
	//starting clock
	start = clock();
	for (itr = 0; itr < numThreads; itr++) {
		int *threadId = malloc(sizeof(*threadId));
		pthread_create(&threads[itr], NULL, &readWriteSeq, NULL);
	}

	for (itr = 0; itr < numThreads; itr++) {
		pthread_join(threads[itr], NULL);
	}
	//stopping clock
	end = clock();
	ediff = (double) (end - start);
	cpu_time_used = (double) ediff / CLOCKS_PER_SEC;
	long number_iter = ((long)FILE_SIZE / blockSize) / numThreads;
	throughPut = (double)(numThreads) * number_iter * blockSize / (cpu_time_used * MB);
	latency = (cpu_time_used * milli) / ((double)(numThreads) * (number_iter));

	fprintf(logFile, "\n****Read and Write Sequentially***\n");
	fprintf(logFile, "\nCPU Time Used: %lf secs", cpu_time_used);
	fprintf(logFile, "\nBlock Size: %ld bytes", blockSize);
	fprintf(logFile, "\nThroughput: %lf MB/secs", throughPut);
	fprintf(logFile, "\nLatency: %lf millisecs\n", latency);

 	/* Read and Write Randomly*/
  	//starting clock
	start = clock();
	for (itr = 0; itr < numThreads; itr++) {
		int *threadId = malloc(sizeof(*threadId));
		pthread_create(&threads[itr], NULL, &readWriteRand, NULL);
	}

	for (itr = 0; itr < numThreads; itr++) {
		pthread_join(threads[itr], NULL);
	}
	//stopping clock
	end = clock();
	ediff = (double) (end - start);
	cpu_time_used = (double) ediff / CLOCKS_PER_SEC;
	number_iter = ((long)FILE_SIZE / blockSize) / numThreads;
	throughPut = (double)(numThreads) * number_iter * blockSize / (cpu_time_used * MB);
	latency = (cpu_time_used * milli) / ((double)(numThreads) * (number_iter));

	fprintf(logFile, "\n****Read and Write Random***\n");
	fprintf(logFile, "\nCPU Time Used: %lf secs", cpu_time_used);
	fprintf(logFile, "\nBlock Size: %ld bytes", blockSize);
	fprintf(logFile, "\nThroughput: %lf MB/secs", throughPut);
	fprintf(logFile, "\nLatency: %lf millisecs\n", latency);


  	/* Write Sequentially*/
  	//starting clock
	start = clock();
	for (itr = 0; itr < numThreads; itr++) {
		int *threadId = malloc(sizeof(*threadId));
		pthread_create(&threads[itr], NULL, &writeSeq, NULL);
	}

	for (itr = 0; itr < numThreads; itr++) {
		pthread_join(threads[itr], NULL);
	}
	//stopping clock
	end = clock();
	ediff = (double) (end - start);
	cpu_time_used = (double) ediff / CLOCKS_PER_SEC;
	number_iter = ((long)FILE_SIZE / blockSize) / numThreads;
	throughPut = (double)(numThreads) * number_iter * blockSize / (cpu_time_used * MB);
	latency = (cpu_time_used * milli) / ((double)(numThreads) * (number_iter));

	fprintf(logFile, "\n****Write Sequentially ***\n");
	fprintf(logFile, "\nCPU Time Used: %lf secs", cpu_time_used);
	fprintf(logFile, "\nBlock Size: %ld bytes", blockSize);
	fprintf(logFile, "\nThroughput: %lf MB/secs", throughPut);
	fprintf(logFile, "\nLatency: %lf millisecs\n", latency);

	/* Write Randomly*/
  	//starting clock
	start = clock();
	for (itr = 0; itr < numThreads; itr++) {
		int *threadId = malloc(sizeof(*threadId));
		pthread_create(&threads[itr], NULL, &writeRand, NULL);
	}

	for (itr = 0; itr < numThreads; itr++) {
		pthread_join(threads[itr], NULL);
	}
	//stopping clock
	end = clock();
	ediff = (double) (end - start);
	cpu_time_used = (double) ediff / CLOCKS_PER_SEC;
	number_iter = ((long)FILE_SIZE / blockSize) / numThreads;
	throughPut = (double)(numThreads) * number_iter * blockSize / (cpu_time_used * MB);
	latency = (cpu_time_used * milli) / ((double)(numThreads) * (number_iter));

	fprintf(logFile, "\n****Write Random ***\n");
	fprintf(logFile, "\nCPU Time Used: %lf secs", cpu_time_used);
	fprintf(logFile, "\nBlock Size: %ld bytes", blockSize);
	fprintf(logFile, "\nThroughput: %lf MB/secs", throughPut);
	fprintf(logFile, "\nLatency: %lf millisecs\n", latency);

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