#include <stdio.h>
#include <string.h>
/*
다를 조건:
    걍 문자가 일치하지 않는다
    str . 뒤에 확장자가 있다.
    와일드 카드 뒤에 문자가 있는데 str 뒤에는 그 문자가 없다
*/

int charcmp(char c1, char c2) {
    if (c1 >= 65 || c1 <= 90 || c2 >= 65 || c2 <= 90) { //문자일때 구분
        if (c1 == c2 || c1 - c2 == 32 || c2 - c1 == 32) {
            return 1;
        }
    }
    if (c1 == c2) return 1;
    return 0;
}
int is_same_wildcard_str(char argv[], char str[]) {
    int argv_index = 0, str_index = 0;
    int test = 0;
    
    if(strstr(str, ".") && strstr(str, ".\0")) return 0;//.이 있고 .으로 끝나지 않으면 0 리턴
    while (argv[argv_index] != '.' && str[str_index] != '\0') {

        if (argv[argv_index] == '*') {
            argv_index++;
            if(argv[argv_index] == '.') return 1;//*이 마지막 글자면 return 1;
            while (!charcmp(argv[argv_index], str[str_index])) { //와일드카드 다음 문자와 같은 글자를 찾을때까지
                if(str[str_index]=='\0') break;//str 끝까지 가버리면 멈추기
                str_index++; // 와일드카드 문자 다음 글자가 나올때까지 str인덱스 뒤로 넘기기
            }
        }

        if (!charcmp(argv[argv_index], str[str_index])) {
            return 0;//만약 argv글자랑 str글자랑 같지 않다면 0 리턴
        }

        argv_index++; str_index++;
    }

    if(argv_index=='.' && str_index=='\0'){//while문이 정상적으로 끝났나?
        return 1;
    }else{
        return 0;
    }
}


int main() {
    char argv[] = "*.";
    char str[] = "..";
    printf("%d\n", sizeof(str));
    if (is_same_wildcard_str(argv, str))
        printf("same\n");
    else printf("diff\n");
    return 0;
}