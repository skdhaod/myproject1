#include "findfunc.h"

/*
해결해야할것
    서브 디렉터리 탐색
*/


void volume_info(){ //볼륨 정보 출력
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

void find_cur(){ //현재 경로 모든 파일 출력
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

void find_target(int argc, char* argv[]){ //타겟 구분
    char target[MAX_PATH];//타겟 이름

    //하위폴더인지 비교용
    sprintf(target, ".\\%s", argv[1]);
    int type=is_file_or_dir(target);

    if(is_last_str(argv[1],".") && argc==2){ //특수한 경우 걸러내기
        find_none_extension(argv); //.으로 끝나면 확장자가 없는 파일을 탐색해야함
        return;
    }else if(!strcmp(argv[1], "?")){ //물음표만 입력
        find_qmark(argv);
        return;
    }else if(!strcmp(argv[1], "-s")){ //옵션 입력
        find_option_s(argc, argv);
        return;
    }
    else if(type==DR && argc==2){
        find_sub_dir(argv[1], 0, 0);//폴더 하나만 검색했다면 하위폴더 탐색으로 넘어가기
        return;
    }
    else find_default(argv);
}

void find_default(char *argv[]){//기본 출력
    struct dirent *de;
    DIR *dp;
    char pathname[100]; //경로 이름
    char strnum[20];//숫자를 문자열로 저장할때 쓸 배열
    ULARGE_INTEGER free_memory;//엄청 큰 크기 측정하려고 있는 구조체
    memset(&free_memory, 0, sizeof(free_memory));//메모리 0으로 초기화
    struct _stat buf;//현재 파일 정보 받아오는 구조체
    int filecnt=0;//파일 개수
    int dircnt=0;//폴더 개수
    __int64 filesum=0;//파일 크기 합계
    __int64 fs;//filesize
    int i=1;//argv 인덱스, 초기값=1

    dp=opendir(".");
    de = readdir(dp);

    printf("\n %s 디렉터리\n\n", getcwd(pathname, 100)); //현위치 출력


    while (argv[i]!=NULL && de!=NULL){
        _stat(de->d_name, &buf);
        if (!strcmpi(argv[i], de->d_name)){ //만약 argv와 dname이 같다면 출력
            printf("%s", get_time(localtime(&buf.st_mtime)));
            if (de->d_type == DT_REG){//파일일때
                fs=get_filesize(de->d_name);
                add_comma(fs, strnum);
                printf("%14s %s\n", strnum, de->d_name);
                filecnt++;
                filesum+=fs;
            }

            else if (de->d_type == DT_DIR){//폴더일때
                printf("<DIR>\t\t %s\n", de->d_name);
                dircnt++;
            }
            i++;
        }
        de = readdir(dp);
    }
        

    if(!filecnt && !dircnt){
        printf("파일을 찾을 수 없습니다.\n");
        return;
    }

    GetDiskFreeSpaceEx(pathname, NULL, NULL, &free_memory); //디스크 용량 정보 받기
    add_comma(filesum, strnum);
    printf("\t%8d개 파일\t%19s 바이트\n", filecnt, strnum);
    add_comma((LONGLONG)free_memory.QuadPart, strnum);
    printf("\t%8d개 디렉터리 %16s 바이트 남음\n", dircnt, strnum);

    closedir(dp);
}

__int64 find_sub_dir(char *argv, int *argc_file, int *argc_dircnt){ //하위 디렉 탐색
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
    
    sprintf(target, ".\\%s", argv);
    dp=opendir(target);
    de=readdir(dp);

    printf("\n %s\\%s 디렉터리\n\n", getcwd(pathname, 100), argv);//폴더면 경로에 추가해서 출력
    GetDiskFreeSpaceEx(pathname, NULL, NULL, &free_memory); //디스크 용량 정보 받기

    do{
        sprintf(acs, ".\\%s\\%s", argv, de->d_name);
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

    if(!filecnt && !dircnt){
        printf("파일을 찾을 수 없습니다.\n");
        return -1;
    }
    add_comma(filesum, strnum);
    printf("\t%8d개 파일\t%19s 바이트\n", filecnt, strnum);
    add_comma((LONGLONG)free_memory.QuadPart, strnum);
    printf("\t%8d개 디렉터리 %16s 바이트 남음\n", dircnt, strnum);
    closedir(dp);

    *argc_dircnt+=dircnt;
    *argc_file+=filecnt;
    return filesum;
}

__int64 find_in_sub_dir(int argc, char *dir_path, int *argc_filecnt, int *argc_dircnt, char* file_to_find[]){ //하위 디렉 안쪽의 파일 탐색
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
    int i;
    
    sprintf(target, ".\\%s", dir_path); //폴더를 경로로 추가
    dp=opendir(target);

    for(i=2;i<argc; i++){
        rewinddir(dp);
        while((de=readdir(dp)) && file_to_find[i]){
            sprintf(acs, "%s\\%s", target, de->d_name);
            _stat(acs, &buf);
            if(!strcmpi(de->d_name, file_to_find[i])){
                if(!filecnt && !dircnt) //처음 찾아지면 이거 출력
                    printf("\n %s\\%s 디렉터리\n\n", getcwd(pathname, 100), dir_path);//폴더면 경로에 추가해서 출력

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
            }
        }
    }
    
    closedir(dp);
    if(filecnt||dircnt){
        add_comma(filesum, strnum);
        printf("\t%8d개 파일\t%19s 바이트\n", filecnt, strnum);
    }
    *argc_dircnt+=dircnt;
    *argc_filecnt+=filecnt;
    return filesum;
}

void find_qmark(char *argv[]){ //?만 입력됐을때
    struct dirent *de;
    DIR *dp;
    char pathname[100]; //경로 이름
    char strnum[20];//숫자를 문자열로 저장할때 쓸 배열
    ULARGE_INTEGER free_memory;//엄청 큰 크기 측정하려고 있는 구조체
    memset(&free_memory, 0, sizeof(free_memory));//메모리 0으로 초기화
    struct _stat buf;//현재 파일 정보 받아오는 구조체
    int filecnt=0;//파일 개수
    int dircnt=0;//폴더 개수
    __int64 filesum=0;//파일 크기 합계
    __int64 fs;//filesize
    char *dirname;
    int i=1;//argv 인덱스, 초기값=1
    
    dp=opendir(".");
    de = readdir(dp);

    printf("\n %s 디렉터리\n\n", getcwd(pathname, 100)); //현위치 출력


    while (i<3){ //.과 ..만 출력되면 됨
        _stat(de->d_name, &buf);
        printf("%s", get_time(localtime(&buf.st_mtime)));
        if (de->d_type == DT_REG){//파일일때
            fs=get_filesize(de->d_name);
            add_comma(fs, strnum);
            printf("%14s %s\n", strnum, de->d_name);
            filecnt++;
            filesum+=fs;
        }

        else if (de->d_type == DT_DIR){//폴더일때
            printf("<DIR>\t\t %s\n", de->d_name);
            dircnt++;
        }
        i++;
        de = readdir(dp);
    }
        

    if(!filecnt && !dircnt){
        printf("파일을 찾을 수 없습니다.\n");
        return;
    }

    GetDiskFreeSpaceEx(pathname, NULL, NULL, &free_memory); //디스크 용량 정보 받기
    add_comma(filesum, strnum);
    printf("\t%8d개 파일\t%19s 바이트\n", filecnt, strnum);
    add_comma((LONGLONG)free_memory.QuadPart, strnum);
    printf("\t%8d개 디렉터리 %16s 바이트 남음\n", dircnt, strnum);

    closedir(dp);
    
}

void find_none_extension(char *argv[]){//확장자가 없는(*. 로 끝나는) 파일 탐색
    struct dirent *de;
    DIR *dp;
    ULARGE_INTEGER free_memory;//엄청 큰 크기 측정하려고 있는 구조체
    memset(&free_memory, 0, sizeof(free_memory));//메모리 0으로 초기화
    struct _stat buf;//현재 파일 정보 받아오는 구조체, 시간 받아올거임
    char pathname[100]; //경로 이름
    int filecnt=0;//파일 개수
    int dircnt=0;//폴더 개수
    __int64 filesum=0;//파일 크기 합계
    __int64 fs;//filesize
    char strnum[20]; //임시로 숫자에 콤마 추가할 스트링

    dp=opendir(".");
    de=readdir(dp);

    printf("\n %s 디렉터리\n\n", getcwd(pathname, 100));


    do{
        _stat(de->d_name, &buf);
        if(is_same_wildcard_str(argv[1], de->d_name, de->d_type)){
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
        }
    }while ((de = readdir(dp)) != NULL);

    if(!filecnt && !dircnt){
        printf("파일을 찾을 수 없습니다.\n");
        return;
    }
    GetDiskFreeSpaceEx(pathname, NULL, NULL, &free_memory); //디스크 용량 정보 받기
    add_comma(filesum, strnum);
    printf("\t%8d개 파일\t%19s 바이트\n", filecnt, strnum);
    add_comma((LONGLONG)free_memory.QuadPart, strnum);
    printf("\t%8d개 디렉터리 %16s 바이트 남음\n", dircnt, strnum);
    closedir(dp);
}

void find_option_s(int argc, char *argv[]){ //옵션 -s가 있을때
    struct dirent *de;
    DIR *dp;
    char pathname[100]; //경로 이름
    char strnum[20];//숫자를 문자열로 저장할때 쓸 배열
    ULARGE_INTEGER free_memory;//엄청 큰 크기 측정하려고 있는 구조체
    memset(&free_memory, 0, sizeof(free_memory));//메모리 0으로 초기화
    struct _stat buf;//현재 파일 정보 받아오는 구조체
    int filecnt=0;//파일 개수
    int dircnt=0;//폴더 개수
    __int64 filesum=0;//파일 크기 합계
    __int64 fs;//filesize

    __int64 file_total=0;

    dp=opendir(".");
    de = readdir(dp);

    if(argc>2){
        filesum=0;
        do{
            if(!strcmp(de->d_name, "..")){
                de=readdir(dp);
                continue;
            }
            if(de->d_type ==DT_DIR) //폴더면 내부 디렉터리 탐색
                filesum += find_in_sub_dir(argc, de->d_name, &filecnt, &dircnt, argv);

            de=readdir(dp);
        }while (de!=NULL);
    }else{
        do{
            _stat(de->d_name, &buf);
            if (de->d_type == DT_DIR){//폴더일때
                filesum += find_sub_dir(de->d_name, &filecnt, &dircnt);
            }
        }while ((de = readdir(dp))!=NULL);
    }

    if(!filecnt && !dircnt){
            printf("파일을 찾을 수 없습니다.\n");
            return;
    }

    GetDiskFreeSpaceEx(NULL, NULL, NULL, &free_memory); //디스크 용량 정보 받기

    printf("\n     전체 파일:\n");
    add_comma(filesum, strnum);
    printf("\t%8d개 파일\t%19s 바이트\n", filecnt, strnum);
    add_comma((LONGLONG)free_memory.QuadPart, strnum);
    printf("\t%8d개 디렉터리 %16s 바이트 남음\n", dircnt, strnum);
    closedir(dp);
}


//보조 함수들
int charcmp(char c1, char c2) {
    if (c1 >= 65 || c1 <= 90 || c2 >= 65 || c2 <= 90) { //문자일때 구분
        if (c1 == c2 || c1 - c2 == 32 || c2 - c1 == 32) {
            return 1;
        }
    }
    if (c1 == c2) return 1;
    return 0;
}

int is_last_str(char str1[], char str2[]){
    int start_index=strlen(str1)-strlen(str2);
    int i;
    for(i=0; i<strlen(str2); i++){
        if(str1[start_index+i]!=str2[i]) return 0;
    }
    return 1;
}

int is_same_wildcard_str(char argv[], char str[], unsigned int type) {
    int argv_index = 0, str_index = 0;
    int test = 0;
    
    if(type==DT_DIR) return 1;//폴더는 확장자가 없으니까 그냥 1 리턴
    if(strstr(str, ".") && !is_last_str(str, ".")) return 0;//.이 있고 .으로 끝나지 않으면 0 리턴
    while (argv[argv_index] != '.' && str[str_index] != '\0') {

        if (argv[argv_index] == '*') {
            argv_index++;
            if(argv[argv_index] == '.') return 1;//*이 마지막 글자면 return 1;
            while (!charcmp(argv[argv_index], str[str_index])) { //와일드카드 다음 문자와 같은 글자를 찾을때까지
                if(str[str_index]=='\0') break;//str 끝까지 가버리면 멈추기
                str_index++; // 와일드카드 문자 다음 글자가 나올때까지 str인덱스 뒤로 넘기기
            }
        }

        if (!charcmp(argv[argv_index], str[str_index])) {
            return 0;//만약 argv글자랑 str글자랑 같지 않다면 0 리턴
        }

        argv_index++; str_index++;
    }

    if(argv_index=='.' && str_index=='\0'){//while문이 정상적으로 끝났나?
        return 1;
    }else{
        return 0;
    }
}

int is_file_or_dir(char *filename){ //파일인지 디렉인지 판단
    struct _finddatai64_t c_file;
    int hFile, result;

                    //3기가 이상의 파일을 다루는 함수, 파일을 검색한다.
    if ( (hFile = _findfirsti64(filename, &c_file)) == -1 )
        result = OTHER; // 파일 또는 디렉토리가 없으면 -1 반환
    else if (c_file.attrib & _A_SUBDIR) // c_file.attrib : 파일의 특성을 나타냄
        result = DR; // 디렉토리면 0 반환
    else
        result = FL; // 그밖의 경우는 존재하는 파일, 1 반환

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