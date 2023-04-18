#include <stdio.h>
#include <Windows.h>

typedef BOOL (*P_GDFSE)(LPCTSTR, PULARGE_INTEGER);


void main(void) {
    BOOL fResult;
    char *pszDrive  = "C:\\Users\\kj233451\\study"; //�̸� ����


    P_GDFSE pGetDiskFreeSpaceEx = NULL;

    unsigned __int64 i64FreeBytesToCaller,
                   i64TotalBytes,
                   i64FreeBytes;


    pGetDiskFreeSpaceEx = (P_GDFSE)GetProcAddress (
                                                 GetModuleHandle ("kernel32.dll"), "GetDiskFreeSpaceExA"
                                                 );

    fResult = pGetDiskFreeSpaceEx (pszDrive,
                                (PULARGE_INTEGER)&i64FreeBytes);

    //if (!fResult) {
    //    printf ("ERROR: Could not get free space for %s\n", pszDrive);
    //    exit(GetLastError());
    //}


  unsigned __int64 UsedBytes = i64TotalBytes - i64FreeBytes;

    printf ("%s\n", pszDrive);
    //printf ("��ü �뷮    = %I64u MB, (%.1f GB)\n\n", i64TotalBytes        / (1024*1024), (double) i64TotalBytes        / (1024*1024*1024));

   // printf ("����� �뷮  = %I64u B, (%.1f GB)\n",   UsedBytes, (double) UsedBytes            / (1024*1024*1024));
    printf ("���� �뷮    = %lld B, (%.1f GB)\n",   i64FreeBytes , (double) i64FreeBytes         / (1024*1024*1024));
    //printf ("���� �뷮*   = %I64u B, (%.1f GB)\n",   i64FreeBytesToCaller, (double) i64FreeBytesToCaller / (1024*1024*1024));

}