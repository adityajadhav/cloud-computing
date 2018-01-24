CC=gcc 
CFLAGS=-Wall
FILE_NAME = stream

all : benchmarking

benchmarking :
	gcc $(FILE_NAME).c -fopenmp -o $(FILE_NAME).o

clean :
	rm -r *.o

run :
	./$(FILE_NAME).o 
