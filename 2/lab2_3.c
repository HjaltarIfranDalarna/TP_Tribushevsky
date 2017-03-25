#include <stdio.h>
#include <string.h>
#define MAXLENGHT 30

int main(){
    char *designation[12] = {"two", "three", "four", "five", "six", "seven", "eight", "nine", "ten", "jack", "queen", "king"};
    char *word;
    int word_size;
    printf("Введите достоинство карты и масть: ");
    scanf("%s", word);
    for(int i = 0; i < MAXLENGHT; i++){
        if(word[i] == '\0')
            break;
        word_size++;
    }
        int result = 0;
        for(int i = 0; i < 12; i++){
            if(!strcmp(word, designation[i]))
            {
                if(i < 10)
                    result = i + 2;
                else
                    result = 10;
                break;
            }
        }
    
        if(!strcmp(word, "ace")){
            result = 11;
        }
    
        if(!strcmp(word, "deuce")){
            result = 2;
        }
    
        if(!strcmp(word, "trey")){
            result = 3;
        }
        
        printf("Результат: %i\n", result);
    
    return 0;
}
