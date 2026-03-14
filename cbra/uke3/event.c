#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct event {
    char *navn;
    int prioritet;
    char beskrivelse[20];
};

struct event* create_event(char *navn, int prioritet, char *desc){
    struct event *event = malloc(sizeof(struct event));

    event->prioritet = prioritet;
    event->navn = navn;

    strncpy(event->beskrivelse, desc, 19);

    return event;
}

void print_event(struct event *e){
    printf("*****");
    printf("\n");
    printf("Navn: %s\n", e->navn);
    printf("Prioritet: %d\n", e->prioritet);
    printf("Beskrivelse: %s\n", e->beskrivelse);
    printf("*****");
    printf("\n");
}

int main(){

    struct event *e = create_event("30-årsdag", 1, "Feire bursdag");
    print_event(e);
    free(e);

    return 0; 
}