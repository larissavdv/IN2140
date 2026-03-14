#include <stdio.h>

void hello() {
    printf("Hei igjen!\n");
}

void basic_types_example() {
    int int_example = 24; //unsigned int_example = 24;
    char char_example = 'A';
    float pi = 3.14f;

    // Ulik formatering for ulike typer
    printf("Tall: %d\n", int_example);
    printf("Bokstav: %c\n", char_example);
    printf("Pi: %f\n", pi);
}

void char_example() {
    char test_char = 'a';
    printf("%c\n", test_char);
    printf("%c\n", (test_char + 1));    //Gir b, fordi "chars" egentlig er ASCII verdier 
    printf("%d\n", (int)test_char);

    if ('X' == 88){
        printf("X har int verdi 88!\n");
    }
}

void string_example() {
    
    // c-string
    char my_string2[] = {'H','e','l','l','o','!','\0'};     
    // '\0' er veldig viktig. Det er null-terminatoren, altså tegnet som sier: “Her slutter strengen.”
    // I C må en vanlig streng slutte med '\0'.
    
    printf("Dette er strengen min: %s\n", my_string2);

    
    // C-strings
    char my_string3[] = "Hello again!";
    // Her legger C automatisk til '\0' på slutten for deg.
    
    printf("Dette er den andre strengen min: %s\n", my_string3);
    
    // char-array
    char my_string[] = {'H','e','l','l','o','!'}; 
    /*
    Denne har ikke '\0' på slutten.
    Det betyr at dette bare er et array med tegn, ikke en trygg C-streng.*/

    printf("Dette vil bli rart %s\n", my_string);

    /* 
    %s forventer en null-terminert streng.
    Men my_string har ikke '\0', så printf fortsetter å lese minnet videre helt til den tilfeldigvis finner en null-byte et sted.
    */

    char mitt_array[5];
    mitt_array[0] = 'h';
    mitt_array[1] = 'e';
    mitt_array[2] = 'i';
    mitt_array[3] = '\n';
    mitt_array[4] = '\0'; 

    printf("%s", mitt_array); //fungerer 
}

void pointer_example() {

    int x = 10; 
    int *pointer_x = &x; 
    
    printf("Addressen til x er: %p\n", (void*)pointer_x);
    printf("Verdien paa peker er: %d\n", *pointer_x);

    
    *pointer_x = 88; // Dette betyr: gå til stedet pointer_x peker på, og legg inn verdien 88 der
    //Siden pointer_x peker på x, blir x nå endret til 88.

    //Hva printes?
    printf("%d\n", x); 

    printf("%d\n", *pointer_x); //88

    x = 50; //Nå setter vi x til 50 direkte. Siden pointer_x fortsatt peker til x, vil også *pointer_x nå være 50.

    //Hva printes?
    printf("%d\n", x); //50
   
    printf("%d\n", *pointer_x); //50

    int **peker_til_peker_x = &pointer_x;
    printf("**peker_til_peker_x == %d\n", **peker_til_peker_x);  // ** = dobbel dereferering 
    /*	
    peker_til_peker_x peker til pointer_x
	*peker_til_peker_x gir pointer_x
	**peker_til_peker_x gir verdien som pointer_x peker på, altså verdien til x*/
}

int main() {
    printf("Hello World!\n");
    hello();
    printf("\n");

    printf("BASIC_TYPES\n");
    basic_types_example();
    printf("\n");

    printf("CHAR_EXAMPLE\n");
    char_example();
    printf("\n");

    printf("STRING_EXAMPLE\n");
    string_example();
    printf("\n");

    printf("POINTER_EXAMPLE\n");
    pointer_example();
    printf("\n");
   
    return 0;
}