#include <stdio.h>
#include <string.h>
int is_last_str(char str1[], char str2[]){
    int start_index=strlen(str1)-strlen(str2);
    int i;
    for(i=0; i<strlen(str2); i++){
        if(str1[start_index+i]!=str2[i]) return 0;
    }
    return 1;
}
int main(){
    char str1[]="hello.";
    char str2[]=".";
    printf("%s %s", str1, str2);
    if (is_last_str(str1, str2)) printf("same\n");
    else printf("diff\n");
}