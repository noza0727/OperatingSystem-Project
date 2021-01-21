#ifndef SPECIALIST_QUERIES_H
#define SPECIALIST_QUERIES_H

#include "../connector.h"
#include "../../../shared/entities/specialist.h"

void insertNewSpecialist(int id, char *username, char *name, char *password) {
    MYSQL *connection = connectDatabase();

    char query[100];
    sprintf(query, "INSERT INTO %s.specialist (id, username, name, password) VALUES(%d,\"%s\",\"%s\", \"%s\");", DATABASE_NAME, id, username, name, password);
    printf("%s\n", query);

    makeQuery(connection, query);
    printf("Insertion was successful\n");

    closeConnection(connection);
}

struct Specialist selectSpecialistByUsername(char *username) {
    struct Specialist specialist;

    MYSQL *connection = connectDatabase();

    char query[100];
    sprintf(query, "SELECT * FROM %s.specialist where username = \"%s\";", DATABASE_NAME, username);

    MYSQL_RES *res;
    MYSQL_ROW row;

    mysql_query(connection, query);

    res = mysql_store_result(connection);
    while (row = mysql_fetch_row(res)) {
        specialist.id = atoi(row[0]);

        sprintf(specialist.name, "%s", row[1]);
        sprintf(specialist.username, "%s", row[2]);
        sprintf(specialist.password, "%s", row[3]);
    }

    mysql_free_result(res);
    closeConnection(connection);

    return specialist;
}

void deleteSpecialistByName(char *username) {
    MYSQL *connection = connectDatabase();
    char query[100];
    
    sprintf(query, "DELETE from %s.specialist where username = \"%s\";", DATABASE_NAME, username);
    // prepare prepared statement for transmisson and execution
    makeQuery(connection, query);

    printf("Deletion was successful\n");
    closeConnection(connection);
}

void updateSpecialistPassword(char *username, char *password) {
    MYSQL *connection = connectDatabase();

    char query[100];
    sprintf(query, "UPDATE %s.specialist SET password = \"%s\" where username = \"%s\";", DATABASE_NAME, password, username);

    makeQuery(connection, query);

    printf("Update was successful\n");
    closeConnection(connection);
}

int validSpecialist(char *username, char *password) {
    MYSQL *connection = connectDatabase();

    //variable definnition
    MYSQL_RES *res;
    MYSQL_ROW row;

    char query1[100];
    char *DBname;
    char *DBpassword; // in future will be used for password

    int nameStatus = 0;
    int passwordStatus = 0;
    int totalStatus = 0; // return 1 if true, 0 if false

    //#####

    //Extracting name email(password) and validate with input
    sprintf(query1, "SELECT username,password FROM %s.specialist", DATABASE_NAME);

    mysql_query(connection, query1);

    res = mysql_store_result(connection);

    while (row = mysql_fetch_row(res)) {
        DBname = row[0];
        DBpassword = row[1];

        if (isEqual(DBname, username)) {
            nameStatus = 1;
            if (isEqual(DBpassword, password)) {
                passwordStatus = 1;
                break;
            } else {
                passwordStatus = 0;
            }
        } else {
            nameStatus = 0;
        }
    }

    mysql_free_result(res);

    if (nameStatus == 0 || passwordStatus == 0) {
        totalStatus = 0;
    } else {
        if (nameStatus == 1 && passwordStatus == 1) {
            totalStatus = 1;
        }
    }
    //#####
    closeConnection(connection);
    return totalStatus;
}

#endif