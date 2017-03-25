#include <stdio.h>
#include <sqlite3.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#define FIELDSCOUNT 11

const int TIME_STRING_LENGTH = 11;
const int LINE_SIZE = 80;

void ShowMenu();
void INSERT(sqlite3 *db);
void SELECT(sqlite3 *db);
void DELETE(sqlite3 *db);
static int callback(void *NotUsed, int argc, char **argv, char **azColName);

enum fieldname {firstname = 0, patronymic, lastname, birthdateP, photopath, birthcity, birthcountry, address, department, position, hiringdateP};

int main(int argc, const char * argv[]) {
    sqlite3 *db;
    char *err_msg = 0;
    
    int rc = sqlite3_open("EMPLOYEES.db", &db);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Невозможно открыть базу данных: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }
    int c;
    bool flag = true;
    ShowMenu();
    while (flag) {
        
        printf("Ваш выбор: ");
        scanf("%d", &c);
        switch (c) {
            case 1:
                SELECT(db);
                break;
            case 2:
                INSERT(db);
                break;
            case 3:
                DELETE(db);
                break;
            case 4:
                ShowMenu();
                break;
            case 5:
                flag = false;
                break;
            default:
                printf("Некоректный выбор.\n");
                break;
        }
    }
    sqlite3_close(db);
    
    return 0;
}

void ShowMenu(){
    printf("======= Меню ======\n"\
           "1. SELECT\n"\
           "2. INSERT\n"\
           "3. DELETE\n"\
           "4. SHOW MENU\n"\
           "5. EXIT\n"\
           "===================\n");
}

void SELECT(sqlite3 *db){
    char criterion[LINE_SIZE];
    char value[LINE_SIZE];
    const char* data = "Callback function called";
    char *err_msg = 0;
    sqlite3_stmt *res;
    
    printf("Критерии: id, фамилия, страна.\n");
    printf("Введите критерий: ");
    scanf("%s", criterion);
    
    char sql[3*LINE_SIZE];
    if(strcmp(criterion,"id") == 0){
        strcpy(sql, "SELECT * FROM employees WHERE id = " );
    }
    else
        if(strcmp(criterion,"фамилия") == 0){
            strcpy(sql, "SELECT * FROM employees WHERE lastName = '");
        }
        else
            if(strcmp(criterion,"страна") == 0){
                strcpy(sql, "SELECT * FROM employees WHERE country LIKE '");
            }
    
    printf("Введите значение: ");
    scanf("%s", value);
    strcat(sql, value);
    if(strcmp(criterion,"страна") == 0)
        strcat(sql, "'");
    else
        if(strcmp(criterion,"фамилия") == 0)
            strcat(sql, "'");
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Ошибка во время выполнения: %s\n", sqlite3_errmsg(db));
        return;
    }
    
    rc = sqlite3_exec(db, sql, callback, (void*)data, &err_msg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "Ошибка SQL: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
    
    
    FILE *fp = fopen("sp1.jpg", "wb");
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    rc = sqlite3_step(res);
    fwrite(sqlite3_column_blob(res, photopath + 1),
           sqlite3_column_bytes(res, photopath + 1), 1, fp);
    fclose(fp);
    
    sqlite3_finalize(res);
    
    return;
}

