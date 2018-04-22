#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "myfunction.h"

#define MAX_BUF_SIZE 1024 // Maximum size of UDP messages
//#define SERVER_PORT 9876  // Server port
int main(int argc, char *argv[]) {

  int SERVER_PORT;
  char *SERVER_IP;

  if (argc == 3) {
    printf("the argoument supplied is IP %s\n", argv[1]); // IP
    SERVER_IP = argv[1];
    printf("the argoument supplied is PORT %s\n", argv[2]); // PORT
    SERVER_PORT = strtol(argv[2], NULL, 10);
  } else if (argc > 3) {
    printf("too many arguments \n");
    exit(0);
  } else {
    printf("two arguments expected \n");
    exit(0);
  }

  struct sockaddr_in
      server_addr; // struct containing server address information

  struct sockaddr_in
      client_addr; // struct containing client address information

  int sfd; // Server socket filed descriptor

  int br; // Bind result

  int i;

  ssize_t byteRecv; // Number of bytes received

  ssize_t byteSent; // Number of bytes to be sent
  socklen_t cli_size;

  char receivedData[MAX_BUF_SIZE]; // Data to be received
  char sendData[MAX_BUF_SIZE];     // Data to be sent

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

  for (;;) {

    byteRecv = recvfrom(sfd, receivedData, MAX_BUF_SIZE, 0,
                        (struct sockaddr *)&client_addr, &cli_size);

    if (byteRecv == -1) {

      perror("recvfrom");

      exit(EXIT_FAILURE);
    }

    printf("Received data: ");

    printData(receivedData, byteRecv);

    if (strncmp(receivedData, "exit", byteRecv) == 0) {

      printf("Command to stop server received\n");
      byteSent = sendto(sfd, "goodbye", 7, 0,
                        (struct sockaddr *)

                        &client_addr,
                        sizeof(client_addr));

      // break;
    } else {
      // convertToUpperCase(receivedData, byteRecv);
      printf("Response to be sent back to client: ");
      printData(receivedData, byteRecv);
      byteSent = sendto(sfd, receivedData, byteRecv, 0,
                        (struct sockaddr *)

                        &client_addr,
                        sizeof(client_addr));
    }

  } // End of for(;;) return 0;
}
