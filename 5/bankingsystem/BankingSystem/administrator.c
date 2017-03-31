
#include "administrator.h"
#include <sqlite3.h>
#include <stdlib.h>

bool ans = true;
int count;

static int empty(void *NotUsed, int argc, char **argv, char **azColName){
    return 0;
}

static int existback(void *NotUsed, int argc, char **argv, char **azColName){
    if(argc == 0)
        ans = true;
    else
        ans = false;
    return 0;
}

static int countback(void *NotUsed, int argc, char **argv, char **azColName){
    count = atoi(argv[0]);
    return 0;
}

bool check_if_exist_client(char *firstname,char * lastname){
    sqlite3_stmt *res;
    sqlite3 *db;
    char sql[150];
    char *err_msg = 0;
    const char* data = "Callback function called";
    sqlite3_open("bankingSystem.db", &db);
    sprintf(sql,"Select * from bank_clients where firstname ='%s' and lastname='%s';" ,firstname,lastname);
    
    sqlite3_prepare_v2(db, sql, -1, &res, 0);
    sqlite3_exec(db, sql, existback, (void*)data, &err_msg);
    sqlite3_finalize(res);
    return ans;
}

int how_much_clients(){
    sqlite3_stmt *res;
    sqlite3 *db;
    char sql[150];
    char *err_msg = 0;
    const char* data = "Callback function called";
    sqlite3_open("bankingSystem.db", &db);
    sprintf(sql,"Select count() from BANK_CLIENTS;");
    sqlite3_prepare_v2(db, sql, -1, &res, 0);
    sqlite3_exec(db, sql, countback, (void*)data, &err_msg);
    sqlite3_finalize(res);
    return count;
}

bool addClient(){
    char firstname[20],lastname[20],email[20],number[20],photo[20];
    printf("Enter firstname: ");
    scanf("%s",firstname);
    printf("Enter lastname: ");
    scanf("%s",lastname);
    if(!check_if_exist_client(firstname,lastname)){
        printf("This is existing client!");
        return false;
    }
    else{
        printf("Enter email: ");
        scanf("%s",email);
        printf("Enter telephone number: ");
        scanf("%s",number);
        printf("Enter photo name: ");
        scanf("%s",photo);
        char *sql = "INSERT INTO bank_clients VALUES(?,?,?,?,?,?);";
        sqlite3_stmt *res;
        sqlite3 *db;
        const char* data = "Callback function called";
        sqlite3_open("bankingSystem.db", &db);
        sqlite3_prepare(db, sql, -1, &res, 0);
        FILE *fp = fopen(photo, "rb");
        if (fp == NULL) {
            fprintf(stderr, "Cannot open image file\n");
            return false;
        }
        
        fseek(fp, 0, SEEK_END);
        long flen = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        char ph[flen+1];
        long size = fread(ph, 1, flen, fp);
        fclose(fp);
        sqlite3_bind_int(res,1, how_much_clients()+1);
        sqlite3_bind_text(res, 2, firstname, (int)strlen(firstname), SQLITE_STATIC);
        sqlite3_bind_text(res, 3, lastname, (int)strlen(lastname), SQLITE_STATIC);
        sqlite3_bind_text(res, 4, email, (int)strlen(email), SQLITE_STATIC);
        sqlite3_bind_text(res, 5, number, (int)strlen(number), SQLITE_STATIC);
        sqlite3_bind_blob(res, 6, ph, (int)size, SQLITE_STATIC);
        sqlite3_step(res);
        sqlite3_finalize(res);
    }
    return true;
}

bool updateClientEmail(){
    char firstname[20],lastname[20],email[20];
    printf("Enter firstname: ");
    scanf("%s",firstname);
    printf("Enter lastname: ");
    scanf("%s",lastname);
    if(check_if_exist_client(firstname,lastname)){
        printf("This is existing client!");
        return false;
    }
    else{
        printf("Enter email: ");
        scanf("%s",email);
        sqlite3_stmt *res;
        sqlite3 *db;
        char *err_msg = 0;
        const char* data = "Callback function called";
        sqlite3_open("bankingSystem.db", &db);
        char sql[150];
        sprintf(sql,"Update bank_clients set email = '%s' where firstname='%s' and lastname = '%s';",email,firstname,lastname);
        sqlite3_prepare_v2(db, sql, -1, &res, 0);
        sqlite3_exec(db, sql, empty, (void*)data, &err_msg);
        sqlite3_finalize(res);
        return true;
    }
    return false;
}

