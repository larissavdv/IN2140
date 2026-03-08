#include <stdio.h>

int len_str(char *sptr){
    int count = 0;
    while(*sptr){
        count++;
        *sptr++;
    }

    return count;
}

void copy_str(char a1[], char a2[]){

    while(*a1){
        *a2 = *a1;
        a1++;
        a2++;
    }

    a2 = '\0';
    

}

int main (){

    //Oppgave 1.1

    printf("\nOppgave 1.1\n");

    char s[] = "Larissa";
    char *ptr = s;

    int length = len_str(ptr);

    printf("%d\n", length);
    printf("\n");

    printf("\nOppgave 1.2\n");

    char a1[] = "Heisann";
    char a2[10] = "xxxx";

    printf("a2 før kopiering: %s\n", a2);
    copy_str(a1,a2);
    printf("a2 etter kopiering: %s\n", a2);

    printf("\n");

    return 0;
}