#include <Windows.h>
#include<stdio.h>
int main(){
    char *name=".";
    ULARGE_INTEGER free_memory, avail_memory, total_memory;//��û ū ũ�� �����Ϸ��� �ִ� ����ü?
    memset(&free_memory, 0, sizeof(free_memory));//�޸� 0���� �ʱ�ȭ

    GetDiskFreeSpaceEx(name, &avail_memory, &total_memory, &free_memory);
    printf("%lld\n%lld", free_memory, (LONGLONG)free_memory.QuadPart);
}