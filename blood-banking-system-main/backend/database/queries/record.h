#ifndef RECORD_QUERIES_H
#define RECORD_QUERIES_H

#include "../connector.h"
#include "../../../shared/entities/entities.h"

struct Record* getUserHistory(int userId, struct Record notes[]) {
    struct Record records[20];

    MYSQL *connection = connectDatabase();
    char query[300];

    sprintf(query, "SELECT * FROM %s.record WHERE donor_id = %d OR taker_id = %d;", DATABASE_NAME, userId,userId);

    MYSQL_RES *res;
    MYSQL_ROW row;

    int i=0;

    mysql_query(connection,query);
    res = mysql_store_result(connection);

    while(row  = mysql_fetch_row(res)){
        records[i].id = atoi(row[0]);
        records[i].donor_id= atoi(row[1]);

        sprintf(records[i].bloodType, "%s", row[2]);
        sprintf(records[i].action, "%s", row[3]);
        sprintf(records[i].status, "%s", row[4]);
        sprintf(records[i].date, "%s", row[5]);

        records[i].taker_id = atoi(row[6]);
        i++;
    }

    i=0;
    mysql_free_result(res); // free result set

    for(int i=0;i<sizeof(records)/sizeof(struct Record);i++ ){
        notes[i]=records[i];
    }

    closeConnection(connection);
    return notes;
}

struct Record *makeRecordsStruct(MYSQL_RES *res) {
    struct Record *records = calloc(10, sizeof(struct Record));
    MYSQL_ROW row;
    int i = 0;

    while((row = mysql_fetch_row(res)) != NULL) {
        struct Record record;

        record.id = atoi(row[0]);
        record.donor_id= atoi(row[1]);
        
        sprintf(record.action, "%s", row[3]);
        sprintf(record.status, "%s", row[4]);
        sprintf(record.date, "%s", row[5]);

        if (row[2] == NULL) {
            sprintf(record.bloodType, "%s", "Undefined");
        } else {
            sprintf(record.bloodType, "%s", row[2]);
        }
        
        if (row[6] == NULL) {
            record.taker_id = 0;
        } else {
            record.taker_id = atoi(row[6]);
        }

        records[i++] = record;
    }

    return records;
}

struct Record* getBloodsToAnalyze() {
    struct Record *records;

     MYSQL *connection = connectDatabase(); 
    
    char query[300];
    sprintf(query, "SELECT * FROM %s.record WHERE status = \"pending\" AND taker_id IS NULL;", DATABASE_NAME);

    MYSQL_RES *res;
    MYSQL_ROW row;
    
    mysql_query(connection, query);
    
    res = mysql_store_result(connection);

    records = makeRecordsStruct(res);

    mysql_free_result(res); // free result set

    closeConnection(connection);

    return records;
}

void takeAnalysis(int userId) {
    MYSQL *connection = connectDatabase(); 
    char query[300];

    sprintf(query, "INSERT INTO %s.record (donor_id, _action, _status, date) VALUES(%d, \"analysis\", \"pending\", NOW());", DATABASE_NAME, userId);
    makeQuery(connection, query);

    closeConnection(connection);
}

void donateBloodByUser(int userId) {
    MYSQL *connection = connectDatabase();
    char query[300];

    sprintf(query, "INSERT INTO %s.record (donor_id, action, status, date) VALUES(%d, \"donor\", \"pending\", NOW());", DATABASE_NAME, userId);
    makeQuery(connection, query);

    closeConnection(connection);
}

void takeBlood(int recordId, int takerId) {
    MYSQL *connection = connectDatabase();
    char query[300];

    sprintf(query, "UPDATE %s.record SET taker_id = %d WHERE id = %d;", DATABASE_NAME, takerId, recordId);
    makeQuery(connection, query);

    closeConnection(connection);
}

