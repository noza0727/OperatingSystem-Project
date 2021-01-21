#ifndef ROUTER_H
#define ROUTER_H

#include <stdio.h>

#include "../../shared/routes.h"

#include "../../shared/entities/system/client.h"
#include "../../shared/entities/system/request.h"
#include "../../shared/entities/system/response.h"

#include "../controllers/auth.h"
#include "../controllers/user.h"
#include "../controllers/record.h"

/** Function Declarations */
int handleRequest(struct Client *client, struct Request *request);

int sendResponse(int sd, struct Response response);

int sendAll(int fd, void *buf, int n, int flags);

/** Function Implementations */
int handleRequest(struct Client *client, struct Request *request) {
  struct Response response;
  struct Route route = request->route;

  printf("\nModule: %s, Method: %s\n", route.module, route.method);

  if (strcmp(route.module, AUTH_MODULE) == 0) {
    if (strcmp(route.method, USER_LOGIN_METHOD) == 0) {
      return sendResponse(client->socket, userLogin(client, *request));
    }

    if (strcmp(route.method, USER_REGISTRATION_METHOD) == 0) {
      return sendResponse(client->socket, userRegistration(client, *request));
    }

    if (strcmp(route.method, ADMIN_LOGIN_METHOD) == 0) {
      return sendResponse(client->socket, adminLogin(client, *request));
    }
  }

  if (strcmp(route.module, USER_MODULE) == 0) {
    if (strcmp(route.method, USER_INFO_METHOD) == 0) {
      return sendResponse(client->socket, userInfo(client, *request));
    }
  }

  if (strcmp(route.module, RECORD_MODULE) == 0) {
    if (strcmp(route.method, DONATE_BLOOD_METHOD) == 0) {
      return sendResponse(client->socket, donateBlood(client, *request));
    }

    if (strcmp(route.method, GET_PENDING_DONATES_METHOD) == 0) {
      return sendResponse(client->socket, getPendingRecords(client, *request));
    }

    if (strcmp(route.method, SET_ANALYSIS_RESULT_METHOD) == 0) {
      return sendResponse(client->socket, setAnalysisResult(client, *request));
    }
  }

  return EXIT_SUCCESS;
}

int sendResponse(int socket, struct Response response) {
  int size = sizeof(response);

  if (sendAll(socket, &response, size, 0)) {
    return EXIT_SUCCESS;
  }

  fprintf(stderr, "Failed to send response");
  return EXIT_FAILURE;
}

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

#endif