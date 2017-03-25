
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
char* month_array[12] = {"january", "february", "march", "april", "may", "june", "july", "august", "september", "october", "november", "desember"};
int day_in_month[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
int get_num_month(char* month){
    int IfNum = atoi(month);
    if(IfNum != 0){
        return IfNum;
    }
    else{
        int k;
        for(int i = 0; i < 12; i++){
            if(!strncmp(month, month_array[i], 3)){
                k = i + 1;
                break;
            }
        }
        return k;
    }
}
bool is_leap(int year){
    return ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0);
}

int main(){
    int year, day, result;
    char month[30];
    printf("Введите день: ");
    scanf("%i", &day);
    printf("Введите месяц: ");
    scanf("%s", month);
    printf("Введите год: ");
    scanf("%i", &year);
    int month_in_int = get_num_month(month);
    if(is_leap(year) && month_in_int > 2)
        result = 1;
    else
        result = 0;
    for(int i = 0; i < month_in_int - 1; i++){
        result += day_in_month[i];
    }
    result += day;
    printf("Дней с начала года: %d\n", result);
    return 0;
}
