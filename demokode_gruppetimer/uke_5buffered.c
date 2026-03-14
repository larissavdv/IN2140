#define BUFSIZE 255
#include <stdio.h> 
#include <string.h>

int main(void) {
    const char *msg = "Dette er gruppe 2!";
    size_t msg_len = strlen(msg);               //Teller ikke med '/n'

     char buf[BUFSIZE + 1];                      //Bufferet holder dataen som vi leser fra fil. Må ha med +1 fordi vi må ha med '/0'

    FILE *file = fopen("buffered.txt", "w+"); //åpner filen "buffered.txt". "w+" betyr: 1. skriv til fil 2. les fra fil 3. filen opprettes eller overskrives 
    if (file == NULL) {
        perror("fopen");
        return -1;
    }


    size_t wc = fwrite(msg, sizeof(char), msg_len, file);   //skriver til filen. Paramerne: 1. data som skal skrives(msg) 2. størrelse per element (sizeof(char)) 3. antall elementer (msg_len) 4. filen det skal skrives til 
    if (wc != msg_len) {                                    // wc blir antall tegn som faktisk ble skrevet.
        perror("fwrite");       
        fclose(file);
        return -1;
    }

    if (fseek(file, 0, SEEK_SET) != 0) {  //Etter skriving står filpekeren på slutten av filen. Denne linjen flytter den tilbake til starten, slik at vi kan lese.
        perror("fseek");                    //Det betyr altså: gå til byte 0 i filen (starten). Funksjonen returnerer 0 hvis ok, noe annet hvis det feilet. 
        fclose(file);
        return -1;
    }

    size_t rc = fread(buf, sizeof(char), BUFSIZE, file);    //Dette leser data fra filen og legger det i buf
    if (rc == 0) {                                          // rc = hvor mange bytes som faktisk ble lest. 
        perror("fread");
        fclose(file);
        return -1;
    }

    buf[rc] = '\0';                                         //fread legger ikke automatisk til '\0'.


    printf("I bufferet ligger det: %s\n", buf);

    fclose(file); // Viktig: Flusher buffer til fil
    return 0;
}