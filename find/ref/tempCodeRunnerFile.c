#include <stdio.h>
#include <glob.h>

int main(int argc, char *argv[])
{
    glob_t globbuf; // glob_t 구조체 선언
    int i;

    if (argc != 2) {// 인자가 하나만 있는지 확인
        printf("Usage: %s pattern\n", argv[0]);
    return -1;
    }

    if (glob(argv[1], 0, NULL, &globbuf) != 0) // glob() 함수 호출
    {
        printf("glob() error\n");
    return -1;
    }

    for (i = 0; i < globbuf.gl_pathc; i++){ // 결과 출력
        printf("%s\n", globbuf.gl_pathv[i]);
    }

        globfree(&globbuf); // 메모리 해제
    return 0;
}