#include "findfunc.h"

/*
이거 구현

C 드라이브의 볼륨에는 이름이 없습니다.
볼륨 일련 번호 : 5C11-2CF0

C:\Users\kj233451\study 디렉터리

변경-날짜  오전/후 시:간    <DIR 여부>  바이트 이름
               n개 파일                   nnnnn 바이트
               r개 디렉터리          nnnnnnnnnnn 바이트 남음

*/

int main(int argc, char* argv[]){

    volume_info();
    
    if(argv[1]==NULL){
        find_cur();
    }
    else{
        find_target(argv);
    }

    return 0;
}