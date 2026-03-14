#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Oppgave 1.2.
void fill_array(int *pInt, int size){
    
    for(int i = 0; i<size; i++){
        //pInt[i] = i*10;
        //int *pI = pInt + sizeof(int)*i;
        int *pI = pInt + i;
        *pI = i*10;
    }

}

//oppgave 1.3
void fill_chars(char *array[], int size){
    char *pStr = "my string";

    for(int i = 0; i<size; i++){
        array[i] = malloc(strlen(pStr)+1);
        memcpy(array[i], pStr, strlen(pStr) +1);
    }

}

int main(){

    //Oppgave 1.1

    int i = 10;
    int *pI = &i; 

    printf("Adresse i: %p\n", pI);
    printf("Verdi i: %d\n", *pI);
    

    int *pInt = malloc(sizeof(int)*10);

    printf("Adresse: %p\n", pInt);
    //printf("Verdi: %d\n", *pInt);

    fill_array(pInt, 10);

    for(int i = 0; i<10; i++){
        printf("#%d: %d\n", i+1, pInt[i]);
    }

    free(pInt);

    //Oppgave 1.3

    char *strings[10];

    fill_chars(strings, 10);

    for(int i = 0; i<10; i++){
        printf("String #%d: %s\n", i+1, strings[i]);
        free(strings[i]);
    }



    return 0;

}

