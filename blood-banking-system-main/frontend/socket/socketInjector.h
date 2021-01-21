#ifndef SOCKET_DEPENDENCIES_H
#define SOCKET_DEPENDENCIES_H

#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>

static int socketClientId;

int sendAll(int fd, void *buf, int n, int flags) {
  int temp, total = 0;

  while (total < n) {
    temp = send(fd, buf + total, n - total, flags);

    if (temp == -1) {
      break;
    }

    total += temp;
  }

  return temp == -1 ? -1 : total;
}

void closeSocketConnection() {
  close(socketClientId);
}

#endif