#include "findfunc.h"

/*�ӽ������*/

void volume_info(){
    char volumeName[MAX_PATH + 1] = { 0 };
    DWORD serialNumber = 0;
    int i;
    char *drive="C:\\";
    GetVolumeInformation(drive, volumeName, MAX_PATH + 1, &serialNumber, NULL, NULL, NULL, 0);
    if(!strlen(volumeName))
        printf(" C ����̺��� �������� �̸��� �����ϴ�.\n");
    else
        printf(" C ����̺��� ����: %s\n", volumeName);
    printf(" ���� �Ϸ� ��ȣ : %04X-%04X\n", HIWORD(serialNumber), LOWORD(serialNumber)); // (serialNumber >> 16) & 0xffff, serialNumber & 0xffff�ε� �� �� ����
}


void find_cur(){
    struct dirent *de;
    DIR *dp;
    ULARGE_INTEGER free_memory;//��û ū ũ�� �����Ϸ��� �ִ� ����ü
    memset(&free_memory, 0, sizeof(free_memory));//�޸� 0���� �ʱ�ȭ
    struct _stat buf;//���� ���� ���� �޾ƿ��� ����ü
    char pathname[100]; //��� �̸�
    int filecnt=0;//���� ����
    int dircnt=0;//���� ����0
    __int64 filesum=0;//���� ũ�� �հ�
    __int64 fs;//filesize
    char strnum[20]; //�ӽ÷� ���ڿ� �޸� �߰��� ��Ʈ��

    dp=opendir(".");
    de=readdir(dp);


    printf("\n %s ���͸�\n\n", getcwd(pathname, 100));

    do{
        _stat(de->d_name, &buf);
        printf("%s", get_time(localtime(&buf.st_mtime)));

        if (de->d_type == DT_REG){ //�Ϲ� �����̸�
            fs=get_filesize(de->d_name); //���� ũ�� ����Ʈ�� ����
            add_comma(fs, strnum); //���� ũ�⿡ �޸����
            printf("%14s %s\n", strnum, de->d_name);
            filecnt++; //���� ���� ����
            filesum+=fs; 
        }

        else if (de->d_type == DT_DIR){//������  dir�����
            printf("<DIR>\t\t %s\n", de->d_name);
            dircnt++;
        }
    }while ((de = readdir(dp)) != NULL);

    GetDiskFreeSpaceEx(pathname, NULL, NULL, &free_memory); //��ũ �뷮 ���� �ޱ�
    add_comma(filesum, strnum);
    printf("\t%8d�� ����\t%19s ����Ʈ\n", filecnt, strnum);
    add_comma((LONGLONG)free_memory.QuadPart, strnum);
    printf("\t%8d�� ���͸� %16s ����Ʈ ����\n", dircnt, strnum);
    closedir(dp);
}

void find_target(int argc, char* argv[]){
    struct dirent *de;
    DIR *dp;
    char pathname[100]; //��� �̸�
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
        printf("\n %s ���͸�\n\n", getcwd(pathname, 100)); //����ġ ���
        printf("������ ã�� �� �����ϴ�.\n");
    }

    closedir(dp);
}

