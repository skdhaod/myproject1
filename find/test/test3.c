#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <dirent.h>

int main(){
    struct tm *t;
    struct _stat buf;
    DIR *dp;
    struct dirent *de;
    char name[20];
    sprintf(name, ".\\find.c");
    printf("%s³¡\n", name);
    /**/
    _stat(name, &buf);//////////////////////////////
    dp=opendir(name);
    while((de=readdir(dp))!=NULL){
        printf("%s\t%d\n", de->d_name, localtime(&buf.st_mtime)->tm_year+1900);
    }
}