#include <stdio.h>
#include <io.h>

// 파일인지 디렉토리인지 판단 함수
int isFileOrDir(char* s);

int main(void) {
  char* filename = "ref.c"; // 여기에 파일이나 폴더명 입력

  int result = isFileOrDir(filename);

  if (result == 1)
    puts("파일입니다.");
  else if (result == 0)
      puts("디렉토리입니다.");
  else if (result == -1)
      puts("그런 파일 또는 디렉토리는 없습니다.");

  return 0;
}

int isFileOrDir(char* s) {
  struct _finddatai64_t c_file;
  int hFile, result;

  if ( (hFile = _findfirsti64(s, &c_file)) == -1L )
    result = -1; // 파일 또는 디렉토리가 없으면 -1 반환
  else if (c_file.attrib & _A_SUBDIR)
      result = 0; // 디렉토리면 0 반환
  else
    result = 1; // 그밖의 경우는 "존재하는 파일"이기에 1 반환

  _findclose(hFile);
  return result;
}