void INSERT(sqlite3 *db){
    sqlite3_stmt *res;
    char *sql;
    printf("Transaction of Autocomit: ");
    char answer[15];
    scanf("%s", answer);
    if(strcmp(answer, "Autocomit")){
        sql = "INSERT INTO  employees VALUES(?,?,?,?,?,?,?,?,?,?,?,?);";
    }
    else{
        sql = " BEGIN TRANSACTION; \
                INSERT INTO employees VALUES(?,?,?,?,?,?,?,?,?,?,?,?);\
                COMMIT;";
    }
    char *err_msg = 0;
    char fields[11][LINE_SIZE];
    struct tm hiringdate, birthdate;
    
    printf("Полное имя (ФИО): ");
    scanf("%s %s %s", fields[lastname], fields[firstname], fields[patronymic]);
    printf("Дата рождения (дд мм гггг): ");
    scanf("%d %d %d", &birthdate.tm_mday, &birthdate.tm_mon, &birthdate.tm_year);
    printf("Путь к фотографии (insert double \"/\"): ");
    scanf("%s", fields[photopath]);
    printf("Страна и город рождения: ");
    scanf("%s %s", fields[birthcountry], fields[birthcity]);
    printf("Адрес проживания: ");
    scanf("%s", fields[address]);
    printf("Первый день на работе (dd mm yyyy): ");
    scanf("%d %d %d", &hiringdate.tm_mday, &hiringdate.tm_mon, &hiringdate.tm_year);
    printf("Отделение: ");
    scanf("%s", fields[department]);
    printf("Должность: ");
    scanf("%s", fields[position]);
    
    birthdate.tm_mon--;
    hiringdate.tm_mon--;
    
    strftime(fields[hiringdateP], TIME_STRING_LENGTH, "%Y-%m-%d", &hiringdate);
    strftime(fields[birthdateP], TIME_STRING_LENGTH, "%Y-%m-%d", &birthdate);
    
    int rc = sqlite3_prepare(db, sql, -1, &res, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Ошибка во время добавления: %s\n", sqlite3_errmsg(db));
        return;
    }
    
    FILE *fp = fopen(fields[photopath], "rb");
    if (fp == NULL) {
        fprintf(stderr, "Невозможно открыть фото.\n");
        return;
    }
    
    fseek(fp, 0, SEEK_END);
    int flen = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char data[flen+1];
    int size = fread(data, 1, flen, fp);
    fclose(fp);
    for(int i = 0; i < FIELDSCOUNT; i++){
        sqlite3_bind_text(res, i+2, fields[i], (int)strlen(fields[i]), SQLITE_STATIC);
    }
    sqlite3_bind_blob(res, photopath+2, data, size, SQLITE_STATIC);
    
    
    int step = sqlite3_step(res);
    
    if (step == SQLITE_ROW) {
        printf("%s: ", sqlite3_column_text(res, 0));
        printf("%s\n", sqlite3_column_text(res, 1));
    }
    
    sqlite3_finalize(res);
    
    if (rc != SQLITE_OK ) {
        fprintf(stderr, "Ошибка SQL: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return;
    }
    
    return;
}

void DELETE(sqlite3 *db){
    
    struct tm date;
    char criterion[LINE_SIZE];
    char value[LINE_SIZE];
    char *err_msg = 0;
    sqlite3_stmt *res;
    
    printf("Критерии: id, firstName, patronymic, lastName, birthday, locality,\n\
           country, address, department, position, firstDay.\n");
    printf("Enter criterion: ");
    scanf("%s", criterion);
    char sql[3*LINE_SIZE];
    strcpy(sql,"DELETE FROM employees WHERE ");
    strcat(sql, criterion);
    strcat(sql, " = ?;");
    int rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Ошибка во время выполнения: %s\n", sqlite3_errmsg(db));
        return;
    }
    
    if(strcmp(criterion, "birthdate") == 0 || strcmp(criterion, "hiringdate") == 0){
        printf("Введите дату рождения(дд мм гггг): ");
        scanf("%d %d %d", &date.tm_mday, &date.tm_mon, &date.tm_year);
        date.tm_mon--;
        strftime(value, TIME_STRING_LENGTH, "%Y-%m-%d", &date);
        printf("%s", value);
        sqlite3_bind_text(res, 1, value, (int)strlen(value), SQLITE_STATIC);
    }
    else{
        if(strcmp(criterion, "id") == 0){
            int id;
            printf("Введите id: ");
            scanf("%d", &id);
            sqlite3_bind_int(res, 1, id);
        }
        else{
            printf("Введите значение: ");
            scanf("%s", value);
            sqlite3_bind_text(res, 1, value, (int)strlen(value), SQLITE_STATIC);
        }
    }
    
    int step = sqlite3_step(res);
    
    if (step == SQLITE_ROW) {
        printf("%s: ", sqlite3_column_text(res, 0));
        printf("%s\n", sqlite3_column_text(res, 1));
    }
    
    sqlite3_finalize(res);
    
    if (rc != SQLITE_OK ) {
        fprintf(stderr, "Ошибка SQL: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return;
    }
    
    return;
}

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
    int i;
    for(i=0; i<argc; i++){
        if(i == photopath + 1)
            continue;
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}
