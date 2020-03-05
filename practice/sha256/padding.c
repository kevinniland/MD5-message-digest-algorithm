// Padding

#include <stdio.h>
#include <inttypes.h>

// 
union block{
    uint64_t sixfour[8];
    uint32_t threetwo[16];
	uint8_t eight[64];
};

//
uint64_t nozerosbytes(uint64_t nobits){
    uint64_t result = 512ULL - (nobits % 512ULL);

    if (result < 65) {
        result += 512;
    }

    result -= 72;

    return (result / 8ULL);
}


// 
int main(int argc, char *argv[])
{
    // if(argc != 2){
    //     printf("Error: expected single filename as argument\n");
    //     return 1;
    // }

    // FILE *infile = fopen(argv[1], "rb");
    // // Test Files
    // FILE *infile = fopen("../../README.md", "rb");

    FILE *infile = fopen("padding.c", "rb");

    if (!infile) {
        printf("ERROR: Unable to open file %s\n", argv[1]);
		
        return 1;
    }

	union block M;
	uint64_t nobits;
    uint8_t b;
    uint8_t i;

    for(nobits = 0; fread(&M.eight[i], 1, 1, infile) == 1; nobits += 8) {
        printf("%02" PRIx8 " ", M.eight[i]);
    }

	// Bits 1000 0000
    printf("%02" PRIx8, 0x80);

    for(uint64_t i = nozerosbytes(nobits); i > 0; i--) {
        printf("%02" PRIx8, 0x00);
    }

    printf("%016" PRIx16 "\n", nobits);

    fclose(infile);

    return 0;
}