void setAnalisysResultForRecord(int recordId, char *status, char *type) {
    MYSQL *connection = connectDatabase();
    char query[300];
    
    sprintf(query, "UPDATE %s.record SET blood_type = \"%s\", status = \"%s\" WHERE id = %d;", DATABASE_NAME, type, status, recordId);
    makeQuery(connection, query);

    closeConnection(connection);
}

struct Record getRecordById(int id) {
    struct Record record;

    MYSQL *connection = connectDatabase(); 

    char query[300];

    sprintf(query, "SELECT * FROM %s.record WHERE id = %d and LIMIT 1;", DATABASE_NAME, id);

    MYSQL_RES *res;
    MYSQL_ROW row;

    int i = 0;

    mysql_query(connection, query);
    res = mysql_store_result(connection);

    while(row  = mysql_fetch_row(res)){
        record.id = atoi(row[0]);

        record.donor_id= atoi(row[1]);
        
        sprintf(record.bloodType, "%s", row[2]);
        sprintf(record.action, "%s", row[3]);
        sprintf(record.status, "%s", row[4]);
        sprintf(record.date, "%s", row[5]);
        
        if (row[6] == NULL) {
            record.taker_id = 0; 
        } else {
            record.taker_id = atoi(row[6]);
        }

        i++;
    }


    i=0;
    mysql_free_result(res); // free result set

    closeConnection(connection);

    return record;
}

struct Record* getCanBeTakenBloodByType(char *type, struct Record notes[]) {
    struct Record records[20];

    MYSQL *connection = connectDatabase(); 

    char query[300];

    sprintf(query, "SELECT * FROM %s.record WHERE blood_type = \"%s\" and  taker_id IS NULL AND _action = \"donor\" AND _status = \"approved\" LIMIT 1;", DATABASE_NAME, type);

    MYSQL_RES *res;
    MYSQL_ROW row;

    int i=0;

    mysql_query(connection, query);
    res = mysql_store_result(connection);


    while(row  = mysql_fetch_row(res)){
        records[i].id = atoi(row[0]);
        records[i].donor_id= atoi(row[1]);
        
        sprintf(records[i].bloodType, "%s", row[2]);
        sprintf(records[i].action, "%s", row[3]);
        sprintf(records[i].status, "%s", row[4]);
        sprintf(records[i].date, "%s", row[5]);
        
        if (row[6] == NULL) { records[i].taker_id = 0; }
        else {
            records[i].taker_id = atoi(row[6]);
        }
        i++;
    }


    i=0;
    mysql_free_result(res); // free result set


    for(int i=0;i<sizeof(records)/sizeof(struct Record);i++ ){
        notes[i]=records[i];
    }

    closeConnection(connection);
    return notes;
}

struct Record* selectHistoryRecords(char *phone_number, struct Record array[]){
    MYSQL *connection = connectDatabase();
    MYSQL_RES *res;
    MYSQL_ROW row;
    char query[100];
    struct Record records[4096];
    int i=0;

    sprintf(query, "SELECT * FROM %s.history where phone_number = %s;", DATABASE_NAME, phone_number);

    mysql_query(connection, query);

    res = mysql_store_result(connection);

    for(int i=0; i < sizeof(records)/sizeof(struct Record); i++ ) {
        records[i].id = 0;
    }

    while(row = mysql_fetch_row(res)) {
        records[i].id = atoi(row[0]);

        records[i].id = atoi(row[0]);
        records[i].donor_id= atoi(row[1]);
        
        sprintf(records[i].bloodType, "%s", row[2]);
        sprintf(records[i].action, "%s", row[3]);
        sprintf(records[i].status, "%s", row[4]);
        sprintf(records[i].date, "%s", row[5]);
        
        if (row[6] == NULL) { records[i].taker_id = 0; }
        else {
            records[i].taker_id = atoi(row[6]);
        }
        
        i++;
    }

    i=0;
    mysql_free_result(res); // free result set
    closeConnection(connection); // close connection

    for(int i=0; i < sizeof(records)/sizeof(struct Record); i++) {
        array[i]=records[i];
    }

    return array;
}


#endif