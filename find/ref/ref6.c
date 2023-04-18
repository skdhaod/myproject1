#include <stdio.h>
#include <io.h>

// �������� ���丮���� �Ǵ� �Լ�
int isFileOrDir(char* s);

int main(void) {
  char* filename = "ref.c"; // ���⿡ �����̳� ������ �Է�

  int result = isFileOrDir(filename);

  if (result == 1)
    puts("�����Դϴ�.");
  else if (result == 0)
      puts("���丮�Դϴ�.");
  else if (result == -1)
      puts("�׷� ���� �Ǵ� ���丮�� �����ϴ�.");

  return 0;
}

int isFileOrDir(char* s) {
  struct _finddatai64_t c_file;
  int hFile, result;

  if ( (hFile = _findfirsti64(s, &c_file)) == -1L )
    result = -1; // ���� �Ǵ� ���丮�� ������ -1 ��ȯ
  else if (c_file.attrib & _A_SUBDIR)
      result = 0; // ���丮�� 0 ��ȯ
  else
    result = 1; // �׹��� ���� "�����ϴ� ����"�̱⿡ 1 ��ȯ

  _findclose(hFile);
  return result;
}