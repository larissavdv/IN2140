#include <stdio.h>
#include <stdlib.h>

//Oppgave 1.2.

void fill_array(int array, int size){

    for(int i = 0; i < size; i++){
        int r = rand() % 10 +1;
        array = r;
        array++;

    }
    
    
    
    
    
    

}

int main(){

    //Oppgave 1.1



    int *intptr = malloc(sizeof(int)*10);
    printf("TEST\n%d\n", *intptr);

    fill_array(*intptr, 10);

    for(int i = 0; i<10; i++){
        printf("%d\n", *intptr);
    }

    free(intptr);


    return 0;

}

