#include <stdio.h>
#include <sys/stat.h>
#include <time.h>
#include <errno.h>

char* timeToString(struct tm *t);


int main(void) {
  struct _stat buf;
  char* filename = "sample.txt";


  if ( _stat(filename, &buf) != 0 ) {
    switch (errno) {
      case ENOENT:
        fprintf(stderr, "File %s not found.\n", filename); break;
      case EINVAL:
        fprintf(stderr, "Invalid parameter to _stat.\n"); break;
      default:
        fprintf(stderr, "Unexpected error in _stat.\n");
    }
  }
  else {
    printf("%s\n", filename);
    printf("\tTime Creation     : %s\n", timeToString(localtime(&buf.st_ctime)));
    printf("\tTime Last Written : %s\n", timeToString(localtime(&buf.st_mtime)));
    printf("\tTime Last Access  : %s\n", timeToString(localtime(&buf.st_atime)));
  }


}




char* timeToString(struct tm *t) {
  static char s[20];

  sprintf(s, "%04d-%02d-%02d %02d:%02d",
              t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
              t->tm_hour, t->tm_min
          );

  return s;
}