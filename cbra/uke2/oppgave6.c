#include <stdio.h>
#include <stdlib.h> 
#include <stdlib.h>

//oppgave 6.6

int * funk(int arr[], int size){
    int *nytt = malloc(sizeof(int) * size);

    for(int i = 0; i<size; i++){
        nytt[i] = arr[i];
    }

    return nytt;
}

int main(){

    //Oppgave 6.2
    int *x;   //Peker ikke på noe ennå, har bare en tilfeldig adresse 

    x = malloc(sizeof(int));   //x peker nå på et område satt av til å inneholde en int (4 bytes). Vi skal ikke hente selve verdien til x, dropper derfor *
    *x = 10;
    free(x);


    //Oppgave 6.4

    char *str = malloc(4);

    str[0] = 97;
    str[1] = 98;
    str[2] = 99;
    str[3] = 0;

    printf("%s\n", str);
    free(str);

    //Oppgave 6.5
    int *pkr = malloc(10*sizeof(int));

    for(int i = 0; i < 10; i++){
        pkr[i] = i+1;
        printf("%d ", pkr[i]);

    }

    free(pkr);
    printf("\n");

    //oppgave 6.6 

    int arr[10] = {1,2,3,4,5,6,7,8,9,10};
    int *ny = funk(arr, 10);

    for(int i = 0; i<10; i++){
        printf("%d \n", ny[i]);
    }

    free(ny);



    return 0;


}