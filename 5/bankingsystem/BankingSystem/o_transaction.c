
#include <stdio.h>
#include <sqlite3.h>
#include "operator.h"

bool transaction(char*  from_card, char* to_card, double money){
    
    sqlite3 *db;
    int rc;
    
    rc = sqlite3_open("bankingSystem.db", &db);
    if(rc){
        return false;
    }
    
    double _balance = credit(from_card, 0);
    
    if((_balance - money) >= 0){
        debit(from_card, money);
        credit(to_card, money);
        return true;
    }else{
        return false;
    }
}
