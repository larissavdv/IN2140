/*
 * Add the include files that you need. "man" can help you find them.
 * You will probably need stdio.h for printf and fprintf
 */
#include <stdio.h>
#include "vowelshift-todo.h"
#include <string.h> //importert for å kunne bruke strlen (jeg antok at dette var ok siden main også bruker denne)

//Jeg lager en hjelpemetode for å sjekke om en gitt karakter er en vokal.
//Denne implementeres under vowelshift() metoden 
int isVowel(char x);


void vowelshift( char* buffer, char repl )
{
    int size = strlen(buffer);

    for(int i = 0; i<(size); i++){
        if(isVowel(buffer[i])) buffer[i] = repl;
        
    }

}


int isVowel(char x){
    char vowels[] = {'a', 'e', 'i', 'o', 'u'};
    int length = sizeof(vowels) / sizeof(vowels[0]);

    for(int i = 0; i<length; i++){
        if(x == vowels[i]) return 1;
    }

    return 0;

}

