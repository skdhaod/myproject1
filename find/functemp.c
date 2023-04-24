#include "findfunc.h"

/*임시저장소*/

void volume_info(){
    char volumeName[MAX_PATH + 1] = { 0 };
    DWORD serialNumber = 0;
    int i;
    char *drive="C:\\";
    GetVolumeInformation(drive, volumeName, MAX_PATH + 1, &serialNumber, NULL, NULL, NULL, 0);
    if(!strlen(volumeName))
        printf(" C 드라이브의 볼륨에는 이름이 없습니다.\n");
    else
        printf(" C 드라이브의 볼륨: %s\n", volumeName);
    printf(" 볼륨 일련 번호 : %04X-%04X\n", HIWORD(serialNumber), LOWORD(serialNumber)); // (serialNumber >> 16) & 0xffff, serialNumber & 0xffff로도 쓸 수 있음
}


void find_cur(){
    struct dirent *de;
    DIR *dp;
    ULARGE_INTEGER free_memory;//엄청 큰 크기 측정하려고 있는 구조체
    memset(&free_memory, 0, sizeof(free_memory));//메모리 0으로 초기화
    struct _stat buf;//현재 파일 정보 받아오는 구조체
    char pathname[100]; //경로 이름
    int filecnt=0;//파일 개수
    int dircnt=0;//폴더 개수0
    __int64 filesum=0;//파일 크기 합계
    __int64 fs;//filesize
    char strnum[20]; //임시로 숫자에 콤마 추가할 스트링

    dp=opendir(".");
    de=readdir(dp);


    printf("\n %s 디렉터리\n\n", getcwd(pathname, 100));

    do{
        _stat(de->d_name, &buf);
        printf("%s", get_time(localtime(&buf.st_mtime)));

        if (de->d_type == DT_REG){ //일반 파일이면
            fs=get_filesize(de->d_name); //파일 크기 바이트로 저장
            add_comma(fs, strnum); //파일 크기에 콤마찍기
            printf("%14s %s\n", strnum, de->d_name);
            filecnt++; //파일 개수 증가
            filesum+=fs; 
        }

        else if (de->d_type == DT_DIR){//폴더면  dir출력함
            printf("<DIR>\t\t %s\n", de->d_name);
            dircnt++;
        }
    }while ((de = readdir(dp)) != NULL);

    GetDiskFreeSpaceEx(pathname, NULL, NULL, &free_memory); //디스크 용량 정보 받기
    add_comma(filesum, strnum);
    printf("\t%8d개 파일\t%19s 바이트\n", filecnt, strnum);
    add_comma((LONGLONG)free_memory.QuadPart, strnum);
    printf("\t%8d개 디렉터리 %16s 바이트 남음\n", dircnt, strnum);
    closedir(dp);
}

void find_target(int argc, char* argv[]){
    struct dirent *de;
    DIR *dp;
    char pathname[100]; //경로 이름
    char target[120];
    

    sprintf(target, ".\\%s", argv[1]);
    
    int type=is_file_or_dir(target);

    if(type==0){
        find_target_dir(argv);
    }
    else if(type==1){
        find_target_file(argv);
    }
    else{
        printf("\n %s 디렉터리\n\n", getcwd(pathname, 100)); //현위치 출력
        printf("파일을 찾을 수 없습니다.\n");
    }

    closedir(dp);
}

