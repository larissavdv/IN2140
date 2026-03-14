#include <stdio.h>
#include <string.h>

int main(){

    //Oppgave1.1
    char buffer[255];

    FILE *file1 = fopen("text.txt", "r");
    if(file1 == NULL){
        perror("Could not open file\n");
        return -1;
    }

    int er = fread(&buffer[0], sizeof(char),1, file1);
    int i = 1;

    while(er){
        er = fread(&buffer[i], sizeof(char), 1, file1);
        i++;
    }

    buffer[i] = '\0';
    printf("%s\n", buffer);

    fclose(file1);


    //oppgave 1.2
    const char *name = "Larissa";
    int strl = strlen(name);

    FILE *file2 = fopen("oppg_1_2", "w");
    if (file2 == NULL){
        perror("Kunne ikke åpne filen");
        return -1;
    }

    char *pos = strstr(buffer, "name"); //Gir minneområdet til "name" i buffer 

    if(pos != NULL){
        *pos = '\0';  //erstatter "n" med "\0", så nå tror C at programemt slutter etter Hello 
        fprintf(file2, "%s%s%s", buffer, name, pos+4);
    }

    fclose(file2);
    return 0;
}