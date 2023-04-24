#include <stdio.h>
#include <stdlib.h> // exit()
#include <io.h>
#include <errno.h>


int main(void) {
  struct _finddatai64_t c_file;
  intptr_t hFile;
  char path[] = "*.*";


  if ( (hFile = _findfirsti64(path, &c_file)) == -1L ) {
    switch (errno) {
      case ENOENT:
        printf(":: 파일이 없음 ::\n"); break;
      case EINVAL:
        fprintf(stderr, "Invalid path name.\n"); exit(1); break;
      case ENOMEM:
        fprintf(stderr, "Not enough memory or file name too long.\n"); exit(1); break;
      default:
        fprintf(stderr, "Unknown error.\n"); exit(1); break;
    }
  } // end if
  else {
    printf("-- 파일 목록 --\n");
    do {
      printf("%s\n", c_file.name);
    } while(_findnexti64(hFile, &c_file) == 0);
    _findclose(hFile); // _findfirsti64(), _findnexti64()에 사용된 메모리를 반환
  } // end else


  return 0;
}