bool updateClientConnectInfo(){
    char firstname[20],lastname[20],connect[20];
    printf("Enter firstname: ");
    scanf("%s",firstname);
    printf("Enter lastname: ");
    scanf("%s",lastname);
    if(check_if_exist_client(firstname,lastname)){
        printf("This is not existing client!");
        return false;
    }
    else{
        printf("Enter telephone number: ");
        scanf("%s",connect);
        sqlite3_stmt *res;
        sqlite3 *db;
        char *err_msg = 0;
        const char* data = "Callback function called";
        sqlite3_open("bankingSystem.db", &db);
        char sql[150];
        sprintf(sql,"Update bank_clients set connect_info = '%s' where firstname='%s' and lastname = '%s';",connect,firstname,lastname);
        sqlite3_prepare_v2(db, sql, -1, &res, 0);
        sqlite3_exec(db, sql, empty, (void*)data, &err_msg);
        sqlite3_finalize(res);
        return true;
    }
    return false;
}

bool check_if_exist_account(char *card){
    sqlite3_stmt *res;
    sqlite3 *db;
    char sql[150];
    char *err_msg = 0;
    const char* data = "Callback function called";
    sqlite3_open("bankingSystem.db", &db);
    sprintf(sql,"Select * from bank_accounts where card='%s';" ,card);
    sqlite3_prepare_v2(db, sql, -1, &res, 0);
    sqlite3_exec(db, sql, existback, (void*)data, &err_msg);
    sqlite3_finalize(res);
    return ans;
}

int how_much_accounts(){
    sqlite3_stmt *res;
    sqlite3 *db;
    char sql[150];
    char *err_msg = 0;
    const char* data = "Callback function called";
    sqlite3_open("bankingSystem.db", &db);
    sprintf(sql,"Select count() from BANK_Accounts;");
    sqlite3_prepare_v2(db, sql, -1, &res, 0);
    sqlite3_exec(db, sql, countback, (void*)data, &err_msg);
    sqlite3_finalize(res);
    return count;
}

bool addAccount(){
    char card[20],	currency[20],startdate[20];
    int balance,client_id,conf_id,type;
    printf("Enter fcard number: ");
    scanf("%s",card);
    if(!check_if_exist_account(card)){
        printf("This is existing account!");
        return false;
    }
    else{
        printf("Enter currency: ");
        scanf("%s",currency);
        printf("Enter start date: ");
        scanf("%s",startdate);
        printf("Enter balnce: ");
        scanf("%d",&balance);
        printf("Enter client id: ");
        scanf("%d",&client_id);
        printf("Enter conig id: ");
        scanf("%d",&conf_id);
        printf("Enter type: ");
        scanf("%d",&type);
        while(!(type==0 || type ==1)){
            printf("Enter type: ");
            scanf("%d",&type);
        }
        char sql[150];
        sqlite3_stmt *res;
        sqlite3 *db;
        char *err_msg = 0;
        const char* data = "Callback function called";
        sprintf(sql,"INSERT INTO bank_accounts VALUES(%d,%d,'%s','%s',%d,%d,'%s',%d);" ,how_much_accounts()+1,
                    balance,startdate,currency,client_id,conf_id,card,type);
        sqlite3_open("bankingSystem.db", &db);
        
        sqlite3_prepare_v2(db, sql, -1, &res, 0);
        
        sqlite3_exec(db, sql, existback, (void*)data, &err_msg);
        sqlite3_finalize(res);
    }
    return true;

}

bool updateAccountCurrency(){
    char card[20],	currency[20];
    printf("Enter fcard number: ");
    scanf("%s",card);
    if(check_if_exist_account(card)){
        printf("This is not existing account!");
        return false;
    }
    else{
        printf("Enter currency: ");
        scanf("%s",currency);
        sqlite3_stmt *res;
        sqlite3 *db;
        char *err_msg = 0;
        const char* data = "Callback function called";
        sqlite3_open("bankingSystem.db", &db);
        char sql[150];
        sprintf(sql,"Update bank_accounts set currency= '%s' where card = '%s';",currency,card);
        sqlite3_prepare_v2(db, sql, -1, &res, 0);
        sqlite3_exec(db, sql, empty, (void*)data, &err_msg);
        sqlite3_finalize(res);
        return true;
    }
    return false;

}

bool updateAccountType(){
    char card[20];
    int type,conf_id;
    printf("Enter fcard number: ");
    scanf("%s",card);
    if(check_if_exist_account(card)){
        printf("This is not existing account!");
        return false;
    }
    else{
        printf("Enter type: ");
        scanf("%d",&type);
        while(!(type==0 || type ==1)){
            printf("Enter type: ");
            scanf("%d",&type);
        }
        printf("Enter config id: ");
        scanf("%d",&conf_id);
        sqlite3_stmt *res;
        sqlite3 *db;
        char *err_msg = 0;
        const char* data = "Callback function called";
        sqlite3_open("bankingSystem.db", &db);
        char sql[150];
        sprintf(sql,"Update bank_accounts set type= %d,BANK_CONFIG_id = %d where card = '%s';",type,conf_id,card);
        sqlite3_prepare_v2(db, sql, -1, &res, 0);
        sqlite3_exec(db, sql, empty, (void*)data, &err_msg);
        sqlite3_finalize(res);
        return true;
    }
    return false;
}
