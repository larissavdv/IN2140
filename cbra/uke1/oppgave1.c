#include <stdio.h>
#include "Myheader.h"
#include <string.h>

//Oppgave 1.1 
//Etter oppgave 1.3 kan vi definere print_hello under main istedenfor, fordi vi kan inkludere headefilen 
void print_hello(){
    printf("Hello world!\n");
}

//oppgave 1.8
void change_int(int *ptr){
        *ptr += 5;
    }

int main (){

    //Oppgave 1.1
    printf("\n\nOppgave 1.1:\n");
    printf("Hello world!\n");
    print_hello();

    printf("\n\nOppgave 1.2:\nUtført");
    printf("\n\nOppgave 1.3:\nUtført");
    //oppgave 1.4
    printf("\n\nOppgave 1.4:\n");

    int numbers[100];

    for (int i = 0; i < 100; i++){
        printf("%d ", numbers[i]);                 //Utskriften  blir helt tilfeldig fordi når arrayet ikke er initialisert, så skrives ut hva som tilfeldigvis er på stacken fra før.
    }

    //Oppgave 1.5
    printf("\n\nOppgave 1.5:\n");
    int numbers2[] = {0,1,2,3,4,5,6,7,8,9};
    numbers2[5] = 1337;
    
    for(int i = 0; i<10; i++){
        printf("%d ", numbers2[i]);
    }

    //Oppgave 1.6
    printf("\n\nOppgave 1.6:\n");

    char navn[] = "Larissa";
    char kopi[100] = {0};

    strcpy(kopi, navn);
    printf("%s\n", navn);
    printf("%s\n", kopi);

    //Oppgave 1.7
    printf("\n\nOppgave 1.7:\n");
    int a = 5;
    int *b = &a;  //& peker til minneadressen 

    printf("a: %d\nb: %d\n", a, *b);  //bruker * for å skrive ut verdien på minneadressen som b peker på kalles "Dereference"


    //oppgave 1.8
    printf("\n\nOppgave 1.8:\n");

    int x = 5; 
    int *p = &x;

    printf("%d\n", x);
    
    change_int(p);

    printf("%d\n", x);



    return 0;
}