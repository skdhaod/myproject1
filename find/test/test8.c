#include <stdio.h>
#include <string.h>
/*
�ٸ� ����:
    �� ���ڰ� ��ġ���� �ʴ´�
    str . �ڿ� Ȯ���ڰ� �ִ�.
    ���ϵ� ī�� �ڿ� ���ڰ� �ִµ� str �ڿ��� �� ���ڰ� ����
*/

int charcmp(char c1, char c2) {
    if (c1 >= 65 || c1 <= 90 || c2 >= 65 || c2 <= 90) { //�����϶� ����
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
    
    if(strstr(str, ".") && strstr(str, ".\0")) return 0;//.�� �ְ� .���� ������ ������ 0 ����
    while (argv[argv_index] != '.' && str[str_index] != '\0') {

        if (argv[argv_index] == '*') {
            argv_index++;
            if(argv[argv_index] == '.') return 1;//*�� ������ ���ڸ� return 1;
            while (!charcmp(argv[argv_index], str[str_index])) { //���ϵ�ī�� ���� ���ڿ� ���� ���ڸ� ã��������
                if(str[str_index]=='\0') break;//str ������ �������� ���߱�
                str_index++; // ���ϵ�ī�� ���� ���� ���ڰ� ���ö����� str�ε��� �ڷ� �ѱ��
            }
        }

        if (!charcmp(argv[argv_index], str[str_index])) {
            return 0;//���� argv���ڶ� str���ڶ� ���� �ʴٸ� 0 ����
        }

        argv_index++; str_index++;
    }

    if(argv_index=='.' && str_index=='\0'){//while���� ���������� ������?
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