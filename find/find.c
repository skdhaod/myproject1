#include "findfunc.h"

/*
�̰� ����

C ����̺��� �������� �̸��� �����ϴ�.
���� �Ϸ� ��ȣ : 5C11-2CF0

C:\Users\kj233451\study ���͸�

����-��¥  ����/�� ��:��    <DIR ����>  ����Ʈ �̸�
               n�� ����                   nnnnn ����Ʈ
               r�� ���͸�          nnnnnnnnnnn ����Ʈ ����

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