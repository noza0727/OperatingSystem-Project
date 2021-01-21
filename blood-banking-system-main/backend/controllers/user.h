#ifndef USER_CONTROLLERS_H
#define USER_CONTROLLERS_H

#include "../../shared/entities/entities.h"
#include "../database/queries/user.h"

// Declarations
struct Response userInfo(struct Client *client, struct Request request);

// Implementations
struct Response userInfo(struct Client *client, struct Request request) {
  struct Response response;

  struct User user = selectUserByPhoneNumber(client->user.phoneNumber);

  response.status = 1;
  response.data.user = user;

  return response;
}

#endif
