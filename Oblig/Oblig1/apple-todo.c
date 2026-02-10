/*
 * Add the include files that you need. "man" can help you find them.
 * You will probably need stdio.h for printf and fprintf
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "apple-todo.h"


//Lager en hjelpemetode. Denne er implementert nederst i koden. 
int isApple(char c);

int locateworm( char* buffer )
{
    printf( "This function does nothing yet.\n" );
    return 0;
}

int removeworm( char* apple )
{
    printf( "This function does nothing yet.\n" );
    return 0;
}

int isApple(char c){
    
    //false = 0 
    //true = -1 (eller noe annet)

    char characters[] = "aple";

    int i = 0;

    while(characters[i] != "\0"){
        if(c == characters[i]){
            return -1;
        }
        i++;
    }

    return 0;



}