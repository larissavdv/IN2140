#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *getAllocatedString(char *s){
    char *str = malloc(strlen(s) + 1);
    strcpy(str, s);
    return str;
}

int main(int argc, char const *argv[]) {
    char *s = getAllocatedString("Hello, World!");

    int lengde = strlen(s);
    for (int i = 0; i < lengde; i++) {
        printf("%c", *(s++));
    }
    printf("\n");

    free(s); //s peker nå på \0 byten! Kan lage en kopi av s og bruke den i løkken istedenfor

    return 0;
}