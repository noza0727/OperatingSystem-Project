#ifndef CLIENTS_INJECTION_H
#define CLIENTS_INJECTION_H

#include "../../shared/entities/system/client.h"

#define MAX_ALLOWED_CLIENTS 10

static struct Client clients[MAX_ALLOWED_CLIENTS];

struct Client getClientByPhone(char *phone);

struct Client getClientByUsername(char *username);

struct Client getClientByUsername(char *username) {
  for (int i = 0; i < MAX_ALLOWED_CLIENTS; i++) {
    if (clients[i].type == 0 && strcmp(clients[i].admin.username, username) == 0) {
      return clients[i];
    } else if (clients[i].type == 1 && strcmp(clients[i].specialist.username, username) == 0) {
      return clients[i];
    }
  }
}

struct Client getClientByPhone(char *phone) {
  for (int i = 0; i < MAX_ALLOWED_CLIENTS; i++) {
    if (strcmp(clients[i].user.phoneNumber, phone) == 0) {
      return clients[i];
    }
  }
}

int startUserSession(struct Client *client, struct User user) {
  client->user = user;
  return EXIT_SUCCESS;
}

int startAdminSession(struct Client *client, struct Admin admin) {
  client->admin = admin;
  return EXIT_SUCCESS;
}

int startSpecialistSession(struct Client *client, struct Specialist specialist) {
  client->specialist = specialist;
  return EXIT_SUCCESS;
}

int closeSession(struct Client *client) {
  struct User user;
  struct Admin admin;
  struct Specialist specialist;

  // 0: Admin, 1: Specialist, 2: User
  if (client->type == 0) {
    client->admin = admin;
  } else if (client->type == 1) {
    client->specialist = specialist;
  } else if (client->type == 2) {
    client->user = user;
  }

  close(client->socket);
  client->socket = 0;

  return EXIT_SUCCESS;
}

#endif