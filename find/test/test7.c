#include <stdio.h>
enum wildcard_loca { NONE = 0, EXIST_FN, EXIST_EX, EXIST_BOTH };
int main(int argc, char* argv[]) {
    int i=1;
    printf("%d\n", argc);
    if(argv[1]=="*"){
        printf("wow\n");
    }
    while(argv[i]!=NULL){
        printf("%s\n", argv[i]);
        i++;
    }
    return 0;
}