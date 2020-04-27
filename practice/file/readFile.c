#include <stdio.h>

#define FNSZ 128

FILE *read_open_file (char *fn);

int main (void) {
    char file_name[FNSZ] = {0};
    FILE *fp = NULL;

    printf ("Please enter file name: ");
    if (scanf ("%127s%*c", file_name) != 1) {
        fprintf (stderr, "error: file_name entry failed.\n");
        return 1;
    }

    /* you must assign return to FILE* */
    fp = read_open_file (file_name);
    if (!fp) {
        fprintf (stderr, "error: file open failed '%s'\n", file_name);
        return 1;
    }

    fclose (fp);

    return 0;
}

FILE *read_open_file (char *fn)
{
    FILE *ifp;
    printf ("File name %s\n", fn);

    ifp = fopen (fn, "r");

    if (ifp == NULL)
        fprintf(stderr,"File %s does not exist", fn);

    return ifp;
}