#include <stdio.h>

struct zebra {
    unsigned char number_of_legs;
};

int main(){
    int b_size = 255;
    char buf[b_size];

    FILE *f = fopen("zebra_struct", "rb");
    if(f == NULL){
        printf("Kunne ikke åpne fil\n");
        return 1;
    }

    struct zebra zebraer[4]; //holder 4 zebra structer 

    fread(zebraer, sizeof(struct zebra), 4, f);

    for(int i = 0; i<4; i++){
        zebraer[i].number_of_legs *= 10;
    }

    fclose(f);

    FILE *fout = fopen("zebra_out", "wb");
    if(fout == NULL){
        printf("Kunne ikke åpne fil\n");
        return 1;
    }

    fwrite(zebraer, sizeof(struct zebra), 4, fout);

    fclose(fout);







    return 0;
}