void find_target_dir(char *argv[]){
    ULARGE_INTEGER free_memory;//��û ū ũ�� �����Ϸ��� �ִ� ����ü
    memset(&free_memory, 0, sizeof(free_memory));//�޸� 0���� �ʱ�ȭ
    struct dirent *de;
    DIR *dp;
    char pathname[100]; //��� �̸�
    struct _stat buf;//���� ���� ���� �޾ƿ��� ����ü
    char acs[100];//�׼����� ��ġ
    int filecnt=0;//���� ����
    int dircnt=0;//���� ����
    __int64 filesum=0;//���� ũ�� �հ�
    __int64 fs;//filesize
    char *dirname;
    char strnum[20];
    char target[120];
    
    sprintf(target, ".\\%s", argv[1]);
    dp=opendir(target);
    de=readdir(dp);

    printf("\n %s\\%s ���͸�\n\n", getcwd(pathname, 100), argv[1]);//������ ��ο� �߰��ؼ� ���
    GetDiskFreeSpaceEx(pathname, NULL, NULL, &free_memory); //��ũ �뷮 ���� �ޱ�

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

        else if (de->d_type == DT_DIR){//������  dir�����
            printf("<DIR>\t\t %s\n", de->d_name);
            dircnt++;
        }
            
    }while ((de = readdir(dp)) != NULL);

    add_comma(filesum, strnum);
    printf("\t%8d�� ����\t%19s ����Ʈ\n", filecnt, strnum);
    add_comma((LONGLONG)free_memory.QuadPart, strnum);
    printf("\t%8d�� ���͸� %16s ����Ʈ ����\n", dircnt, strnum);
    closedir(dp);
}
void find_target_file(char *argv[]){
    ULARGE_INTEGER free_memory;//��û ū ũ�� �����Ϸ��� �ִ� ����ü
    memset(&free_memory, 0, sizeof(free_memory));//�޸� 0���� �ʱ�ȭ
    struct dirent *de;
    DIR *dp;
    char pathname[100]; //��� �̸�
    struct _stat buf;//���� ���� ���� �޾ƿ��� ����ü
    int filecnt=0;//���� ����
    int dircnt=0;//���� ����
    __int64 filesum=0;//���� ũ�� �հ�
    __int64 fs;//filesize
    char *dirname;
    char strnum[20];
    char target[120];

    dp=opendir(".");

    printf("\n %s ���͸�\n\n", getcwd(pathname, 100)); //����ġ ���
    GetDiskFreeSpaceEx(pathname, NULL, NULL, &free_memory);

    while ((de = readdir(dp)) != NULL){
        _stat(de->d_name, &buf);
        if (de->d_type == DT_REG && !strcmp(argv[1], de->d_name)){
            printf("%s", get_time(localtime(&buf.st_mtime)));//�ð� ��� ����
            fs=get_filesize(de->d_name); //����Ʈ�� �о����
            add_comma(fs, strnum);
            printf("     \t%8s %s\n", strnum, de->d_name); //����Ʈ, ���� �̸� ���
            filecnt++;
            filesum+=fs;
        }
    }


    if(!filecnt && !dircnt){
        printf("������ ã�� �� �����ϴ�.\n");
        return;
    }

    add_comma(filesum, strnum);
    printf("\t%8d�� ����\t%19s ����Ʈ\n", filecnt, strnum);
    add_comma((LONGLONG)free_memory.QuadPart, strnum);
    printf("\t%8d�� ���͸� %16s ����Ʈ ����\n", dircnt, strnum);
}

int is_file_or_dir(char *filename){
    struct _finddatai64_t c_file;
    int hFile, result;

                    //3�Ⱑ �̻��� ������ �ٷ�� �Լ�, ������ �˻��Ѵ�.
    if ( (hFile = _findfirsti64(filename, &c_file)) == -1 )
        result = -1; // ���� �Ǵ� ���丮�� ������ -1 ��ȯ
    else if (c_file.attrib & _A_SUBDIR) // c_file.attrib : ������ Ư���� ��Ÿ��
        result = 0; // ���丮�� 0 ��ȯ
    else
        result = 1; // �׹��� ���� "�����ϴ� ����"�̱⿡ 1 ��ȯ

    _findclose(hFile);
    return result;
}

char* get_time(struct tm *t){
    char *date;
    if(t->tm_hour == 0){
        sprintf(date, "%04d-%02d-%02d  ���� 12:%02d    ", t->tm_year+1900, t->tm_mon+1, t->tm_mday, t->tm_min);
    }
    else if(t->tm_hour == 12)
        sprintf(date, "%04d-%02d-%02d  ���� %02d:%02d    ", t->tm_year+1900, t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min);
    else if(t->tm_hour > 12)
        sprintf(date, "%04d-%02d-%02d  ���� %02d:%02d    ", t->tm_year+1900, t->tm_mon+1, t->tm_mday, t->tm_hour-12, t->tm_min);
    else{
        sprintf(date, "%04d-%02d-%02d  ���� %02d:%02d    ", t->tm_year+1900, t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min);
    }
    return date;

}
__int64 get_filesize(char *filename){
    struct _stati64 buf;

    if(_stati64(filename, &buf)) return -1;
    return buf.st_size;
}

void add_comma(__int64 num, char *strnum){
    __int64 temp=num;//�ڸ��� ���Ҷ� ����� �ӽ� ����
    __int64 div=1;//���� �� 10 ������ ����
    int intlen=1;//���� ����
    int i;//���� ����
    int index=0;//�迭 �ε���

    while(temp/10 >= 1){
        intlen++;
        temp=temp/10;
        div*=10;
    }

    for(i=intlen; i>0; i--){
        if(i !=intlen && i%3==0){//ù ��° ���ڰ� �ƴϰ� 3�ǹ���� ���ڰ� �ƴϸ� �޸� ����
            strnum[index]=',';
            index++;
        }
        strnum[index] = num/div + 48; //ù ��° �ڸ����� ����
        num%=div;
        div/=10;
        index++;
    }
    strnum[index]='\0';
}
