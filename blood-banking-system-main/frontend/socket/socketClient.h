#ifndef SOCKET_CLIENT_H
#define SOCKET_CLIENT_H

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080

#include "socketInjector.h"

struct sockaddr_in serverAddress;

int initSocket() {
  if ((socketClientId = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("Error: Socket creation");

    return -1;
  }

  memset(&serverAddress, '0', sizeof(serverAddress));
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(SERVER_PORT);

  if ((inet_pton(AF_INET, SERVER_IP, &serverAddress.sin_addr)) < 0) {
    printf("Error: Invalid address");

    return -1;
  }

  if (connect(socketClientId, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
    printf("Error: Connection error");

    return -1;
  }

  return 0;
}

#endif