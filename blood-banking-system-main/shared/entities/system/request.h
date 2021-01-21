#ifndef REQUEST_ENTITY_H
#define REQUEST_ENTITY_H

#include "route.h"

struct Request {
  struct Route route;

  char username[512];

  char phoneNumber[512];
  char bloodType[512];
  char password[512];
  char region[512];
  char email[512];
  char name[512];
  char date[512];

  int id;
  char status[512];
};

#endif