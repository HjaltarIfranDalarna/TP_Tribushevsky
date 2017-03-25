

#include <stdio.h>
#include <stdbool.h>

int get_count_word(char *line){
    int i = 0;
    int count_word = 0;
    while(line[i] != '\0'){
        if(i == 0 &&
           line[i] != ' '  &&
           line[i] != '\r' &&
           line[i] != '\t' &&
           line[i] != '\n'){
               ++count_word;
           }
        ++i;
        if(line[i] != ' ' &&
           line[i] != ' ' &&
           line[i] != '\r' &&
           line[i] != '\t' &&
           line[i] != '\n' &&
           line[i - 1] == ' '){
            ++count_word;
        }
    }
    return count_word;
}

int main(){
    
    FILE *file_in;
    FILE *file_out;
    int array_count_word[10];
    char *file_in_name = "input.txt";
    char *file_out_name = "output.txt";
    char result_string[100];

    file_in = fopen(file_in_name,"r");
    
    if(file_in == NULL){
        printf("Не могу открыть файл со входными данными: '%s'",file_in_name);
        return 0;
    }
    

    int i = 0;
    while(fgets(result_string,sizeof(result_string),file_in)){
        array_count_word[i++] = get_count_word(result_string);
    }
    
    fclose(file_in);
    
    file_out = fopen(file_out_name,"w");
    if(file_out == NULL){
        printf("Не удалось создать файл для выходных данных: '%s'",file_out_name);
        return 0;
    }
    int p = i;
    i = 0;
    for(int i = 0; i < p; i++){
        fprintf(file_out, "Строка %d) состоит из из %d слов.\n",i + 1, array_count_word[i]);
    }
    return 0;
}
