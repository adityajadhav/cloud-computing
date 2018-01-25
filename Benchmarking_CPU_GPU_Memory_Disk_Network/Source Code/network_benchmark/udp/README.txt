To Compile:
	make

To Clean:
	make clean

To Run Sever:
	./server-udp.o <number of threads>
	e.g ./server-udp.o 2
	
To Run Client:
	./client-udp.o <number of threads> <ip address>
	e.g ./client-udp.o 2 127.0.0.1

Output file name: network-benchmarking-udp.log

Note:
1: Number of theads on both client and server should be same
2: To run the program for 2 nodes just give the server ip address while running the client	