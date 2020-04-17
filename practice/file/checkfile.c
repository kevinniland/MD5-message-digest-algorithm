#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *words[] = { "cat", "dog", "snake", "bee" };

int main()
{
  char line[200];   // maximum line size is 200
  size_t len = 0;

  FILE *f;
  f = fopen("test.txt", "r");

  if (f == NULL)
  {
    printf("Can't open file\n");
    return 1;
  }

  int line_no = 0;
  while (fgets(line, sizeof line, f))
  {
    ++line_no;

    // (sizeof words)/sizeof *words is the the number of words in the words array
    for (int i = 0; i < (sizeof words)/sizeof *words; i++) 
    {
      if (strstr(line, words[i]) != NULL)
      {
        printf("found %s in line %d\n", words[i], line_no);
      }
    }
  }

  fclose(f);
}