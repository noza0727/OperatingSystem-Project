#include "bootstrap_injection.h"
#include "../clients_injection.h"
#include "../../../shared/entities/system/request.h"
#include "../../router/router.h"

int startServer();

int initMasterSocket();

int initAddress();

int handleConnections();

int handleNewConnection();

fd_set readFds;

struct sockaddr_in address;

int masterSocket;
int addressLength;
int activity;

int startServer() {
  if (initMasterSocket()) {
    fprintf(stderr, "Master socket init error");
    return EXIT_FAILURE;
  }

  if (initAddress()) {
    fprintf(stderr, "Address init error");
    return EXIT_FAILURE;
  }

  printf("Waiting for connections\n");

  if (handleConnections()) {
    fprintf(stderr, "Connection error");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int initMasterSocket() {
  int opt = 1;

  for (int i = 0; i < MAX_ALLOWED_CLIENTS; i++) {
    clients[i].socket = 0;
  }

  // Creating master socket
  masterSocket = socket(AF_INET, SOCK_STREAM, 0);

  if (masterSocket == 0) {
    perror("Error: creating master socket");
    return EXIT_FAILURE;
  }

  // Allow multiple connections logic
  if (setsockopt(masterSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0) {
    perror("Error: setsockopt");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int initAddress() {
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  addressLength = sizeof(address);

  if (bind(masterSocket, (struct sockaddr *)&address, addressLength) < 0) {
    perror("Error: binding socket to port");
    return EXIT_FAILURE;
  }

  if (listen(masterSocket, 3) < 0) {
    perror("Error: set listener");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int handleConnections() {
  int maxSd, curSd, newSocket;
  struct Request request;

  while (1) {
    /** Clear the controller set */
    FD_ZERO(&readFds);
    /** Add Master Controller to set */
    FD_SET(masterSocket, &readFds);
    maxSd = masterSocket;
    /** Add child sockets to set */
    for (int i = 0; i < MAX_ALLOWED_CLIENTS; i++) {
      /** Controller Descriptor */
      curSd = clients[i].socket;
      /** Add to read list if valid */
      if (curSd > 0)
        FD_SET(curSd, &readFds);

      /** Highest file descriptor number, is needed for the select function */
      if (curSd > maxSd)
        maxSd = curSd;
    }

    /** Wait for an activity on one of the sockets, timeout is NULL, so wait indefinitely */
    activity = select(maxSd + 1, &readFds, NULL, NULL, NULL);
    if ((activity < 0) && (errno != EINTR))
      fprintf(stderr, "Select error");

    /** Handle request to master controller (incoming connection) */
    if (handleNewConnection()) {
      fprintf(stderr, "New connection error");
      return EXIT_FAILURE;
    }

    /** Handle request from non-master controller (request to other controller)*/
    for (int i = 0; i < MAX_ALLOWED_CLIENTS; i++) {
      curSd = clients[i].socket;
      if (FD_ISSET(curSd, &readFds)) {
        /** Read incoming request and check if is closing one */
        if (recv(curSd, &request, sizeof(struct Request), MSG_WAITALL) == 0) {
          getpeername(curSd, (struct sockaddr *)&address, (socklen_t *)&addressLength);

          printf("Host disconnected , ip %s , port %d \n", inet_ntoa(address.sin_addr),
          ntohs(address.sin_port));

          closeSession(&clients[i]);
        } else {
          /** Handle incoming request */
          if (handleRequest(&clients[i], &request))
            fprintf(stderr, "Failed to handle request");
        }
      }
    }
  }
}

int handleNewConnection() {
  int newSocket;

  if (FD_ISSET(masterSocket, &readFds)) {
    if ((newSocket = accept(masterSocket, (struct sockaddr *)&address, (socklen_t *)&addressLength)) < 0) {
      perror("accept");
      return EXIT_FAILURE;
    }

    printf("New connection , controller fd is %d , ip is : %s , port : %d \n", newSocket,
           inet_ntoa(address.sin_addr), ntohs(address.sin_port));

    /** Add new controller to an array of sockets */
    for (int i = 0; i < MAX_ALLOWED_CLIENTS; i++)
      if (clients[i].socket == 0) {
        clients[i].socket = newSocket;
        printf("Adding to list of sockets as %d\n", i);
        break;
      }
  }

  return EXIT_SUCCESS;
}