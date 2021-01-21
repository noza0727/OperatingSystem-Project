#ifndef RESPONSE_ENTITY_H
#define RESPONSE_ENTITY_H

#include "responseData.h"

struct Response {
  int status;
  char message[1024];
  struct ResponseData data;
};

#endif