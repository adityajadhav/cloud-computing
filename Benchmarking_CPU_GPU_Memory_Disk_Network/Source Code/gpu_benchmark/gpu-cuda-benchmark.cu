#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define NUM_ITERATIONS 100000000000
#define NUM_THREADS 3072


__global__ void solve_equation_float(float * d_out, float * d_in) {

	int x = threadIdx.x;
	long i;
	for (i = 0; i < NUM_ITERATIONS; i++) {
		d_out[x] =  5.66 + 7.888 + 4.67 + 8.99
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
}

__global__ void solve_equation_int(int * d_out, int * d_in) {

	int x = threadIdx.x;
	long i;

	for (i = 0; i < NUM_ITERATIONS; i++) {
		d_out[x]  = 566 + 7888 + 467 + 899
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
}


void calculateGIOPS() {

	clock_t start;
	FILE *logFile;
	double timeTaken;

	logFile = fopen("gpu-benchmarking.log", "a");

	const int ARRAY_SIZE = NUM_THREADS * sizeof(int);

	int h_in[NUM_THREADS];

	for (int i = 0; i < NUM_THREADS; i++) {
		h_in[i] = i;
	}

	int h_out[NUM_THREADS];

	int * d_in;
	int * d_out;


	start = clock();


	cudaMalloc((void**) &d_in, ARRAY_SIZE);
	cudaMalloc((void**) &d_out, ARRAY_SIZE);

	cudaMemcpy(d_in, h_in, ARRAY_SIZE, cudaMemcpyHostToDevice);


	// launch the kernel
	solve_equation_int <<< 1, NUM_THREADS>>>(d_out, d_in);

	cudaDeviceSynchronize();

	start = clock() - start;


	cudaMemcpy(h_out, d_out, ARRAY_SIZE, cudaMemcpyDeviceToHost);

	cudaFree(d_in);
	cudaFree(d_out);

	timeTaken = (double) start / CLOCKS_PER_SEC;


	printf("time taken -> %lf s\n", timeTaken);
	double flops, gflops;

	flops = NUM_ITERATIONS * (40 / timeTaken);
	gflops = flops / 1000000000;

	fprintf(logFile, "\n\n\nGIOPS Result");
	fprintf(logFile, "\nThreads: %d", NUM_THREADS);
	fprintf(logFile, "\nCPU Time Used: %lf ms", timeTaken);
	fprintf(logFile, "\nNumber of IOPS: %lf", flops);
	fprintf(logFile, "\nNumber of GIOPS: %lf", gflops);


	printf("Please check output file -> gpu-benchmarking.log\n");
	printf("benchmark success\n");


}

void calculateGFLOPS() {

	clock_t start;
	FILE *logFile;
	double timeTaken = 0;

	logFile = fopen("gpu-benchmarking.log", "a");

	const int ARRAY_SIZE = NUM_THREADS * sizeof(float);

	float h_in[NUM_THREADS];

	for (int i = 0; i < NUM_THREADS; i++) {
		h_in[i] = float(i);
	}

	float h_out[NUM_THREADS];

	float * d_in;
	float * d_out;

	start = clock();

	cudaMalloc((void**) &d_in, ARRAY_SIZE);
	cudaMalloc((void**) &d_out, ARRAY_SIZE);

	cudaMemcpy(d_in, h_in, ARRAY_SIZE, cudaMemcpyHostToDevice);

	// launch the kernel
	solve_equation_float <<< 1, NUM_THREADS>>>(d_out, d_in);

	cudaDeviceSynchronize();

	start = clock() - start;


	cudaMemcpy(h_out, d_out, ARRAY_SIZE, cudaMemcpyDeviceToHost);

	cudaFree(d_in);
	cudaFree(d_out);

	timeTaken = (double) start / CLOCKS_PER_SEC;

	printf("time taken -> %lf s\n", timeTaken);
	double flops, gflops;

	flops = NUM_ITERATIONS * (40 / timeTaken);
	gflops = flops / 1000000000;

	fprintf(logFile, "\n\n\nGFLOPS Result");
	fprintf(logFile, "\nThreads: %d", NUM_THREADS);
	fprintf(logFile, "\nCPU Time Used: %lf ms", timeTaken);
	fprintf(logFile, "\nNumber of FLOPS: %lf", flops);
	fprintf(logFile, "\nNumber of GFLOPS: %lf", gflops);

	fclose(logFile);
	printf("Please check output file -> gpu-benchmarking.log\n");
	printf("benchmark success\n");



}






int main(int argc, char ** argv) {

	int mode = 0;
	if (argc != 2) {
		printf("usage: ./a.out <mode>\n");
		printf("1 -> Calculate GIOPS\n");
		printf("2 -> Calculate GFLOPS\n");
		return EXIT_FAILURE;
	}

	mode = atoi(argv[1]);

	if (mode == 1)
		calculateGIOPS();
	else if (mode == 2)
		calculateGFLOPS();
	else
		printf("Unknown mode");

	return 0;
}
