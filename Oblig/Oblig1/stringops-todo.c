/*
 * Add the include files that you need. "man" can help you find them.
 * You will probably need stdio.h for printf and fprintf
 */
#include <stdio.h>
#include <string.h>
#include "stringops-todo.h"
#include <ctype.h> //For å kunne bruke tolower

int   stringsum( char *s )
{
    int size = strlen(s);
    int sum = 0;

    for (int i = 0; i<size; i++){
        if(s[i] == ' '){
            continue;                           //Ignorerer mellomrom, går til neste iterasjon av for-løkken
        } else if (isdigit(s[i])) return -1;    //Sjekker om s[i] er et tall, og returner -1 med en gang hvis det er sant
                   
        
        char lower = tolower(s[i]);             //Oppretter en variabel som er lowercase av bokstaven 
        int value = lower - 'a' + 1;            //'a' tilsvarer ASCII verdien til bokstaven a. Ved å trekke denne fra ascii verdien til karakteren s[i], får vi den alfabetiske verdien.  
                                                //Dette er fordi bokstavene ligger numerisk sortert i ascii tabellen også 
        sum += value;
    }

    return sum;
}


int   distance_between( char *s, char c )
{
    int size = strlen(s);

    int firstPos = -1;
    int lastPos = -2;

    for(int i = 0; i<size; i++){
        if(s[i] == c){
            if (firstPos == -1){
                firstPos = i;
            }
        } else{
            lastPos = i;
        }
    }

    if(firstPos == -1 ){
        return -1;
    }else if (lastPos == -2){
        return 0;
    } else {
        return lastPos - firstPos;
    }

    
}

char* string_between( char *s, char c )
{
    printf( "%s does nothing yet\n", __FUNCTION__ );
    return NULL;
}

int  stringsum2( char *s, int *res )
{
    printf( "%s does nothing yet\n", __FUNCTION__ );
    return 0;
}


