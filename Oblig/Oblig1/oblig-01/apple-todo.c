/*
 * Add the include files that you need. "man" can help you find them.
 * You will probably need stdio.h for printf and fprintf
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "apple-todo.h"

int locateworm( char* buffer )
{
    int i = 0;
    while(buffer[i] != '\0'){
        if(buffer[i] == 'w') return i;
        i++;
    }

    return -1;
}

int removeworm( char* apple )
{
    int i = locateworm(apple);

    if(i == -1) return 0;
    
    int count = 0;

    while(apple[i] != 'a'){
        apple[i] = ' ';
        count++;
        i++;
    }

    return count;

}

