#ifndef CLIENT_ENTITY_H
#define CLIENT_ENTITY_H

#include "../admin.h"
#include "../specialist.h"
#include "../user.h"

struct Client {
  int socket;
  int type; // 0: Admin, 1: Specialist, 2: User

  union {
    struct Admin admin;
    struct Specialist specialist;
    struct User user;
  };
};

#endif
