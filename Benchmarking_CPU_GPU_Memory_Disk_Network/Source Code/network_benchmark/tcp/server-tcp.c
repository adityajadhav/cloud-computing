#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>

#define PORT 9090

#define NUM_PACKETS 100000

#define PACKET_SIZE 64000

int numThreads;

void *connectionHandler(void *thread_data);

int main(int argc, char const *argv[])
{

    if (argc != 2) {
        printf("usage: ./a.out <number of threads>\n");
        return EXIT_FAILURE;
    }

    numThreads = atoi(argv[1]);


    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);


    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        printf("Socket creation error\n");
        return -1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        printf("Bind error\n");
        return -1;
    }
    if (listen(server_fd, 8) < 0)
    {
        printf("Listen error\n");
        return -1;
    }

    printf("Server listening on port 9090\n");

    while (1) {

        int newsocket_fd  = 0;

        newsocket_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);

        if (newsocket_fd < 0) {
            printf("\nConnection Failed \n");
            return -1;
        }

        pthread_t thread;
        int *newsock = malloc(sizeof(int));

        *newsock = newsocket_fd;

        pthread_create(&thread, NULL, &connectionHandler, newsock);
        pthread_detach(thread);

    }
    return 0;
}

void *connectionHandler(void *thread_data) {

    char *buffer = (char *) malloc(PACKET_SIZE * sizeof(char));
    memset(buffer, 'D', PACKET_SIZE);
    int i = 0;
    int itr = 0;

    int fd = *(int *) thread_data;
    // srand((unsigned) time(&t));
    // sleep(rand()% 20);

    itr = (int) NUM_PACKETS / numThreads;

    for (i = 0; i < itr; i++) {
        send(fd , buffer , PACKET_SIZE , 0 );
        read( fd , buffer, PACKET_SIZE);
        //printf("buffer %d message sent\n",i);
    }
    free(thread_data);

}
