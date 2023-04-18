#include <Windows.h>
#include<stdio.h>
int main(){
    char *name=".";
    ULARGE_INTEGER free_memory, avail_memory, total_memory;//엄청 큰 크기 측정하려고 있는 구조체?
    memset(&free_memory, 0, sizeof(free_memory));//메모리 0으로 초기화

    GetDiskFreeSpaceEx(name, &avail_memory, &total_memory, &free_memory);
    printf("%lld\n%lld", free_memory, (LONGLONG)free_memory.QuadPart);
}