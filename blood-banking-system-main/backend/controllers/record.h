#ifndef RECORD_CONTROLLERS_H
#define RECORD_CONTROLLERS_H

#include "../../shared/entities/entities.h"
#include "../database/queries/record.h"

// Declarations
struct Response donateBlood(struct Client *client, struct Request request);

struct Response getPendingRecords(struct Client *client, struct Request request);

struct Response setAnalysisResult(struct Client *client, struct Request request);

// Implementations
struct Response donateBlood(struct Client *client, struct Request request) {
  struct Response response;

  donateBloodByUser(client->user.id);

  response.status = 1;

  return response;
}

struct Response getPendingRecords(struct Client *client, struct Request request) {
  struct Response response;

  struct Record *records = getBloodsToAnalyze();

  response.status = 1;

  for (int i = 0; i < 10; i++) {
    response.data.records[i] = records[i];
  }
  
  return response;
}

struct Response setAnalysisResult(struct Client *client, struct Request request) {
  struct Response response;

  printf("type: %s\n", request.bloodType);

  setAnalisysResultForRecord(request.id, request.status, request.bloodType);
  
  response.status = 1;

  return response;
}


#endif