#include <stdio.h>
#include "authentication.h"
#include"work.h"

int p;
char lg[20],password[20];

void enter(){
    printf("Enter login: ");
    scanf("%s",lg);
    printf("Enter password: ");
    scanf("%s",password);
    printf("Enter position(0 - operator,1 - administrator): ");
    scanf("%d",&p);
    while(!(p==0 || p ==1)){
        printf("Enter position(0 - operator,1 - administrator): ");
        scanf("%d",&p);
    }
}

int main(){
    
    enter();
    while(!authenticate(lg, password,p)){
        printf("Try again\n");
        enter();
    }

    if(p){
        administrator_work();
    }
    else{
        operationist_work();
    }
    
    return 0;
}


