#ifndef RESPONSE_DATA_ENTITY_H
#define RESPONSE_DATA_ENTITY_H

#include "../user.h"
#include "../admin.h"
#include "../record.h"
#include "../specialist.h"

struct ResponseData {
  union {
    struct User user;
    struct Admin admin;
    struct Record record;
    struct Record records[10];
    struct Specialist specialist;
  };
};

#endif