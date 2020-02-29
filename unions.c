#include <stdio.h>
#include <inttypes.h>

union endian {
    uint8_t ei[2];
    uint16_t si;
};

int main(int argc, char *argv[]) {
    union endian myUnion;

    myUnion.si = 0xff;
    myUnion.ei[0] = 0x2;
    myUnion.ei[1] = 0xab;

    printf("16 bit: %04" PRIx16 "\n", myUnion.si);
    printf("8 bit: %02" PRIx8 "%02" PRIx8 "\n", myUnion.ei[0], myUnion.ei[1]);

    return 0;
}
