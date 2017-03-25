#include <stdio.h>
int main(){
    
    int year = 0;
    int leap_year = 0;
    printf("Enter your age: ");
    scanf("%i", &year);
    leap_year = year / 4;
    int days = year * 365 + leap_year;
    printf("Your age in days: %i\n", days);
    return 0;
}


