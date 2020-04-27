#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char **argv) {

if(2 != argc)
{
    printf("Directory name missing!\n");
    return 0;
}
DIR *dp = NULL;
struct dirent *dptr = NULL;
FILE *input_file;
FILE *next_file;
char c,cwd[256]; //created new character array

if (NULL == (dp = opendir(argv[1])))
{
    printf("Cannot open the given directory %s", argv[1]);
    exit(1);
}
else
{
    while((dptr = readdir(dp))!=NULL)
    {
        if(!strcmp (dptr->d_name, "."))
        {
            continue;
        }
        if(!strcmp (dptr->d_name, ".."))
        {
            continue;
        }
    if(getcwd(cwd, sizeof(cwd)) == NULL) //this is important
    {
        printf("No such file or directory");
        continue;
    }
    if(dptr->d_name[0]=='.')  //Files never begin with '.' 
        continue;
    strcat(cwd,"/");   //For windows "\"
    strcat(cwd,argv[1]);
    strcat(cwd,"/");   //For windows "\"
    strcat(cwd,dptr->d_name);
    printf("%s",cwd);  //check for working directory
        input_file = fopen(cwd, "r");
    printf("%s\n",dptr->d_name); //validation check
        if(input_file == NULL)
        {
            perror("Cannot open file or no file exists\n");
            fclose(input_file);
            return 0;
        }
        c = fgetc(input_file);
        while(c != EOF)
        {
            printf("%d", c);
        }
        printf("%s\n", dptr->d_name);
    }
    closedir(dp);
}
return 0;
}