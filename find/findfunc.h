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
#include <unistd.h>//getcwd(���� �۾� ��� ���) �Լ� ���
#endif
#ifndef __TIME_H__
#include <time.h>//localtime �Լ�, tm����ü ���
#endif
#ifndef __DIRENT_H__
#include <dirent.h>//���丮 ����
#endif
#ifndef __WINDOW_H__
#include <Windows.h>//GetDiskFreeSpaceEx ���
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


void volume_info();//���� ����

void find_cur();//���� ���丮 find

void find_target(char *argv[]);//Ÿ�� find
void find_target_dir(char *argv[]);//dir find
void find_target_file(char *argv[]);//file find

int is_file_or_dir(char *filename); //dir���� file���� ����
char* get_time(struct tm *t); //�ð� �޾ƿ���
__int64 get_filesize(char *filename); //���� ũ�� ����
void add_comma(__int64 num, char *strnum); //�޸����
