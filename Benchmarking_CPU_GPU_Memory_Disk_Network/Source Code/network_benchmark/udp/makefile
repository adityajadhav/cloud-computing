FILE_NAME_SERVER = server-udp
FILE_NAME_CLIENT = client-udp

threads = 1
ipaddr = 127.0.0.1

CC=gcc 
CFLAGS=-Wall

all : benchmarking

benchmarking :
	gcc $(FILE_NAME_SERVER).c -lpthread -o $(FILE_NAME_SERVER).o
	gcc $(FILE_NAME_CLIENT).c -lpthread -o $(FILE_NAME_CLIENT).o

clean :
	rm -r *.o

run_server :
	./$(FILE_NAME_SERVER).o $(threads)

run_client :
	./$(FILE_NAME_CLIENT).o $(threads) $(ipaddr)
