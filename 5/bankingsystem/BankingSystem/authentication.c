#include "authentication.h"
#include <sqlite3.h>
#include <stdio.h>

bool answer;

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
    if(argc == 0){
        answer = false;
        return 4;
    }
    answer = true;
    return 0;
}



bool authenticate(char *login,char *password,int position){
    sqlite3_stmt *res;
    sqlite3 *db;
    char sql[150];
    char *err_msg = 0;
    const char* data = "Callback function called";
    sqlite3_open("bankingSystem.db", &db);
    sprintf(sql,"Select * from BANK_USERS where login ='%s' and password='%s' and position=%d;" ,login ,password,position);
    sqlite3_prepare_v2(db, sql, -1, &res, 0);
    sqlite3_exec(db, sql, callback, (void*)data, &err_msg);
    sqlite3_finalize(res);
    return answer;
}



