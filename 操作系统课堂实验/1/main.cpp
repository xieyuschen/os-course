#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<time.h>
#define LINE 1024//һ������ַ���

const char* filepath = "CommandLine.txt";
char nowtime[64];//�洢��ȡ�ĵ�ǰʱ��
void getCurrentTime()
{
	struct tm t;   //tm�ṹָ��

	time_t now;  //����time_t���ͱ���
	time(&now);      //��ȡϵͳ���ں�ʱ��

	localtime_s(&t, &now);   //��ȡ�������ں�ʱ��

	strftime(nowtime, sizeof(nowtime), "%Y%m%d\0", &t);     //������ ->nowtime
}

//�ú������ڽ��ļ���һ�иı�
void changeData(FILE* fp)
{
	//���ã�����һ�������ļ����ı��һ�У��ٰ��޹��и��ƹ�ȥ��
	//Ȼ��ɾ��ԭ�ļ����������ļ�����Ϊԭ�ļ���
	char tmpname[] = "tmp";//��ʱ���ļ���
	FILE* fpNew;
	//����ʱ�ļ�
	errno_t err;
	if ((err = fopen_s(&fpNew, tmpname, "w")) != 0)//w��ʾ���ļ���д
	{
		printf("�ļ���ʧ�ܣ�");
		exit(1);
	}

	char buf[LINE] = "���ļ����һ�δ򿪺�����������";
	getCurrentTime();//����ʱ�� ��ʱ�丳��nowtime[]
	strcat_s(buf, nowtime);//��ʱ�����ӵ�����
	strcat_s(buf, "\n");//ĩβ��һ������

	char* p;
	fputs(buf, fpNew);//��buf������������ļ���

	fgets(buf, LINE, fp);//�����ȡ��һ��ֻ��Ϊ��ָ��ָ����һ��
	/*
		����һ�£�fgets()��һ�����л�ѵ�һ�ж��������ڶ������л�����ڶ���
		������������һ�Σ�����Ϊ����ֻ�ӵڶ��п�ʼ
	*/
	//ѭ����ԭ�ļ�2-n�и��Ƶ���ʱ�ļ�
	while (1)
	{
		p = fgets(buf, LINE, fp);
		if (p == NULL)
			break;//�ж��Ƿ��ȡ��
		fputs(buf, fpNew);
	}

	fclose(fp);
	fclose(fpNew);
	remove(filepath); //ɾ��ԭ�ļ�
	rename(tmpname, filepath); //���������ļ�
}
int main()
{

	FILE* fp;
	errno_t err;
	if ((err = fopen_s(&fp, filepath, "r")) != 0)//r��ʾ���ļ��ɶ�
	{
		printf("�ļ���ʧ�ܣ�");
		return 0;
	}

	//�޸ĵ�һ�У� ���ļ����һ�δ򿪺�����������~~~~
	changeData(fp);
	//��Ϊfgets��ÿ�ζ�ȡһ���Ҳ��ܻ�ͷ��ֻ���ٴ�һ���ļ���ʹ��fgets��ȡ���ǵ�һ��
	if ((err = fopen_s(&fp, filepath, "r")) != 0)//r��ʾ���ļ��ɶ�
	{
		printf("�ļ���ʧ�ܣ�");
		return 0;
	}
	char buf[LINE];
	char* p;
	//��ȡһ�µ�һ��������
	p = fgets(buf, LINE, fp);

	while (1)
	{
		p = fgets(buf, LINE, fp);
		if (p == NULL)
			break;

		//���ｲһ�£���Ϊfgets,���ÿ�����Ļ��з�'\n'Ҳ��ȡ�����������������в��˵ģ�
		//�ð�'\n'ɾ������'\0'����β
		int length = strlen(buf);
		if (buf[length - 1] == '\n')
			buf[length - 1] = '\0';

		//���̵Ĳ���
		STARTUPINFO si = { sizeof(si) };
		PROCESS_INFORMATION pi;
		BOOL ret;
		si.dwFlags = STARTF_USESHOWWINDOW;//ָ��wShowWindow��Ա��Ч
		si.wShowWindow = TRUE;//�Ƿ���ʾ���������Ľ��̴���
		TCHAR command[LINE];
		MultiByteToWideChar(CP_ACP, 0, buf, -1, command, LINE);

		//����һ�����̣�����������ִ��һ������
		ret = CreateProcess(NULL, command, NULL, NULL, FALSE,
			CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
		if (ret == false)
			printf("���̴���ʧ��");
	}

	return 0;
}
