
#include "operator.h"
#include <stdio.h>
#include <sqlite3.h>
#include <stdlib.h>
#define MAXCOUNTSIGNINOPERATION 15

char push_money_balance[MAXCOUNTSIGNINOPERATION];

static double overdraft = 0;

static int callback_credit_pull(void *data, int argc, char **argv, char **azColName){
    
    char* string_data = data;
    double push_money = atof(string_data);
    double balance = atof(argv[0]);
    balance += push_money;
    
    sprintf(push_money_balance, "%.2f", balance);
    return 0;
}

static int callback_overdraft(void *data, int argc, char **argv, char **azColName){
    overdraft = atof(argv[0]);
    return 0;
}


double credit(char* card, double push_money){
    
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    char sql[SQLSIZE];
    
    rc = sqlite3_open("bankingSystem.db", &db);
    if(rc){
        return false;
    }
    
    sprintf(sql, "SELECT overdraft FROM BANK_CONFIG WHERE id=(SELECT BANK_CONFIG_id FROM BANK_ACCOUNTS WHERE card='%s');", card);
    rc = sqlite3_exec(db, sql, callback_overdraft, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
        sqlite3_free(zErrMsg);
        return false;
    }
    double to_balance = 0;
    overdraft -= push_money;
    if(overdraft < 0){
        to_balance = -overdraft;
        overdraft = 0;
        
        sprintf(sql, "UPDATE BANK_CONFIG SET overdraft = 0 WHERE id=(SELECT BANK_CONFIG_id FROM BANK_ACCOUNTS WHERE card='%s');", card);
        rc = sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
        if( rc != SQLITE_OK ){
            sqlite3_free(zErrMsg);
            return false;
        }
    }else{
        to_balance = push_money;
    }
    sprintf(sql,"SELECT balance FROM BANK_ACCOUNTS WHERE card='%s';", card);
    
    char push_money_string[MAXCOUNTSIGNINOPERATION];
    sprintf(push_money_string, "%.2f", to_balance);
    rc = sqlite3_exec(db, sql, callback_credit_pull, push_money_string, &zErrMsg);
    if( rc != SQLITE_OK ){
        sqlite3_free(zErrMsg);
        return false;
    }
    
    sprintf(sql,"UPDATE BANK_ACCOUNTS SET balance = %s WHERE card='%s';",
            push_money_balance, card);
    
    rc = sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
        sqlite3_free(zErrMsg);
        return false;
    }
    double returned_balance = atof(push_money_balance);
    sqlite3_close(db);
    return returned_balance;
}
