#ifndef __STDIO_H__
#include <stdio.h>
#endif
//#ifndef __IO_H__
//#include <io.h>
//#endif
#ifndef __STDLIB_H__
#include <stdlib.h>
#endif
#ifndef __UNISTD_H__
#include <unistd.h>//getcwd(현재 작업 경로 얻는) 함수 사용
#endif
#ifndef __TIME_H__
#include <time.h>//localtime 함수, tm구조체 사용
#endif
#ifndef __DIRENT_H__
#include <dirent.h>//디렉토리 접근
#endif
#ifndef __WINDOW_H__
#include <Windows.h>//GetDiskFreeSpaceEx 사용
#endif

#ifndef __STRING_H__
#include <string.h>
#endif
#ifndef _SYS_STAT_H
#include <sys/stat.h>
#endif
/*
#ifndef __FILEAPI_H__
#include <fileapi.h> //getvolumeinfo
#endif
*/


void volume_info();//볼륨 정보

void find_cur();//현재 디렉토리 find

void find_target(char *argv[]);//타겟 find
void find_target_dir(char *argv[]);//dir find
void find_target_file(char *argv[]);//file find

int is_file_or_dir(char *filename); //dir인지 file인지 구분
char* get_time(struct tm *t); //시간 받아오기
__int64 get_filesize(char *filename); //파일 크기 측정
void add_comma(__int64 num, char *strnum); //콤마찍기
