// Padding for SHA256

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#pragma warning(disable:4996)

union block {
    uint64_t sixfour[8];
    uint32_t threetwo[16];
    uint8_t eight[64];
};

uint64_t nozerobytes(uint64_t nobits) {
	uint64_t result = 512ULL - (nobits % 512ULL);

	if (result < 65) {
		result += 512;
	}

	result -= 72;

	return (result / 8ULL);
}

int main(int argc, char *argv) {
	if (argc != 2) {
		printf("ERROR: Expected filename as argument");

		return 1;
	}

	FILE *file = fopen(argv[1], "rb");

	if (!file) {
		printf("ERROR: Unable to open file");

		return 1;
	}

    union block M;
	// uint8_t b;
    uint8_t i;
	uint64_t nobits;

	for (nobits = 0, i = 0; fread(&M.eight[i], 1, 1, file) == 1; nobits += 8) {
		printf("%02x" PRIx8, M.eight[i]);
	}

	printf("%02" PRIx8, 0x80); // Bits in: 1000 0000

	for (uint64_t i = nozerobytes(nobits); i > 0; i--) {
		printf("%02" PRIx8, 0x00);
	}

	printf("%016" PRIx64 "\n", nobits);

	printf("\n");

	fclose(file);

	return 0;
}
