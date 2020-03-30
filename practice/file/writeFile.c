#include <stdio.h>
#include <stdlib.h>

int main() {
    char sentence[500];

    // creating file pointer to work with files
    FILE *fptr;

    // opening file in writing mode
    fptr = fopen("newfile.txt", "w");

    printf("Enter a sentence please:\n");
    fgets(sentence, sizeof(sentence), stdin);
    fprintf(fptr, "%s", sentence);
    fclose(fptr);

    return 0;
}