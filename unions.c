#include <stdio.h>
#include <inttypes.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("ERROR: Expected single filename as argument.\n");

        return 1;
    }

    FILE *file = fopen(argv[1], "rb");

    if (!file) {
        printf("ERROR: Couldn't open file %s.\n", argv[1]);

        return 1;
    }

    union block M;
    uint64_t nobits;
    uint8_t i;

    for (nobits = 0; i = 0; fread(&M[i], 1, 1, file) == 1; nobits += 8) {
        printf("%02", PRIx8, M[i]);
    }

    printf("%02", PRIx8, 0x80);
}
