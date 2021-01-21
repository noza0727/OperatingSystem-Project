#ifndef ADMIN_QUERIES_H
#define ADMIN_QUERIES_H

#include "../connector.h"
#include "../../../shared/entities/admin.h"

void insertNewAdmin(int id, char *username, char *name, char *password){
    MYSQL *connection = connectDatabase();
    char query[100];
    
    sprintf(query, "INSERT INTO %s.admin (id,username,name,password) VALUES(%d,\"%s\",\"%s\", \"%s\");",DATABASE_NAME,id,username,name,password);
    
    makeQuery(connection, query);
    printf("Insertion was successful\n");

    closeConnection(connection);
}

struct Admin selectAdminByName(char *username){
    MYSQL *connection = connectDatabase();

    struct Admin admin;

    MYSQL_RES *res;
    MYSQL_ROW row;
    char query[100];

    sprintf(query, "SELECT * FROM %s.admin where username = \"%s\";", DATABASE_NAME, username);

    mysql_query(connection, query);

    res = mysql_store_result(connection);
    while(row  = mysql_fetch_row(res)){
        admin.id = atoi(row[0]);
        
        sprintf(admin.username, "%s", row[1]);
        sprintf(admin.password, "%s", row[2]);
    }

    mysql_free_result(res);
    closeConnection(connection);

    return admin;
}

void deleteAdminByName(char *username){
    MYSQL *connection = connectDatabase();

    char query[100];

    sprintf(query, "DELETE from %s.admin where username = \"%s\";", DATABASE_NAME ,username); 
    makeQuery(connection, query);

    printf("Deletion was successful\n");
     closeConnection(connection);
}

void updateAdminPassword(char *username, char *password){
    MYSQL *connection = connectDatabase();
    char query[100];
    
    sprintf(query, "UPDATE %s.admin SET password = \"%s\" where username = \"%s\";", DATABASE_NAME, password,username);
    makeQuery(connection, query);

    printf("Update was successful\n");
    closeConnection(connection);
}

int validAdmin( char *username, char *password){
    MYSQL *connection = connectDatabase();

    //variable definnition
    MYSQL_RES *res;
    MYSQL_ROW row;

    char query1[100];
    char *DBname;
    char *DBpassword; // in future will be used for password

    int nameStatus = 0;
    int passwordStatus = 0;
    int status = 0; // return 1 if true, 0 if false

    //#####


    //Extracting name email(password) and validate with input
    sprintf(query1, "SELECT username,password FROM %s.admin", DATABASE_NAME);

    mysql_query(connection,query1);

    res = mysql_store_result(connection);

    while(row  = mysql_fetch_row(res)){
        DBname = row[0];
        DBpassword= row[1];

        if( isEqual(DBname, username) ){
            nameStatus = 1;
            if( isEqual(DBpassword, password) ){
                passwordStatus=1;
                break;
            }
            else{
                passwordStatus=0; 
            }
        }
        else{
            nameStatus=0;
        }
    }

    mysql_free_result(res);

    if(nameStatus == 0 || passwordStatus == 0){ 
        status =0;
        }
    else{
        if (nameStatus ==1  && passwordStatus ==1 ){ 
            status = 1; 
            }
    }
   
    closeConnection(connection);
    return status;
}

#endif