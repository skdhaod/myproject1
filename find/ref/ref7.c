#include <stdio.h>
#include <Windows.h>
#include <WinNT.h>

int main()
{
    char volumeName[MAX_PATH + 1] = { 0 };
    char fileSystemName[MAX_PATH + 1] = { 0 };
    DWORD serialNumber = 0;
    DWORD maxComponentLen = 0;
    DWORD fileSystemFlags = 0;

    if (GetVolumeInformation("C:\\", volumeName, MAX_PATH + 1, &serialNumber, NULL, NULL, NULL, 0))
    {
        printf("%04x-%04x\n", (serialNumber >> 16) & 0xffff, serialNumber & 0xffff);
        //     16진수 출력   // 4비트 옆으로 옮긴듯(앞에만 출력하려고)        //이건 뒷자리 네 개만 출력하려고 그러는듯
    }

    return 0;
}