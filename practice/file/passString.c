#include <stdio.h>
void displayString(char str[]);

int main()
{
    char str[50];
    printf("Enter string: ");
    fgets(str, sizeof(str), stdin);             
    displayString(str);     // Passing string to a function.    
    return 0;
}

void displayString(char string[])
{
    FILE *fptr;

  fptr = fopen("userFile.txt", "w");

  printf("Enter a string: \n");
  fgets(string, sizeof(string), stdin);
  fprintf(fptr, "%s", string);
  fclose(fptr);
}