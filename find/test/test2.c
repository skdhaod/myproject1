#include <stdio.h>
#include <string.h>
void addslash(char* name){
    char* p=name;
    char temp[100]="\0";
    int cnt=0, i=0;
    while (p!=NULL){
        if(p[i]=='\\'){
            temp[i+cnt]='\\';
            cnt++;
        }
        temp[i+cnt]=name[i];
        i++; p++;
    }
    strcpy(name, temp);
}
int main(){
    char* str="riri\\kkrkrk";
    //addslash(str);
    printf("%s", str);
}