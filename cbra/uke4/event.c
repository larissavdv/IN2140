#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct event {
    char navn[5];
    char beskrivelse[50];
}__attribute__((packed));

int main(void) {
    char *beskrivelse = "Her kan man lære om null-bytes og stuff!";
    char *navn = "Cbra";

    struct event ev;
    strcpy(ev.beskrivelse, beskrivelse);
    strcpy(ev.navn, navn);

    printf("Navn: %s, beskrivelse: %s\n", ev.navn, ev.beskrivelse);

    return 0;
}