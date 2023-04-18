#include <stdio.h>
#include <dirent.h>
 
int main(void)
{
    struct dirent *de;  // Pointer for directory entry

    // opendir() returns a pointer of DIR type.
    DIR *dr = opendir(".");

    if (dr == NULL)  // opendir returns NULL if couldn't open directory
    {
        printf("Could not open current directory" );
        return 0;
    }

    // for readdir()
    while ((de = readdir(dr)) != NULL) 
    {
        if (de->d_type == DT_REG)
            printf("%s\n", de->d_name);
        else if (de->d_type == DT_DIR)
            printf("%s\n", de->d_name);
        else 
            printf("%s--> ??",de->d_name);

    }

    closedir(dr);
    return 0;
}