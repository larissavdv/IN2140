#include <stdio.h>

void print_size(char array[]){
    printf("%ld\n", sizeof(array));
}

int main(){

    //Oppgave 5.1

    printf("\nOppgave 5.1\n");
    float f = 5.6;
    float *fpkr = &f;
    printf("%ld\n", sizeof(f));
    printf("%ld\n", sizeof(fpkr));
    printf("\n");

    printf("\nOppgave 5.2\n");

    char array[50];
    printf("%ld\n", sizeof(array));
    print_size(array);

    /*
    Første utskrift blir 50, siden kompilatoren vet størrelsen på arrayet (det er opprettet i samme skop med en konstant lengde).
    Andre utskrift blir 8, ettersom kompilatoren ikke vet størrelsen på arrayet som blir sendt inn. 
    Dermed gir den ut størrelsen av datatypen istedenfor, nemlig størrelsen av en char * som er 8. 
    */




    return 0;
}