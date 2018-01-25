#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>

#define PORT 9090

#define  NUM_PACKETS 65536

#define PACKET_SIZE 64000

#define MB 1000000

FILE *logFile;
int numThreads;
char *ip;

void *client();

int main(int argc, char const *argv[])
{

    if (argc != 3) {
        printf("usage: ./a.out <number of threads> <ip address>\n");
        return EXIT_FAILURE;
    }

    numThreads = atoi(argv[1]);

    ip = (char *) malloc(strlen(argv[2]) * sizeof(char));
    memcpy(ip, argv[2], strlen(argv[2]));

    // printf("IP -> %s",ip);

    logFile = fopen("network-benchmarking-tcp.log", "a");


    pthread_t threadId[numThreads];
    int errInfo[numThreads];
    int itr = 0;
    clock_t time;
    double timeTaken;

    time = clock();

    for (itr = 0; itr < numThreads; itr++) {

        errInfo[itr] = pthread_create(&(threadId[itr]), NULL, &client, NULL );
        if (errInfo[itr] != 0)
            printf("Error creating thread %d\n", errInfo[itr]);

    }

    for (itr = 0; itr < numThreads; itr++) {
        if (pthread_join(threadId[itr], NULL ) != 0)
            printf("Error joining thread %d\n", itr);
    }


    time = clock() - time;
    timeTaken = (double) time / CLOCKS_PER_SEC;
    double throughput = (double)PACKET_SIZE * (NUM_PACKETS / timeTaken);


    fprintf(logFile, "\n**** Network Benchmarking ****\n");
    fprintf(logFile, "Internet Protocol: TCP\n");
    fprintf(logFile, "Packet size:\t 64KB\n");
    fprintf(logFile, "Number of packets transferred:\t %d\n", NUM_PACKETS);
    fprintf(logFile, "Number of threads:\t %d\n", numThreads);
    fprintf(logFile, "Latency:\t %.2lf ms\n", timeTaken * 1000);
    fprintf(logFile, "Throughput:\t %.2lf Mb/sec\n", throughput * 8 / MB);
    printf("Network benchmark completed\n");
    
    return 0;
}

void *client() {

    struct sockaddr_in serv_addr;

    int soc = 0;
    int packetsTobeSend = 0;
    int itr = 0;
    char buffer[PACKET_SIZE] = {0};



    if ((soc = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr(ip);


    if (connect(soc, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return;
    }

    packetsTobeSend = NUM_PACKETS / numThreads;

    for (itr = 0; itr < packetsTobeSend; itr++) {
        
        read(soc, buffer, PACKET_SIZE);

        send(soc, buffer, strlen(buffer), 0);
    }


}
