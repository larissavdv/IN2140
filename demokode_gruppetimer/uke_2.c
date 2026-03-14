#include <stdio.h>
#include <stdlib.h>  
#include <string.h>  

char *stackExample() { 

    char streng[] = "Denne strengen blir lagt på stacken";
    printf("String: %s\n", streng);

    return streng;
}

char *heapExample() {
    char *streng = (char*)malloc(10 * sizeof(char));   // Malloc returnerer void*, og (char*) caster denne returverdienchar*, men det gjøres automatisk når vi tilegner den en variabel av typen char*. Det er altså ikke nødvendig.
    if (streng == NULL) {                             
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }

    strcpy(streng, "heap!");

    return streng;
}

void sizeExample() {
    int x = 10;
    long y = 10;
    char z = 'Y';
    char str[] = "Dette er en streng!";
    int numbers[] = {1, 2, 3, 4, 5};

    printf("Størrelse på int x: %zu bytes\n", sizeof(x));
    printf("Størrelse på long y: %zu bytes\n", sizeof(y));
    printf("Størrelse på char z: %zu bytes\n", sizeof(z));

    printf("Størrelse på str[]: %zu bytes\n", sizeof(str));     // '/0'telles også med her, så svaret bli 20"
    printf("Lengden av str: %d\n", strlen(str));                // Her telles '/0' IKKE med, så her blir svaret 19 
 
    printf("Størrelse på numbers[]: %zu bytes\n", sizeof(numbers)); // 5 tall x 4 bytes = 20 bytes 

    printf("Antall elementer i numbers: %zu\n",
           sizeof(numbers) / sizeof(numbers[0]));  //tilsvarer "length"

}

int main() {

    char *stackString = stackExample();  
    printf("Strengen fra stackExample: %s\n", stackString);


    char *heapString = heapExample();   
    printf("Dette er strengen som ligger på heapen: %s\n", heapString);

    free(heapString);
    
    sizeExample();
    return 0;
}