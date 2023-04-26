#include "findfunc.h"

/*
�ذ��ؾ��Ұ�

    * ������ �������� ���;���

    *.*������ ���͸��� ���;���

    1. ������������
    
    2. ������ ���� �ڿ� .�� �߰��ϸ� �ɱ�?
        argv�� ������ ������ �Ѵ�.

    ������ : �� ���ϵ�ī�尡 dir�� �Ȱ��� ����� ���� �ʴµ�
*/

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
    char target[120];//Ÿ�� �̸�
    char strnum[20];//���ڸ� ���ڿ��� �����Ҷ� �� �迭
    ULARGE_INTEGER free_memory;//��û ū ũ�� �����Ϸ��� �ִ� ����ü
    memset(&free_memory, 0, sizeof(free_memory));//�޸� 0���� �ʱ�ȭ
    struct _stat buf;//���� ���� ���� �޾ƿ��� ����ü
    int filecnt=0;//���� ����
    int dircnt=0;//���� ����
    __int64 filesum=0;//���� ũ�� �հ�
    __int64 fs;//filesize
    char *dirname;
    char* add_period;
    int i=1;//argv �ε���, �ʱⰪ=1

    dp=opendir(".");
    de = readdir(dp);
    
    //������������ �񱳿�
    sprintf(target, ".\\%s", argv[1]);
    int type=is_file_or_dir(target);

    if(is_last_str(argv[1],".") && argc==2){ //Ư���� ��� �ɷ�����
        find_none_extension(argv); //.���� ������ Ȯ���ڰ� ���� ������ Ž���ؾ���
        return;
    }else if(type==DR && argc==2){
        find_target_dir(argv);//���� �ϳ��� �˻��ߴٸ� �������� Ž������ �Ѿ��
        return;
    }

     

    printf("\n %s ���͸�\n\n", getcwd(pathname, 100)); //����ġ ���


    while (argv[i]!=NULL){
        _stat(de->d_name, &buf);
        if (!strcmp(argv[i], de->d_name)){ //���� argv�� dname�� ���ٸ� ���
            printf("%s", get_time(localtime(&buf.st_mtime)));
            if (de->d_type == DT_REG){//�����϶�
                fs=get_filesize(de->d_name);
                add_comma(fs, strnum);
                printf("%14s %s\n", strnum, de->d_name);
                filecnt++;
                filesum+=fs;
            }

            else if (de->d_type == DT_DIR){//�����϶�
                printf("<DIR>\t\t %s\n", de->d_name);
                dircnt++;
            }
            i++;
        }
        de = readdir(dp);
    }
        

    if(!filecnt && !dircnt){
        printf("������ ã�� �� �����ϴ�.\n");
        return;
    }

    GetDiskFreeSpaceEx(pathname, NULL, NULL, &free_memory); //��ũ �뷮 ���� �ޱ�
    add_comma(filesum, strnum);
    printf("\t%8d�� ����\t%19s ����Ʈ\n", filecnt, strnum);
    add_comma((LONGLONG)free_memory.QuadPart, strnum);
    printf("\t%8d�� ���͸� %16s ����Ʈ ����\n", dircnt, strnum);

    closedir(dp);
}

void find_target_dir(char *argv[]){ //���� �� Ž��

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

    if(!filecnt && !dircnt){
        printf("������ ã�� �� �����ϴ�.\n");
        return;
    }
    add_comma(filesum, strnum);
    printf("\t%8d�� ����\t%19s ����Ʈ\n", filecnt, strnum);
    add_comma((LONGLONG)free_memory.QuadPart, strnum);
    printf("\t%8d�� ���͸� %16s ����Ʈ ����\n", dircnt, strnum);
    closedir(dp);
}

void find_none_extension(char *argv[]){//Ȯ���ڰ� ����(*. �� ������) ���� Ž��
    struct dirent *de;
    DIR *dp;
    ULARGE_INTEGER free_memory;//��û ū ũ�� �����Ϸ��� �ִ� ����ü
    memset(&free_memory, 0, sizeof(free_memory));//�޸� 0���� �ʱ�ȭ
    struct _stat buf;//���� ���� ���� �޾ƿ��� ����ü, �ð� �޾ƿð���
    char pathname[100]; //��� �̸�
    int filecnt=0;//���� ����
    int dircnt=0;//���� ����
    __int64 filesum=0;//���� ũ�� �հ�
    __int64 fs;//filesize
    char strnum[20]; //�ӽ÷� ���ڿ� �޸� �߰��� ��Ʈ��

    dp=opendir(".");
    de=readdir(dp);

    printf("\n %s ���͸�\n\n", getcwd(pathname, 100));


    do{
        _stat(de->d_name, &buf);
        if(is_same_wildcard_str(argv[1], de->d_name, de->d_type)){
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
        }
    }while ((de = readdir(dp)) != NULL);

    if(!filecnt && !dircnt){
        printf("������ ã�� �� �����ϴ�.\n");
        return;
    }
    GetDiskFreeSpaceEx(pathname, NULL, NULL, &free_memory); //��ũ �뷮 ���� �ޱ�
    add_comma(filesum, strnum);
    printf("\t%8d�� ����\t%19s ����Ʈ\n", filecnt, strnum);
    add_comma((LONGLONG)free_memory.QuadPart, strnum);
    printf("\t%8d�� ���͸� %16s ����Ʈ ����\n", dircnt, strnum);
    closedir(dp);
}

int charcmp(char c1, char c2) {
    if (c1 >= 65 || c1 <= 90 || c2 >= 65 || c2 <= 90) { //�����϶� ����
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
    
    if(type==DT_DIR) return 1;//������ Ȯ���ڰ� �����ϱ� �׳� 1 ����
    if(strstr(str, ".") && !is_last_str(str, ".")) return 0;//.�� �ְ� .���� ������ ������ 0 ����
    while (argv[argv_index] != '.' && str[str_index] != '\0') {

        if (argv[argv_index] == '*') {
            argv_index++;
            if(argv[argv_index] == '.') return 1;//*�� ������ ���ڸ� return 1;
            while (!charcmp(argv[argv_index], str[str_index])) { //���ϵ�ī�� ���� ���ڿ� ���� ���ڸ� ã��������
                if(str[str_index]=='\0') break;//str ������ �������� ���߱�
                str_index++; // ���ϵ�ī�� ���� ���� ���ڰ� ���ö����� str�ε��� �ڷ� �ѱ��
            }
        }

        if (!charcmp(argv[argv_index], str[str_index])) {
            return 0;//���� argv���ڶ� str���ڶ� ���� �ʴٸ� 0 ����
        }

        argv_index++; str_index++;
    }

    if(argv_index=='.' && str_index=='\0'){//while���� ���������� ������?
        return 1;
    }else{
        return 0;
    }
}

int is_file_or_dir(char *filename){ //�������� ������ �Ǵ�
    struct _finddatai64_t c_file;
    int hFile, result;

                    //3�Ⱑ �̻��� ������ �ٷ�� �Լ�, ������ �˻��Ѵ�.
    if ( (hFile = _findfirsti64(filename, &c_file)) == -1 )
        result = OTHER; // ���� �Ǵ� ���丮�� ������ -1 ��ȯ
    else if (c_file.attrib & _A_SUBDIR) // c_file.attrib : ������ Ư���� ��Ÿ��
        result = DR; // ���丮�� 0 ��ȯ
    else
        result = FL; // �׹��� ���� "�����ϴ� ����"�̱⿡ 1 ��ȯ

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
