#include <stdio.h>
enum wildcard_loca { NONE = 0, EXIST_FN, EXIST_EX, EXIST_BOTH };
int main(int argc, char* argv[]) {
    char r='*';
    char d[5]="as*d";
    if(r==d[2]){
        printf("d");
    }
    return 0;
}