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

enum FILE_OR_DIR{DR=0, FL, OTHER}; //디렉토리, 파일, 그 외 구분

void volume_info();//볼륨 정보

void find_cur();//현재 디렉토리 find

void find_target(int argc, char *argv[]);//타겟 찾기
void find_default(char *argv[]);//기본 find
__int64 find_sub_dir(char *argv, int *filecnt, int *dircnt);//하위 dir find
__int64 find_in_sub_dir(int argc, char *argv, int *filecnt, int *dircnt, char* file_to_find[]);//하위 dir 안쪽 find
void find_qmark(char *argv[]);//?만 입력됐을때 출력
void find_option_s(int argc, char *argv[]);//find -s가 입력됐을때

void find_none_extension(char *argv[]);

int is_same_wildcard_str(char argv[], char str[], unsigned int type);//와일드카드 포함 문자열이 일치하는가?
int charcmp(char argv, char str);//문자가 일치하는가?
int is_last_str(char str1[], char str2[]);//str2가 str1의 마지막 문자열이 맞는가?


int is_file_or_dir(char *filename); //dir인지 file인지 구분
char* get_time(struct tm *t); //시간 받아오기
__int64 get_filesize(char *filename); //파일 크기 측정
void add_comma(__int64 num, char *strnum); //숫자 사이에 콤마찍기
