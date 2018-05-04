#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <pthread.h>

#include "myfunction.h"

#define MAX_BUF_SIZE 1024 // Maximum size of UDP messages

//Variable for connection to server, ip and port
int SERVER_PORT;
char *SERVER_IP;
//Now start configuration parameters for client
struct sockaddr_in
        server_addr; // Struct containing server address information

struct sockaddr_in
        client_addr; // Struct containing client address information

int sfd; // Server socket filed descriptor

int br; // Bind result

int i;

ssize_t byteRecv; // Number of bytes received

ssize_t byteSent; // Number of bytes to be sent

size_t msgLen;

socklen_t serv_size;

char receivedData[MAX_BUF_SIZE]; // Data to be received
char sendData[MAX_BUF_SIZE]; // Data to be sent



void *sendThr(void *arg){
        //now start the cycle for send message
        for(;;) {
                printf("Insert message:\n");

                // scanf("%s", sendData);
                fgets(sendData, MAX_BUF_SIZE, stdin);

                printf("String going to be sent to server: %s", sendData);

                msgLen = countStrLen(sendData);
                //the action for send message
                byteSent = sendto(sfd, sendData, msgLen, 0,
                                  (struct sockaddr *)

                                  &server_addr,
                                  sizeof(server_addr));

                printf("Bytes sent to server: %zd\n", byteSent);
        } // end of while return 0;
        pthread_exit(NULL);
}

void *recThr(void *arg){
        //now start the cycle for send message
        for(;;) {
                //read relative response from server
                byteRecv = recvfrom(sfd, receivedData, MAX_BUF_SIZE, 0,
                                    (struct sockaddr *)&server_addr, &serv_size);

                printf("Received from server: ");

                printData(receivedData, byteRecv);
                //check if response is equals to goodbye, if this is true close client connection
                if (strcmp(receivedData, "goodbye") == 0) {
                        exit(0);
                }
                printf("\nInsert message:\n");

        }
        pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
        //check if parameters is what i expective and relative print
        if (argc == 3) {
                printf("The IP address supplied is %s\n", argv[1]); // IP
                SERVER_IP = argv[1];
                printf("The Port number supplied is %s\n", argv[2]); // PORTA
                SERVER_PORT = strtol(argv[2], NULL, 10);
        } else if (argc > 3) {
                printf("Too many arguments supplied.\n");
                exit(0);
        } else {
                printf("Two argument expected [IP]&[PORT].\n");
                exit(0);
        }

        //open socket
        sfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

        if (sfd < 0) {
                perror("socket"); // Print error message
                exit(EXIT_FAILURE);
        }

        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(SERVER_PORT);
        server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

        serv_size = sizeof(server_addr);

        pthread_t th;
        int rc, i=1, *intptr;

        //Sender
        intptr=malloc(sizeof(int));
        if( !intptr ) { printf("malloc failed\n"); exit(1); }
        *intptr=i;
        rc=pthread_create( &th,NULL,sendThr,(void*)intptr);
        i++;

        //Receiver
        intptr=malloc(sizeof(int));
        if( !intptr ) { printf("malloc failed\n"); exit(1); }
        *intptr=i;
        rc=pthread_create( &th,NULL,recThr,(void*)intptr);

        pthread_exit(NULL);

        return 0;
}
