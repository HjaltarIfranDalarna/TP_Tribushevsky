

#ifndef operator_h
#define operator_h

#include <stdbool.h>
#define SQLSIZE 200
#define MAXOVERDRAFT 200
#define MAXDAYSOVERDRAFT 30
#define OVERDRAFTFINEPERDAY 2

void balance(char* first_name, char* last_name, char* account_number);

double credit(char* account_number, double push_money);

bool debit(char* account_number, double pull_money);

bool transaction();

#endif
