#include <stdio.h>
#include <sys/stat.h>
#include <time.h>
#include <errno.h>

int main(){
    struct tm *t;
    struct _stat buf;
    char name[]="�ȳ� �ϼ���?";
    printf("%s\n", name);
    printf("%d", localtime(&buf.st_ctime));
}