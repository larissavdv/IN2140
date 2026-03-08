#include <stdio.h>


void print_array(int a[], int size){
    for(int i = 0; i<size; i++){
        printf("%d ", a[i]);
    }

}


int main(){

    //oppgave 2.1
    printf("\nOppgave 2.1\n");
    int array[] = {1,2,3,4,5,6,7,8,9,10};
    int size = sizeof(array)/sizeof(array[0]);

    print_array(array, size);
    printf("\n");

    //Oppgave 2.2
    printf("\nOppgave 2.2\n");
    char c[] = "Larissa";  //// samme som {'L', 'a', 'r', 'i', 's','s','a', '\0'}
    char *cptr = c;

    while(*cptr != '\0'){       //Kunne bare skrevet while(*cptr) fordi \0 er det samme som 0 som er det samme som false  
        printf("%c", *cptr);
        cptr += 1;
    }
    printf("\n");

    //Oppgave 2.3
    printf("\nOppgave 2.3\n");

    char c2[] = "Heisann";
    char needle = 'a';
    char *c2ptr = c2;

    while(*c2ptr){
        if (*c2ptr == needle){
            printf("Fant nålen!\n");
            break;
        }
        *c2ptr += 1;
    }


    printf("\n");


    return 0;

}
