#include <stdio.h>
#include <time.h>
int main() {
    struct tm t_birth;
    time_t t_now = time(NULL);
    int day, month, year, hours, minutes;
    long long life;
    
    printf("Введите дату рождения в формате (dd.mm.yyyy hh:mm): \n");
    scanf("%d.%d.%d %d:%d", &day, &month, &year, &hours, &minutes);
    
    t_birth.tm_year = year - 1900;
    t_birth.tm_mon = month;
    t_birth.tm_mday = day;
    t_birth.tm_hour = hours;
    t_birth.tm_min = minutes;
    t_birth.tm_sec = 0;
    life = t_now - mktime(&t_birth);
    printf("Ваша жизнь в секундах: %lli\n", life);
    return 0;
}
