#include <stdio.h>

/* 
Oppgave 3.1

int *p;
*p = 3;

Kodesnutten over vil føre til segmentation fault. Hvorfor det?

p er en peker og kan ikke initialiseres?

Fasit: 

Grunnen til at dette gir segmentation fault er fordi variabelen p er en peker. 
Altså er det er variabel som har plass til en peker, men denne settes aldri. 
Dermed er verdien til p udefinert (mest sannsynlig NULL, eller et annet tilfeldig tall basert på hva som lå på stacken fra før), 
og når vi da prøver å følge pekeren vil vi få segmentation fault. Prøv å skriv ut p og se hva du får!
*/


void min_funk(int *ptr, int i){
    *ptr = i;
}

int main(){

    int x= 5;
    int *xptr = &x;

    int i = 3;

    min_funk(xptr, i);

    printf("%d\n", x);

    return 0;
}