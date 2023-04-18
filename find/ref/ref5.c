#include <stdio.h>
#include <windows.h>
#include <string.h>

int main() {
    char volumeName[MAX_PATH + 1] = { 0 };
    DWORD serialNumber = 0;
    char drive[] = "C:\\";

    if (GetVolumeInformation(drive, volumeName, MAX_PATH + 1, &serialNumber, NULL, NULL, NULL, 0)) {
        printf("%d",strlen(volumeName));
        printf("Volume name: %s\n", volumeName);
        printf("Volume serial number: %lu\n", serialNumber);
    }
    else {
        printf("Error getting volume information.\n");
    }

    return 0;
}