
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define COUNTPASSENGER 12

int count_free_place = 12;
int count_reserved_place = 0;
typedef struct{
    int number;
    int is_reserved;
    char first_name[20];
    char last_name[20];
} place;

void sort(place* aircraft){
    for(int i = 0; i < count_reserved_place; i++){
        for(int j = i; j < count_reserved_place; j++)
            if(strcmp(aircraft[i].last_name, aircraft[j].last_name) > 0){
                place tmp = aircraft[i];
                aircraft[i] = aircraft[j];
                aircraft[j] = tmp;
            }
    }
}
void show_menu(){
    printf("  \n==========================\n");
    printf("  a. Show count free place\n");
    printf("  b. Show the list of free places\n");
    printf("  c. Show the list of reserved places\n");
    printf("  d. Book your place\n");
    printf("  e. Remove from your place\n");
    printf("  f. Quit the program") ;
    printf("  \n==========================\n");
}

void save_change(place *result, char* file_name, int count_passenger){
    FILE *file_out;
    file_out = fopen(file_name, "w");
    for(int i = 0; i < count_passenger; i++){
        fprintf(file_out, "%d -> %d -> %s -> %s\n",
               result[i].number,
               result[i].is_reserved,
               result[i].first_name,
               result[i].last_name);
    }
    fclose(file_out);
}

void show_count_free_place(int count){
    printf("The aircraft has %d free place\n", count);
}

void show_list_free_place(place* aircraft){
    printf("List of free places:\n");
    for(int i = 0; i < COUNTPASSENGER; i++){
        if(!aircraft[i].is_reserved){
            printf("%d  %s  %s\n",
                   aircraft[i].number,
                   aircraft[i].first_name,
                   aircraft[i].last_name);
        }
    }
}

void show_list_reserved_place(place* aircraft){
    printf("List of reserved places:\n");
    place* array_reserve = (place*)calloc(count_reserved_place, sizeof(place));
    int j = 0;
    for(int i = 0; i < COUNTPASSENGER; i++){
        if(aircraft[i].is_reserved){
            array_reserve[j++] = aircraft[i];
        }
    }
    sort(array_reserve);
    for(int i = 0; i < count_reserved_place; i++){
            printf("%d  %s  %s\n",  array_reserve[i].number,
                                    array_reserve[i].first_name,
                                    array_reserve[i].last_name);
    }
    free(array_reserve);
}

bool book_place(place* new_place, int number, char* first_name, char* last_name){
    if(new_place[number - 1].is_reserved == 0){
        new_place[number - 1].is_reserved = 1;
        strcpy(new_place[number - 1].first_name, first_name);
        strcpy(new_place[number - 1].last_name, last_name);
        return true;
    }
    return false;
}


bool remove_place(place* new_place, int number){
    if(new_place[number - 1].is_reserved == 1){
        new_place[number - 1].is_reserved = 0;
        strcpy(new_place[number - 1].first_name, "-");
        strcpy(new_place[number - 1].last_name, "-");
        return true;
    }
    return false;
}

bool quit_program(place* aircraft){
    save_change(aircraft, "input.txt", COUNTPASSENGER);
    printf("The program has finished.\n");
    return 1;
}

place* load_data(FILE *file){
    
    place* aircraft = (place*)calloc(12, sizeof(place));
    int i = 0;
    int ret;
    while((ret = fscanf(file, "%d -> %d -> %s -> %s",
                        &aircraft[i].number,
                        &aircraft[i].is_reserved,
                        &aircraft[i].first_name,
                        &aircraft[i].last_name))){
        if(i >= 12)
            break;
        
        if(ret == EOF)
            break;
        
        ++i;
    }
    
    for(int j = 0; j < i; j++){
        if(aircraft[j].is_reserved){
            --count_free_place;
            ++count_reserved_place;
        }
    }
    return aircraft;
    
}

bool set_decide(place* aircraft){
    char answer_arr[2];
    char answer;
    while(true){
        show_menu();
        printf("Enter the selected menu item: ");
        scanf("%s", &answer_arr);
        answer = answer_arr[0];
        switch (answer) {
            case 'a':{
                show_count_free_place(count_free_place);
            }
                break;
                
            case 'b':{
                show_list_free_place(aircraft);
            }
                break;
                
            case 'c':{
                show_list_reserved_place(aircraft);
            }
                break;
                
            case 'd':{
                int number;
                while(true){
                    char* first_name;
                    char* last_name;
                    printf("Enter the number of place, first name and last name:");
                    scanf("%d %s %s", &number, first_name, last_name);
                    if(book_place(aircraft, number, first_name, last_name))
                        break;
                    else{
                        printf("\nThis place is reserved, try again.\n");
                    }
                }
            }
                break;
                
            case 'e':{
                int number;
                while(true){
                    printf("Enter the number of place or -1 for exit:");
                    scanf("%d", &number);
                    if(remove_place(aircraft, number))
                        break;
                }
            }
                break;
                
            case 'f':{
                if(quit_program(aircraft))
                    return 1;
            }
                break;
                
            default:{
                printf("\nWarning! Unknown command!\n");
            }
                break;
        }
    }
}

int main(){
    FILE *file_in;
    char* file_in_name = "input_6.txt";
    file_in = fopen(file_in_name, "r");
    
    place *aircraft;
    
    if(file_in != NULL){
        aircraft = load_data(file_in);
    }
    
    fclose(file_in);
    if(set_decide(aircraft))
        exit(0);

    
    return 0;
}

