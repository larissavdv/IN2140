/* Eksempel lånt av gruppelærer gruppe 3, Birk */
#include <stdio.h>
#include <stdlib.h>

typedef struct Student {
    int id;
    char navn[50];
    char harFadder;
    struct Student *fadder;
} Student;

int write_student(Student *student, FILE *file){ //Her får funksjonen en peker til en eksisterende Student-struct.

    //Disse if-sjekkene er der for å kontrollere at skrivingen til fil faktisk lykkes.
    //fwrite returnerer hvor mange elementer som faktisk ble skrevet (fwrite(data, størrelse, antall, fil))

    if (
        fwrite(&student->id, sizeof(int), 1, file) != 1 ||          //Vi prøver å skrive 1 int, så vi forventer 1
        fwrite(student->navn, sizeof(char), 50, file) != 50 ||      //Vi prøver å skrive 50 chars, så vi forventer 50. Her kan vi droppe & fordi navn er et array, og et array blir automatisk til en peker til første element
        fwrite(&student->harFadder, sizeof(char), 1, file) != 1     //Vi prøver å skrive 1 char, så vi forventer 1 
    ) {
        perror("fwrite");
        return -1;
    }

    if (student->harFadder) {
        if (write_student(student->fadder, file) < 0) { //Hvis en student har en fadder, så kalles funksjonen rekursivt på fadderen. Slik får vi alle fadderne (lenket liste ish)
            return -1;
        }
    }
    return 0;
}

int read_student(Student *student, FILE *file){ //student er en peker til en Student-struct som vi skal fylle med data og file er filen vi leser fra (peker til)
    if (
        fread(&student->id, sizeof(int), 1, file) != 1 ||
        fread(student->navn, sizeof(char), 50, file) != 50 ||
        fread(&student->harFadder, sizeof(char), 1, file) != 1
    ) {
        perror("fread");
        return -1;
    }

    if (student->harFadder) {                       //Hvis studenten har en fadder, så må vi lese inn en student til 
        student->fadder = malloc(sizeof(Student));  //Her reserverer vi minne til en ny Student på heapen fordi fadder er en peker. Den må peke til et gyldig område i minnet før vi kan lese data inn der. Uten malloc hadde student->fadder bare vært en tom/ubestemt peker.
        if (student->fadder == NULL) {              
            perror("malloc");
            return -1;
        }


        if (read_student(student->fadder, file) < 0) { //hvis read_student feiler (feil = returnerer -1)
            free(student->fadder);   //frigjør minnet vi allokerte 
            student->fadder = NULL; //setter pekeren til NULL 
            return -1;
        }
    } else {
        student->fadder = NULL;     //Hvis student ikke har en fadder, så settes pekeren til NULL 
    }

    return 0;
}

void free_students(Student *student) {
    if (student->fadder != NULL) {
        free_students(student->fadder);  //rekursivt kall for å frigjøre alle studenter
        free(student->fadder);
        student->fadder = NULL;
    }
}

int main() {
    FILE *file;

    Student stud_u_fadder = {2, "Student 1", 0, NULL};
    Student stud_m_fadder = {1, "Student 2", 1, &stud_u_fadder};  
    
    //SKRIV TIL FIL 

    file = fopen("student", "wb");  // wb: åpner filen for skriving i binærmodus. Data skrives byte-for-byte slik de ligger i minnet (brukes med fwrite).
    if (file == NULL) {
        perror("Failed to open file for writing");
        return 1;
    }
    
    if (write_student(&stud_m_fadder, file) < 0){
        fclose(file);
        return -1;
    }

    fclose(file);

    //LES FRA FIL

    Student stud_in;   //Tom student struct som vi skal fylle med data. Det er en "mottaker" for når vi leser fra filen

    file = fopen("student", "rb"); // rb: åpner filen for lesing i binærmodus. Data leses byte-for-byte fra filen uten at systemet endrer innholdet.
    if (file == NULL) {
        perror("Failed to open file for reading");
        return 1;
    }

    if (read_student(&stud_in, file) < 0){
        fclose(file);
        return -1;
    }

    fclose(file);

    printf("Lest fra fil:\n");
    printf("ID: %d\n", stud_in.id);
    printf("Navn: %s\n", stud_in.navn);
    printf("HarFadder: %d\n", stud_in.harFadder);

    if (stud_in.fadder != NULL)
        printf("Fadder: %s\n", stud_in.fadder->navn);
    else
        printf("Fadder: (ingen)\n");

    free_students(&stud_in);

    return 0;
}