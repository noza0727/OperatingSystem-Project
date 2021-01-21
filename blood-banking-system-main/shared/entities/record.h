#ifndef RECORD_ENTITY_H
#define RECORD_ENTITY_H

struct Record {
  int id;
  int donor_id;
  int taker_id;

  char bloodType[512];
  char date[512];
  char action[512]; // donor or recepient 
  char status[512]; // approved or not
};

#endif
