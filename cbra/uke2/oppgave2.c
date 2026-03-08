#include <stdio.h>
#include <string.h>

//oppgave 2.1 

// int main(int argc, char const *argv[]){

//     if(argc < 4) {  //argv[0] er selve programkallet, så denne må man også "telle" med!
//         printf("Ikke nok argumenter, du å oppgi minst 3!\n");
//         return 1;
//     }

//     for(int i = 1; i<argc; i++){
//         printf("arg%d: %s , ", i,argv[i]);
//     }

//     printf("\n");

//     return 0;
// }


//oppgave 2.2

int main(int argc, char const *argv[]){     //argv er en liste av pekere. Hver peker peker på en streng 

    if(argc != 2){
        printf("Angi nøyaktig 1 argument i tillegg til programnavn\n");
        printf("Du oppga %d argumenter \n", --argc);
        return 1;
    }

    //char const *argument = argv[1];

    if(strlen(argv[1]) > 1){
        printf("%s er ikke en bokstav \n", argv[1]);
        return 1;
    }

    int next = *argv[1]; //Henter strengen som argv[1] peker på (deseralisering med *) 
    next++;
  
    

    printf("-> %c \n", next);


    return 0;
}