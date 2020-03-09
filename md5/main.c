#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <md5.c>

#pragma warning(disable:4996)

#define MAX_INPUT_LENGTH 2000

int main() {
	char *fileName = "fileToHash.txt";
	char userInput[2000];
	char line = NULL;
	int i;
	int option;
	bool keepAlive = true;

	while (keepAlive) {
		printf("Enter 1 to hash an input,\nEnter 2 to hash the contents of a file or\nEnter -1 to exit: ");
		scanf("%d", &option);

		switch (option) {
		case 1:
			printf("\nInput hash\n=========\n");

			printf("Enter a string to hash: ");
			scanf("%s", userInput);

			printf(userInput);
			break;
		case 2:
			printf("\nFile hash\n==========\n");

			printf("Reading file...");

			// Need to change to fopen_s
			FILE *file = fopen(fileName, "r");

			if (file == NULL) {
				printf("ERROR: %s cannot be opened", fileName);

				getch();

				exit(0);
			}
			else {
				printf("Reading file...\n\n");

				line = fgetc(file);

				while (line != EOF) {
					printf("%c", line);

					line = fgetc(file);
				}
			}
			printf("\n\nFinished reading file");

			fclose(file);
			break;
		case -1:
			printf("Exiting the program...");

			getch();

			exit(0);
		default:
			printf("ERROR: Invalid input. Please enter either 1 or 2 when prompted");
			break;
		}
	}

	getch();

	return 0;
}
