#include <stdio.h>

double tax_function(double salary){
    double result = 0;
    double first_step = 300;
    double second_step = 150;
    double first_proc = 0.13;
    double second_proc = 0.15;
    double last_proc = 0.2;
    
    if(salary < (first_step + second_step)){
        second_step = salary - first_step;
        last_proc = 0;
    }
    
    if(salary < first_step){
        first_step = salary;
        second_proc = 0;
    }
    
    result = first_step * first_proc + second_step * second_proc + (salary - first_step - second_step) * last_proc;
        
    return result;
}

int main(){
    double salary = 0;
    double tax = 0;
    int hours = 0;
    int hours_above_norm = 0;
    
    const int NORM = 40;
    const double coef = 1.5;
    const double main_rate = 10;
    printf("Enter how many hours a week do you work: ");
    scanf("%d", &hours);
    hours_above_norm = hours - NORM;
    
    if(hours_above_norm < 0){
        hours_above_norm  = 0;
    }
    else{
        hours -= hours_above_norm;
    }
    
    
    salary = hours * main_rate + hours_above_norm * coef * main_rate;
    
    printf("The total amount accrued for this week: %.2f\n", salary);
    
    tax = tax_function(salary);
    
    printf("The total tax for this week: %.2f\n", tax);
    
    double salary_without_taxes = salary - tax;
    
    printf("The salary without taxes for this week: %.2f\n", salary_without_taxes);
    return 0;

}



