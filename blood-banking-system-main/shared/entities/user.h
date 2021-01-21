#ifndef USER_ENTITY_H
#define USER_ENTITY_H

#include "record.h"

struct User {
  int id;
  char phoneNumber[512];
  char password[512];
  char name[512];
  char date[512];
  char bloodType[512];
  char email[512];
  char region[512];
  union {
    struct Record lastDonation;
  };
};

#endif


