#include <stdio.h>
#include <string.h>

//oppgave 4.1
struct tall {
    int tall;
};

//oppgave 4.2
struct person{
    int alder;
    char navn[50];
    char *navn2;
};

//Oppgave 4.4 
struct bil {
    int nummer;
};

void fyll_biler(struct bil biler[], int str){
    for(int i = 0; i < str; i++){
        //struct bil b;     Unødvendig! Det er allerede opprettet bilobjecter str-ganger i arrayet 
        //b.nummer = i+1;
        biler[i].nummer = i+1;
    }

}

int main(){

    printf("\nOppgave 4.1\n");
    struct tall tallet;
    tallet.tall = 5; 
    printf("Tallet er %d\n", tallet.tall);
    printf("\n");


    printf("\nOppgave 4.2\n");
    struct person p1;
    p1.alder = 32;
    strcpy(p1.navn, "Larissa");  //Må kopiere over en og en char
    p1.navn2 = "vdv";           //Kan sette direkte 

    printf("%s %s er %d år gammel\n", p1.navn, p1.navn2, p1.alder);
    printf("\n");



    printf("\nOppgave 4.3\n");
    
    struct bil biler[10];

    fyll_biler(biler, 10);

    for(int i = 0; i<10; i++){
        printf("Bil%d, " , biler[i].nummer);
    }

    printf("\n");




    return 0;
}