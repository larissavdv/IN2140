#include <stdio.h>
#include <string.h>
#include <stdlib.h>

    /*
    * Tar inn en char peker og oppretter en kopi av
    * strengen den peker på. Returnerer en peker
    * til kopien.
    *
    * Parametre: char *original
    * Returnerer: char* - kopi.
    */
char *getCopy(char *original){
    char *copy = malloc(strlen(original) + 1);
    for(int i = 0; i < strlen(original) + 1; i++){
        copy[i] = original[i];
    }
    return copy;
}

int main() {
    char *string = "Alle Cbraer vil ha kopier!";
    char *stringkopi = getCopy(string);
    free(stringkopi);
    return 0;
}
