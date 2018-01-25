#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>

#define PORT 9090

#define  NUM_PACKETS 65536

// Fixed packet size to 64 kilobytes
#define PACKET_SIZE 64000

#define MB 1000000

FILE *logFile;
int numThreads;
char *serverIp;

void *client();

int main(int argc, char const *argv[])
{

    if (argc != 3) {
        printf("usage: ./a.out <number of threads> <ip address>\n");
        return EXIT_FAILURE;
    }

    numThreads = atoi(argv[1]);

    printf("Number of threads -> %d\n", numThreads);


    serverIp = (char *) malloc(strlen(argv[2]) * sizeof(char));
    memcpy(serverIp, argv[2], strlen(argv[2]));

    // printf("IP -> %s",ip);

    logFile = fopen("network-benchmarking-udp.log", "a");


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
    fprintf(logFile, "Internet Protocol: UDP\n");
    fprintf(logFile, "Packet size:\t 64KB\n");
    fprintf(logFile, "Number of packets transferred:\t %d\n", NUM_PACKETS);
    fprintf(logFile, "Number of threads:\t %d\n", numThreads);
    fprintf(logFile, "Latency:\t %.2lf ms\n", timeTaken * 1000);
    fprintf(logFile, "Throughput:\t %.2lf Mb/sec\n", throughput * 8 / MB);

    printf("Network benchmark completed\n");
    return 0;
}

void *client() {

    struct sockaddr_in si_other;
    int soc, slen = sizeof(si_other);
    int i, itr = 0;
    int packetsTobeSend = 0;
    char* buffer = malloc(PACKET_SIZE);
    memset(buffer, 'a', PACKET_SIZE);

    if ((soc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
        printf("Unable to create socket");

    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(10009);

    if (inet_aton(serverIp, &si_other.sin_addr) == 0) {
        fprintf(stderr, "Failed to connect: %s\n", serverIp);
    }

    itr = (int) NUM_PACKETS / numThreads;
    printf("Num Iteration per threads -> %d\n", itr);

    //int byte_received = sendto(soc, buffer, PACKET_SIZE, 0, (struct sockaddr *)&si_other, slen);

    for (i = 0; i < itr; i++) {

        int byte_received = sendto(soc, buffer, PACKET_SIZE, 0, (struct sockaddr *)&si_other, slen);
        //printf("%d\n",i);
        if (byte_received < PACKET_SIZE)
            sendto(soc, buffer, PACKET_SIZE, 0, (struct sockaddr *)&si_other, slen);
        if (byte_received == -1)
            printf("Error in sendto\n");
        //byte_received = recvfrom(soc, buffer, PACKET_SIZE, 0, (struct sockaddr *)&si_other, &slen);
        if (byte_received < PACKET_SIZE)
            recvfrom(soc, buffer, PACKET_SIZE, 0, (struct sockaddr *)&si_other, &slen);
        if (byte_received == -1)
            printf("Error in sendto\n");

    }

}
