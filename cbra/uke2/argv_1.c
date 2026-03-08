#include <stdio.h>

int main(int argc, char const *argv[]) {
    char *streng = "Cbra!";

    for (int i = 0; i < argc; i++) {
        printf("%c", streng[i]);
    }
    printf("\n");
    return 0;
}
