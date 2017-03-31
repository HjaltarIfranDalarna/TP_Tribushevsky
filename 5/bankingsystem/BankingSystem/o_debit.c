
#include "operator.h"
#include <stdio.h>
#include <sqlite3.h>
#include <stdlib.h>
#include "system_info.h"
#define MAXCOUNTSIGNINOPERATION 15


char pull_money_balance[MAXCOUNTSIGNINOPERATION];
static double overdraft = 0;
static bool is_standart_account = true;


static int callback_overdraft(void *data, int argc, char **argv, char **azColName){
    overdraft = atof(argv[0]);
    return 0;
}

static int callback_is_standart(void *data, int argc, char **argv, char **azColName){
    int temp_is_standart_account = atoi(argv[0]);
    if(!temp_is_standart_account)
        is_standart_account = false;
    return 0;
}

bool debit(char* card, double pull_money){
    
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    char sql[SQLSIZE];
    
    rc = sqlite3_open("bankingSystem.db", &db);
    if(rc){
        return false;
    }
    
    sprintf(sql,"SELECT type FROM BANK_ACCOUNTS\
            WHERE card='%s';", card);
    
    rc = sqlite3_exec(db, sql, callback_is_standart, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
        sqlite3_free(zErrMsg);
        return false;
    }
    
    double balance = credit(card, 0);
    
    if(is_standart_account){
        if((balance + MAXOVERDRAFT) - pull_money >= 0){
            char balance_string[MAXCOUNTSIGNINOPERATION];
            
            if((balance - pull_money) < 0){

                sprintf(sql, "SELECT overdraft FROM BANK_CONFIG WHERE id=(SELECT BANK_CONFIG_id FROM BANK_ACCOUNTS WHERE card='%s');", card);
                rc = sqlite3_exec(db, sql, callback_overdraft, 0, &zErrMsg);
                if( rc != SQLITE_OK ){
                    sqlite3_free(zErrMsg);
                    return false;
                }
                
                double push_overdraft = -1 * (balance - pull_money);
                if((overdraft + push_overdraft) > 200)
                    return false;
                
                sprintf(sql, "UPDATE BANK_ACCOUNTS SET balance = 0\
                        WHERE card='%s';", card);
                rc = sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
                if( rc != SQLITE_OK ){
                    sqlite3_free(zErrMsg);
                    return false;
                }
                
                char string_overdraft[MAXCOUNTSIGNINOPERATION];
                sprintf(string_overdraft, "%.2f", push_overdraft + overdraft);
                
                sprintf(sql, "UPDATE BANK_CONFIG SET overdraft = %s WHERE id=(SELECT BANK_CONFIG_id FROM BANK_ACCOUNTS WHERE card='%s');", string_overdraft, card);
                rc = sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
                if( rc != SQLITE_OK ){
                    sqlite3_free(zErrMsg);
                    return false;
                }
                return true;
            }
            sprintf(balance_string, "%.2f", balance - pull_money);
            sprintf(sql, "UPDATE BANK_ACCOUNTS SET balance = %s\
                    WHERE card='%s';",
                    balance_string, card);
            
            rc = sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
            if( rc != SQLITE_OK ){
                sqlite3_free(zErrMsg);
                return false;
            }
            char PRESENTDAY_string[MAXCOUNTSIGNINOPERATION];
            sprintf(PRESENTDAY_string, "%d", PRESENTDAY);
            sprintf(sql, "UPDATE BANK_ACCOUNTS SET start_date = %s\
                    WHERE card='%s';",
                    PRESENTDAY_string, card);
            
            rc = sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
            if( rc != SQLITE_OK ){
                sqlite3_free(zErrMsg);
                return false;
            }
            
            return true;
        }else{
            return false;
        }
    }else{
        if((balance - pull_money) >= 0){
            char string_push_balance[MAXCOUNTSIGNINOPERATION];
            sprintf(string_push_balance, "%f", balance - pull_money);
            sprintf(sql, "UPDATE BANK_ACCOUNTS SET balance = %s WHERE card='%s';", string_push_balance, card);
            rc = sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
            if( rc != SQLITE_OK ){
                sqlite3_free(zErrMsg);
                return false;
            }
        }else{
            return false;
        }
    }
    
    is_standart_account = true;
    sqlite3_close(db);
    return true;
}
