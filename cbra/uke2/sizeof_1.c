#include <stdio.h>

struct dummy {
    char c[23];
};

int main() {

    char *streng = "Cbra hver fredag! :-)))";

    for (int i = 0; i < sizeof(struct dummy); i++) {
        printf("%c", streng[i]);
    }
    printf("\n");
    return 0;
}
