#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>

#define PORT 9090

#define NUM_PACKETS 65536

#define PACKET_SIZE 64000

int numThreads;
void *connectionHandler(void *thread_data);
struct sockaddr_in si_other;

int main(int argc, char const *argv[])
{

    if (argc != 2) {
        printf("usage: ./a.out <number of threads>\n");
        return EXIT_FAILURE;
    }

    numThreads = atoi(argv[1]);

    struct sockaddr_in si_me;
    int soc, i, itr;
    char* buffer = malloc(PACKET_SIZE);
    ssize_t byte_received = 0;

    if ((soc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
        printf("socket");

    memset((char *) &si_me, 0, sizeof(si_me));
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(10009);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY );
    if (bind(soc, (struct sockaddr *)&si_me, sizeof(si_me)) == -1)
        printf("bind");

    printf("Server listening on port 9090");

    pthread_t threadId[numThreads];

    for (itr = 0; itr < numThreads; itr++) {

        int *newsock = malloc(sizeof(int));

        *newsock = soc;
        pthread_create(&(threadId[itr]), NULL, &connectionHandler, newsock);

    }

    for (itr = 0; itr < numThreads; itr++) {
        if (pthread_join(threadId[itr], NULL ) != 0)
            printf("Error joining thread %d\n", itr);
    }

    return 0;
}

void *connectionHandler(void *thread_data) {


    int soc = *(int *) thread_data;
    //printf("Client request recv %d\n",soc);
    socklen_t slen = sizeof(si_other);


    char *buffer = (char *) malloc(PACKET_SIZE * sizeof(char));
    int i = 0, itr = 0, byte_received;

    memset(buffer, 'D', PACKET_SIZE);

    itr = (int) NUM_PACKETS / numThreads;

    for (i = 0; i < itr; i++) {
        //printf("waiting for %d\n",i );
        byte_received = recvfrom(soc, buffer, PACKET_SIZE, 0, (struct sockaddr *)&si_other, &slen);
        if (byte_received < PACKET_SIZE)
            recvfrom(soc, buffer, PACKET_SIZE, 0, (struct sockaddr *)&si_other, &slen);
        if (byte_received == -1)
            printf("Error in sendto\n");
        //byte_received = sendto(soc, buffer, PACKET_SIZE, 0, (struct sockaddr *)&si_other, slen);
        if (byte_received < PACKET_SIZE)
            sendto(soc, buffer, PACKET_SIZE, 0, (struct sockaddr *)&si_other, slen);
        if (byte_received == -1)
            printf("Error in sendto\n");

    }

}
