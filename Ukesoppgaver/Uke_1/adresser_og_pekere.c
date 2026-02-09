#include <stdio.h>

/*

1.  Lag en peker til en peker til et tall. Hent tallet ved å følge pekerne.

2.  Lag en metode som tar inn en peker til en int som argument. 
    Metoden skal følge pekeren, og plusse dette tallet med 1. Sjekk verdien til tallet både før og etter metoden kalles på det.
    
*/ 


//Oppgave 2 

void follow_pointer(int *pointer){
    (*pointer)++;
}

int main(){

    //Oppgave 1 

    int value = 10;
    int *p = &value;
    int **pp = &p;

    printf("Oppgave 1:\nMinneadresse: %p\n",(void**)pp ); //void sier "dette peker på noe, men vi bryrs og ikk om hva". Kan være int, char, osv..
    printf("Verdi: %d\n\n", **pp) ; 

    //Oppgave2
    printf("Oppgave 2:\n");
    printf("Før:    %d\n", value);
    follow_pointer(p);
    printf("Etter:  %d\n", value);
    

}