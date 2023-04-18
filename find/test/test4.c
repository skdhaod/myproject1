#include <stdio.h>
char *print();
int main(){
    long long large=10;
    long long div=5;
    char c=48;
    c+=div;
    printf("%c", c);
    printf("%s", print());
    return 0;
}
char *print(){
    return "print¿‘¥œ¥Ÿ";
}