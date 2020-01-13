#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#pragma warning(disable:4996)

int main() {
	char fileName[100] = "fileToHash.txt";
	char line;
	int i, bytes;

	// Need to change to fopen_s
	FILE *file = fopen(fileName, "r");

	if (file == NULL) {
		printf("ERROR: %s cannot be opened", fileName);
		exit(0);
	} 
	line = fgetc(file);

	while (line != EOF) {
		printf("%c", line);

		line = fgetc(file);
	}

	fclose(file);

	getch();

	return 0;
}
