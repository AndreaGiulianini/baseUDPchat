#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <pthread.h>

//For inet_toa
#include <arpa/inet.h>
#include <netinet/in.h>

#include "myfunction.h"

#define MAX_BUF_SIZE 1024 // Maximum size of UDP messages

//Configuration variable for server
int SERVER_PORT;
char *SERVER_IP;

struct sockaddr_in server_addr; // struct containing server address information
struct sockaddr_in client_addr; // struct containing client address information
int sfd; // Server socket filed descriptor
int br; // Bind result
int i;

ssize_t byteRecv; // Number of bytes received

ssize_t byteSent; // Number of bytes to be sent
socklen_t cli_size;

size_t msgLen;

char receivedData[MAX_BUF_SIZE]; // Data to be received
char sendData[MAX_BUF_SIZE]; // Data to be sent



void *sendThr(void *arg){
        //Now start the cycle for send message
        while (true) {
                printf("Insert message:\n");

                // scanf("%s", sendData);
                fgets(sendData, MAX_BUF_SIZE, stdin);

                printf("String going to be sent to client: %s", sendData);

                msgLen = countStrLen(sendData);
                //the action for send message
                byteSent = sendto(sfd, sendData, msgLen, 0,
                                  (struct sockaddr *)

                                  &client_addr,
                                  sizeof(client_addr));
                if(byteSent==-1) {
                        printf("!!!Server can't send message to client if this hasn't sent something before!!!\n");
                }else{
                        printf("Bytes sent to client: %zd\n", byteSent);
                }

        } // End of while return 0;
        pthread_exit(NULL);
}

void *recThr(void *arg){
        //cycle for receive message, in this case we can see the gestion of message is sequencile
        for (;;) {
                //read message
                byteRecv = recvfrom(sfd, receivedData, MAX_BUF_SIZE, 0,
                                    (struct sockaddr *)&client_addr, &cli_size);
                //check if message is "normal" or a error
                if (byteRecv == -1) {
                        perror("recvfrom");
                        exit(EXIT_FAILURE);
                }

                printf("Received data Socket(%s:%d): \n", inet_ntoa(client_addr.sin_addr),
                       client_addr.sin_port);
                printData(receivedData, byteRecv);
                printf("Insert message:\n");

                //compare if data receive is equals to exit,
                //in that case we response with "goodbye" string
                //in all case we respond with the thame message of client
                if (strncmp(receivedData, "exit\n", byteRecv) == 0) {

                        byteSent = sendto(sfd, "goodbye", 7, 0,
                                          (struct sockaddr *)

                                          &client_addr,
                                          sizeof(client_addr));
//                } else {
//
//                        printf("Response to be sent back to client: ");
//                        printData(receivedData, byteRecv);
//                       byteSent = sendto(sfd, receivedData, byteRecv, 0,
//                                          (struct sockaddr *)
//
//                                          &client_addr,
//                                          sizeof(client_addr));
                }

        } // End of for(;;) return 0;
}

int main(int argc, char *argv[]) {
        //Check if parameters is what i expective and relative print
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

        sfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

        if (sfd < 0) {
                perror("socket"); // Print error message
                exit(EXIT_FAILURE);
        }

        //• Initialize server address information
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(SERVER_PORT); // Convert to network byte order
        server_addr.sin_addr.s_addr = INADDR_ANY; // Bind to any address

        br = bind(sfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
        if (br < 0) {
                perror("bind"); // Print error message
                exit(EXIT_FAILURE);
        }
        cli_size = sizeof(client_addr);

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
