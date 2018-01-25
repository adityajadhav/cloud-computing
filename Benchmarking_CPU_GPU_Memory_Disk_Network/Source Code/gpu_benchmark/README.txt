Compile: 
	nvcc gpu-cuda-benchmark.cu -o gpu-cuda-benchmark

Run:	
	./gpu-cuda-benchmark <Mode>

	Mode 1 -> to calculate GIOPS
	Mode 2 -> to calculate GFLOPS

Output file name: gpu-benchmarking.log
	