#include <stdio.h>

int main(){
    //Oppgave 1
    int firstInt = 10;
    float firstFloat = 10.1;

    int addResult = firstFloat + firstInt;  //Dette er lov, men vi får 20 som svar, ikke 20.1. Da må addResult også være float.

    printf("%i\n", addResult);

    //Oppgave 2
    float secondFloat = 2.9;
    int toFloat = secondFloat;  //Dette blir 2 

    printf("%d\n", toFloat); 
    
    //Oppgave 3 

    for(int i = 65; i<=90; i++){
        printf("%c ", i);
    }
    printf("\n");

    //Oppgave 4 
    /*Lag et predikat "er_partall(int x)", som tar et tall som input, og returnerer 1 om det er er et partall, og 0 om det er et oddetall. 
    lag en if-else, som printer "oddetall!", eller "partall", basert på resultatet, og test det på ulike tall.*/ 
    int er_partall(int x){
        return x%2 == 0;
    }

    int x = 15;

    if(er_partall(x)){
        printf("%d er et partall!\n", x);
    } else{
        printf("%d er et oddetall!\n", x);
    }


    return 0;
}