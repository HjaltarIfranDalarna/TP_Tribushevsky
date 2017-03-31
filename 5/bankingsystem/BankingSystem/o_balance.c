
#include "operator.h"
#include <sqlite3.h>
#include <stdio.h>

static int callback_balance(void *data, int argc, char **argv){

    char** string_data = data;
    printf("%s %s\n", string_data[1], string_data[0]);
    printf("Account number: %s\n", string_data[2]);
    printf("Current balance: %s\n", argv[0]);
    return 0;
}

void print_not_found(char* first_name, char* last_name, char* card){
    printf("%s %s", last_name, first_name);
    printf("Account number: %s", card);
    printf("+-------------------------+\n");
    printf("|     Info not found      |\n");
    printf("+-------------------------+\n");
}

void balance(char* first_name, char* last_name, char* card){
    
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    char sql[SQLSIZE];
    char *info[3] = { first_name, last_name, card };
    rc = sqlite3_open("bankingSystem.db", &db);
    if(rc){
        print_not_found(first_name, last_name, card);
        return;
    }
    
    sprintf(sql,"SELECT balance FROM BANK_ACCOUNTS WHERE card='%s';" ,card);
    
    rc = sqlite3_exec(db, sql, callback_balance, info, &zErrMsg);
    if( rc != SQLITE_OK ){
        print_not_found(first_name, last_name, card);
        sqlite3_free(zErrMsg);
    }
    
    sqlite3_close(db);
}



