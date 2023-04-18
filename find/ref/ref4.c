#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

int main(void) {
    DIR *dp;
    struct dirent *dirp;
    struct stat filestat;
    char *dir = ".vscode";

    if ((dp = opendir(dir)) == NULL) {
        printf("Error opening %s\n", dir);
        return 1;
    }

    while ((dirp = readdir(dp)) != NULL) {
        if (dirp->d_type == DT_DIR && strcmp(dirp->d_name, dir) != 0 && strcmp(dirp->d_name, "..") != 0) {
            char subdir[256];
            snprintf(subdir, sizeof(subdir), "%s/%s", dir, dirp->d_name);
            if (lstat(subdir, &filestat) == -1) continue;
            printf("%s was created at %s", subdir, ctime(&filestat.st_ctime));
        }
    }

    closedir(dp);
    return 0;
}