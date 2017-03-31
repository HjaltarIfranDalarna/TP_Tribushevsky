

#include "administrator.h"
#include "operator.h"
#include "work.h"

void operator_menu(){
    printf("-----------------------------\n");
    printf("(1)balance\n");
    printf("(2)credit\n");
    printf("(3)debit\n");
    printf("(4)transaction\n");
    printf("(5)menu\n");
    printf("(0)end work\n");
    printf("-----------------------------\n");
}

void administrator_menu(){
    printf("-----------------------------\n");
    printf("(1)add Client\n");
    printf("(2)update Client Email\n");
    printf("(3)update Client Connect Info\n");
    printf("(4)add Account\n");
    printf("(5)update Account Currency\n");
    printf("(6)update Account Type\n");
    printf("(7)menu\n");
    printf("(0)end work\n");
    printf("-----------------------------\n");
}

void administrator_work(){
    bool work = true;
    int choose;
    administrator_menu();
    while(work){
        printf("Choose: ");
        scanf("%d",&choose);
        switch(choose){
            case 1:{
                addClient();
                break;
            }
            case 2:{
                updateClientEmail();
                break;
            }
            case 3:{
                updateClientConnectInfo();
                break;
            }
            case 4:{
                addAccount();
                break;
            }
            case 5:{
                updateAccountCurrency();
                break;
            }
            case 6:{
                updateAccountType();
                break;
            }
            case 7:{
                administrator_menu();
                break;
            }
            case 0:{
                work = false;
                break;
            }
        }
    }
}



void operationist_work(){
    bool work = true;
    char firstname[20],lastname[20],card[20];
    int choose;
    double money;
    operator_menu();
    while(work){
         printf("Choose: ");
        scanf("%d",&choose);
        switch(choose){
            case 1:{
                
                printf("Enter firstname: ");
                scanf("%s",firstname);
                printf("Enter lastname: ");
                scanf("%s",lastname);
                printf("Enter card number: ");
                scanf("%s",card);
                balance(firstname,lastname,card);
                break;
            }
            case 2:{
                printf("Enter card number: ");
                scanf("%s",card);
                printf("Enter amount: ");
                scanf("%lf",&money);
                credit(card,money);
                break;
            }
            case 3:{
                printf("Enter card number: ");
                scanf("%s",card);
                printf("Enter amount: ");
                scanf("%lf",&money);
                debit(card,money);
                break;
            }
            case 4:{
                char second_card[20];
                printf("Enter first card number: ");
                scanf("%s",card);
                printf("Enter second card number: ");
                scanf("%s",second_card);
                printf("Money for transaction: ");
                scanf("%lf",&money);
                transaction(card, second_card, money);
                break;
            }
            case 5:{
                operator_menu();
                break;
            }
            case 0:{
                work = false;
                break;
            }
        }
    }
}
