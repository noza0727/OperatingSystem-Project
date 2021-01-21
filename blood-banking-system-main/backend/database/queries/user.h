#ifndef USER_QUERIES_H
#define USER_QUERIES_H

#include "../connector.h"
#include "../../../shared/entities/entities.h"
#include "./record.h"

void insertNewUser(char *phone_number, char *password,char *name, char *date, char *bloodType, char *email, char *region){
    MYSQL *connection = connectDatabase();

    char query[300];
    
    sprintf(query, "INSERT INTO %s.user (phone_number, password, name, dateofbirth, email, region) VALUES(\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\");", DATABASE_NAME, phone_number, password, name, date, email, region);
   
    printf("%s\n",query);
    
    makeQuery(connection, query);

    printf("Insertion was successful\n");
    closeConnection(connection);
}

struct User selectUserByPhoneNumber(char *phone){
    MYSQL *connection = connectDatabase();

    struct User user;

    MYSQL_RES *res;
    MYSQL_ROW row;
    char query[100];

    sprintf(query, "SELECT * FROM %s.user where phone_number = \"%s\";", DATABASE_NAME, phone);

    mysql_query(connection,query);

    res = mysql_store_result(connection);
    while(row  = mysql_fetch_row(res)){
        user.id = atoi(row[0]);

        sprintf(user.phoneNumber, "%s", row[1]);
        sprintf(user.password, "%s", row[2]);
        sprintf(user.name, "%s", row[3]);
        sprintf(user.date, "%s", row[4]);
        sprintf(user.email, "%s", row[6]);
        sprintf(user.region, "%s", row[8]);

        if (row[5] == NULL) {
            sprintf(user.bloodType, "%s", "Undefined");
        } else {
            sprintf(user.bloodType, "%s", row[5]);
        }

        struct Record record;

        if (row[7] == NULL) {
            record.id = 0;
        } else {
            record = getRecordById(atoi(row[7]));
        }

        user.lastDonation = record;
    }

    mysql_free_result(res);
    closeConnection(connection);
    return user;
}

struct User selectUserByEmail(char *email){
    MYSQL *connection = connectDatabase();

    struct User user;

    MYSQL_RES *res;
    MYSQL_ROW row;
    char query[100];

    sprintf(query, "SELECT * FROM %s.user where email = \"%s\";", DATABASE_NAME, email);

    mysql_query(connection,query);

    res = mysql_store_result(connection);
    while(row  = mysql_fetch_row(res)){
        user.id = atoi(row[0]);

        sprintf(user.phoneNumber, "%s", row[1]);
        sprintf(user.password, "%s", row[2]);
        sprintf(user.name, "%s", row[3]);
        sprintf(user.date, "%s", row[4]);
        sprintf(user.bloodType, "%s", row[5]);
        sprintf(user.email, "%s", row[6]);
        sprintf(user.region, "%s", row[8]);
    }
    mysql_free_result(res);
    closeConnection(connection);
    return user;
}

void updateUser(char *phone_number, char *password,char *name, char *date, char *bloodType,char *email, char *region){
    MYSQL *connection = connectDatabase();

    char query[300];

    sprintf(query, "UPDATE %s.user SET password = \"%s\", name = \"%s\",dateofbirth= \"%s\", bloodtype = \"%s\", phone_number = \"%s\", region = \"%s\" where email = \"%s\";",
    DATABASE_NAME, password, name, date, bloodType, phone_number, region, email);

    makeQuery(connection, query);

    printf("Update was successful\n");
    closeConnection(connection);
}

void updateUserBloodType(char *phone_number,char *bloodType){
    MYSQL *connection = connectDatabase();

    char query[100];

    sprintf(query, "UPDATE %s.user SET bloodtype = \"%s\" where phone_number = \"%s\";", DATABASE_NAME, bloodType,phone_number);

    makeQuery(connection, query);

    printf("Update was successful\n");
    closeConnection(connection);

}

/*void updateUserStatus(char *user_id, char *status){
    MYSQL *connection = connectDatabase();

    char query[100];

    sprintf(query, "UPDATE %s.user SET where donor_id = \"%s\";", DATABASE_NAME, user_id);


    if((isEqual(status,"Donor") || (isEqual(status,"Recipient")))){
        sprintf(query, "UPDATE %s.user SET userstatus = \"%s\" where phone_number = \"%s\";", DATABASE_NAME, status, phone_number);

        makeQuery(connection, query);

        printf("Update was successful\n");
    }
    else{
        printf("Invalid user status parameter, try again\n");
    }
    
    closeConnection(connection);

}*/

int validUser(char *phone_number, char *password){  // not used
    MYSQL *connection = connectDatabase();

    //variable definnition
    MYSQL_RES *res;
    MYSQL_ROW row;

    char query[100];
    char *DBphone_number;
    char *DBpassword; // in future will be used for password

    char *phoneNum = phone_number;
    char *userPassword = password;

    int phoneNumberStatus = 0;
    int passwordStatus = 0;
    int status = 0; // return 1 if true, 0 if false

    //Extracting name email(password) and validate with input
    sprintf(query, "SELECT phone_number,password FROM %s.user", DATABASE_NAME);

    mysql_query(connection, query);

    res = mysql_store_result(connection);

    while(row  = mysql_fetch_row(res)){
        DBphone_number = row[0];
        DBpassword= row[1];

        if(isEqual(DBphone_number,phoneNum)){
            phoneNumberStatus=1;
            if(isEqual(DBpassword, userPassword)){
                passwordStatus=1;
                break;
            }
            else{
                passwordStatus=0;
            }
        }
        else{
            phoneNumberStatus=0;
        }
    }

    mysql_free_result(res);

    if(phoneNumberStatus == 0 || passwordStatus == 0){ 
        status = 0;
    }
    else{
        if (phoneNumberStatus == 1  && passwordStatus == 1 ){ 
            status = 1;
        }
    }
  
    closeConnection(connection);

    return status;
}

struct User* getAllUsers(){
    MYSQL *connection = connectDatabase();

    struct User users[4096];
    MYSQL_RES *res;
    MYSQL_ROW row;
    char query[100];

    sprintf(query, "SELECT * FROM %s.user;", DATABASE_NAME);
    
    int i=0;

    mysql_query(connection, query);

    res = mysql_store_result(connection);
    while(row  = mysql_fetch_row(res)) {
        users[i].id = atoi(row[0]);

        sprintf(users[i].phoneNumber, "%s", row[1]);
        sprintf(users[i].password, "%s", row[2]);
        sprintf(users[i].name, "%s", row[3]);
        sprintf(users[i].date, "%s", row[4]);
        sprintf(users[i].bloodType, "%s", row[5]);
         sprintf(users[i].email, "%s", row[6]);
          sprintf(users[i].region, "%s", row[8]);
        i++;
    }
    
    i = 0;
    mysql_free_result(res); // free result set
    closeConnection(connection); // close connection

    return users; //returning array of all users
}

/* 
DR_user* ShowAllUsers(DR_user people[]){

    for(int i=0;i<sizeof(users)/sizeof(DR_user);i++ ){
        users[i].id = 0;
    }

    GetAllDRUsers();

    for(int i=0;i<sizeof(users)/sizeof(DR_user);i++ ){
        people[i]=users[i];
    }
    return people;
}
*/


#endif