void find_target_dir(char *argv[]){
    ULARGE_INTEGER free_memory;//엄청 큰 크기 측정하려고 있는 구조체
    memset(&free_memory, 0, sizeof(free_memory));//메모리 0으로 초기화
    struct dirent *de;
    DIR *dp;
    char pathname[100]; //경로 이름
    struct _stat buf;//현재 파일 정보 받아오는 구조체
    char acs[100];//액세스할 위치
    int filecnt=0;//파일 개수
    int dircnt=0;//폴더 개수
    __int64 filesum=0;//파일 크기 합계
    __int64 fs;//filesize
    char *dirname;
    char strnum[20];
    char target[120];
    
    sprintf(target, ".\\%s", argv[1]);
    dp=opendir(target);
    de=readdir(dp);

    printf("\n %s\\%s 디렉터리\n\n", getcwd(pathname, 100), argv[1]);//폴더면 경로에 추가해서 출력
    GetDiskFreeSpaceEx(pathname, NULL, NULL, &free_memory); //디스크 용량 정보 받기

    do{
        sprintf(acs, ".\\%s\\%s", argv[1], de->d_name);
        _stat(acs, &buf);
        printf("%s", get_time(localtime(&buf.st_mtime)));

        if (de->d_type == DT_REG){
            fs=get_filesize(acs);
            add_comma(fs, strnum);
            printf("%14s %s\n", strnum, de->d_name);
            filecnt++;
            filesum+=fs;
        }

        else if (de->d_type == DT_DIR){//폴더면  dir출력함
            printf("<DIR>\t\t %s\n", de->d_name);
            dircnt++;
        }
            
    }while ((de = readdir(dp)) != NULL);

    add_comma(filesum, strnum);
    printf("\t%8d개 파일\t%19s 바이트\n", filecnt, strnum);
    add_comma((LONGLONG)free_memory.QuadPart, strnum);
    printf("\t%8d개 디렉터리 %16s 바이트 남음\n", dircnt, strnum);
    closedir(dp);
}
void find_target_file(char *argv[]){
    ULARGE_INTEGER free_memory;//엄청 큰 크기 측정하려고 있는 구조체
    memset(&free_memory, 0, sizeof(free_memory));//메모리 0으로 초기화
    struct dirent *de;
    DIR *dp;
    char pathname[100]; //경로 이름
    struct _stat buf;//현재 파일 정보 받아오는 구조체
    int filecnt=0;//파일 개수
    int dircnt=0;//폴더 개수
    __int64 filesum=0;//파일 크기 합계
    __int64 fs;//filesize
    char *dirname;
    char strnum[20];
    char target[120];

    dp=opendir(".");

    printf("\n %s 디렉터리\n\n", getcwd(pathname, 100)); //현위치 출력
    GetDiskFreeSpaceEx(pathname, NULL, NULL, &free_memory);

    while ((de = readdir(dp)) != NULL){
        _stat(de->d_name, &buf);
        if (de->d_type == DT_REG && !strcmp(argv[1], de->d_name)){
            printf("%s", get_time(localtime(&buf.st_mtime)));//시간 출력 정상
            fs=get_filesize(de->d_name); //바이트도 읽어야함
            add_comma(fs, strnum);
            printf("     \t%8s %s\n", strnum, de->d_name); //바이트, 파일 이름 출력
            filecnt++;
            filesum+=fs;
        }
    }


    if(!filecnt && !dircnt){
        printf("파일을 찾을 수 없습니다.\n");
        return;
    }

    add_comma(filesum, strnum);
    printf("\t%8d개 파일\t%19s 바이트\n", filecnt, strnum);
    add_comma((LONGLONG)free_memory.QuadPart, strnum);
    printf("\t%8d개 디렉터리 %16s 바이트 남음\n", dircnt, strnum);
}

int is_file_or_dir(char *filename){
    struct _finddatai64_t c_file;
    int hFile, result;

                    //3기가 이상의 파일을 다루는 함수, 파일을 검색한다.
    if ( (hFile = _findfirsti64(filename, &c_file)) == -1 )
        result = -1; // 파일 또는 디렉토리가 없으면 -1 반환
    else if (c_file.attrib & _A_SUBDIR) // c_file.attrib : 파일의 특성을 나타냄
        result = 0; // 디렉토리면 0 반환
    else
        result = 1; // 그밖의 경우는 "존재하는 파일"이기에 1 반환

    _findclose(hFile);
    return result;
}

char* get_time(struct tm *t){
    char *date;
    if(t->tm_hour == 0){
        sprintf(date, "%04d-%02d-%02d  오전 12:%02d    ", t->tm_year+1900, t->tm_mon+1, t->tm_mday, t->tm_min);
    }
    else if(t->tm_hour == 12)
        sprintf(date, "%04d-%02d-%02d  오후 %02d:%02d    ", t->tm_year+1900, t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min);
    else if(t->tm_hour > 12)
        sprintf(date, "%04d-%02d-%02d  오후 %02d:%02d    ", t->tm_year+1900, t->tm_mon+1, t->tm_mday, t->tm_hour-12, t->tm_min);
    else{
        sprintf(date, "%04d-%02d-%02d  오전 %02d:%02d    ", t->tm_year+1900, t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min);
    }
    return date;

}
__int64 get_filesize(char *filename){
    struct _stati64 buf;

    if(_stati64(filename, &buf)) return -1;
    return buf.st_size;
}

void add_comma(__int64 num, char *strnum){
    __int64 temp=num;//자릿수 구할때 사용할 임시 숫자
    __int64 div=1;//나눌 수 10 제곱수 단위
    int intlen=1;//숫자 길이
    int i;//포문 숫자
    int index=0;//배열 인덱스

    while(temp/10 >= 1){
        intlen++;
        temp=temp/10;
        div*=10;
    }

    for(i=intlen; i>0; i--){
        if(i !=intlen && i%3==0){//첫 번째 숫자가 아니고 3의배수인 숫자가 아니면 콤마 저장
            strnum[index]=',';
            index++;
        }
        strnum[index] = num/div + 48; //첫 번째 자릿수를 저장
        num%=div;
        div/=10;
        index++;
    }
    strnum[index]='\0';
}
