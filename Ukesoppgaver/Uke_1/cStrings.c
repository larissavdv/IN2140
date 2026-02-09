#include <stdio.h>

/*
1.  Cellene i et array ligger etter hverandre i minnet. Lag en streng (char array) "abcde". 
    Kan du gjøre om strengen til "bcde" kun ved å endre verdien til pekeren?

2.  Lag en streng, bruk en for-løkke til å skrive ut denne baklengs.

*/


int main(){

    //Oppgave 1

    char myString[] = "abcde";

    char *ptr = myString;

    printf("%s\n", ptr); //printer strengen (kan også gjøre ("%s, myString"))
    printf("%p\n", ptr); //printer adressen til strengen 
    printf("%c\n", *ptr); // printer akkurat det elementet (char) som ptr peker på nå 
    printf("\n");

    ptr++;
    printf("%s\n", ptr);
    printf("%p\n", ptr);
    printf("%c\n", *ptr);

    //myString er uforandret, vi har kun endret på pekeren. 

    printf("\n%s\n\n", myString);


    //Oppgave 2
    printf("**Oppgave 2**\n");

    char hello[] = "hello";
    int size = sizeof(hello);
    printf("%s\n", hello);
    printf("%d\n", size);
    
    for(int i = (sizeof(hello)-2); i>=0; i--){
        printf("%c", hello[i]);
        
    }
    printf("\n");
    return 0;
}

