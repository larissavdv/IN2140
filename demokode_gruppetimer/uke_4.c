#include <stdio.h>
#include <string.h>

/*
struct student { 
    int id; 
    char navn[100]; 
    int alder; 
};
*/

typedef struct student { //typedef gjør at vi kan skrive: "student stud1" i stedet for "struct student stud1"
    int id;
    char navn[100];
    int alder;
} student;

void printStudent(student *stu_ptr) {
    printf("ID: %d | Navn: %s | Alder: %d\n", stu_ptr->id, stu_ptr->navn, stu_ptr->alder); //-> betyr: gå til structen pekeren peker på, og hent feltet. Det er det samme som: (*stu_ptr).id

}

void bursdag(student *stu_ptr) {
    (*stu_ptr).alder += 1;

    // alternativ: stu_ptr->alder += 1;
}

int main(void) {
    student stu1;
    stu1.id = 1234;
    strcpy(stu1.navn, "Student 1");  //Vi bruker strcpy fordi navn er et char-array.
    stu1.alder = 24;
    
    printf("stu1 sin alder: %d\n", stu1.alder);

    printf("Størrelsen på struct student: %lu\n", sizeof(stu1));

    student stu2 = {5678, "Student 2", 30};

    //Structs med pekere
    student *stu_ptr = &stu1;

    printStudent(stu_ptr);   
    bursdag(stu_ptr);
    printf("Etter bursdag:\n");
    printStudent(stu_ptr);

    return 0;
}