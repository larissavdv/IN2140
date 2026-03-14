#include <stdio.h>

int main(){
    int buf_size = 255;
    char buf[buf_size];

    FILE *file = fopen("dikt.txt", "r+");
    if(file == NULL){
        printf("Kunne ikke åpne fil");
        return -1;
    }

    int wr = fread(buf, sizeof(char), buf_size-1, file);
    if(wr == 0){
        printf("Kunne ikke lese innhold");
        fclose(file);
        return -1;
    }

    buf[wr] = '\0';

    printf("%s\n", buf);

    fseek(file, 0, SEEK_SET);

    for(int i = wr-1; i>=0; i--){
        fwrite(&buf[i], sizeof(char), 1, file);
        printf("%c", buf[i]);
    }
    printf("\n");



    fclose(file);
    return 0;

}