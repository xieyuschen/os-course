#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<time.h>
#define LINE 1024//一行最大字符数

const char* filepath = "CommandLine.txt";
char nowtime[64];//存储获取的当前时间
void getCurrentTime()
{
	struct tm t;   //tm结构指针

	time_t now;  //声明time_t类型变量
	time(&now);      //获取系统日期和时间

	localtime_s(&t, &now);   //获取当地日期和时间

	strftime(nowtime, sizeof(nowtime), "%Y%m%d\0", &t);     //年月日 ->nowtime
}

//该函数用于将文件第一行改变
void changeData(FILE* fp)
{
	//采用：另创建一个备份文件，改变第一行，再把无关行复制过去，
	//然后删除原文件，讲备份文件改名为原文件名
	char tmpname[] = "tmp";//临时新文件名
	FILE* fpNew;
	//打开临时文件
	errno_t err;
	if ((err = fopen_s(&fpNew, tmpname, "w")) != 0)//w表示对文件可写
	{
		printf("文件打开失败！");
		exit(1);
	}

	char buf[LINE] = "本文件最后一次打开和运行日期是";
	getCurrentTime();//加载时间 将时间赋给nowtime[]
	strcat_s(buf, nowtime);//将时间连接到后面
	strcat_s(buf, "\n");//末尾加一个换行

	char* p;
	fputs(buf, fpNew);//将buf里的内容输入文件中

	fgets(buf, LINE, fp);//这里读取第一行只是为了指针指向下一行
	/*
		解释一下，fgets()第一次运行会把第一行读出来，第二次运行会读出第二行
		所以这里运行一次，是因为拷贝只从第二行开始
	*/
	//循环将原文件2-n行复制到临时文件
	while (1)
	{
		p = fgets(buf, LINE, fp);
		if (p == NULL)
			break;//判断是否读取完
		fputs(buf, fpNew);
	}

	fclose(fp);
	fclose(fpNew);
	remove(filepath); //删除原文件
	rename(tmpname, filepath); //重命名新文件
}
int main()
{

	FILE* fp;
	errno_t err;
	if ((err = fopen_s(&fp, filepath, "r")) != 0)//r表示对文件可读
	{
		printf("文件打开失败！");
		return 0;
	}

	//修改第一行： 本文件最后一次打开和运行日期是~~~~
	changeData(fp);
	//因为fgets而每次读取一行且不能回头，只能再打开一遍文件，使得fgets读取的是第一行
	if ((err = fopen_s(&fp, filepath, "r")) != 0)//r表示对文件可读
	{
		printf("文件打开失败！");
		return 0;
	}
	char buf[LINE];
	char* p;
	//读取一下第一行跳过他
	p = fgets(buf, LINE, fp);

	while (1)
	{
		p = fgets(buf, LINE, fp);
		if (p == NULL)
			break;

		//这里讲一下，因为fgets,会把每句后面的换行符'\n'也读取，但是这样子是运行不了的，
		//得把'\n'删掉，用'\0'做结尾
		int length = strlen(buf);
		if (buf[length - 1] == '\n')
			buf[length - 1] = '\0';

		//进程的参数
		STARTUPINFO si = { sizeof(si) };
		PROCESS_INFORMATION pi;
		BOOL ret;
		si.dwFlags = STARTF_USESHOWWINDOW;//指定wShowWindow成员有效
		si.wShowWindow = TRUE;//是否显示创建出来的进程窗口
		TCHAR command[LINE];
		MultiByteToWideChar(CP_ACP, 0, buf, -1, command, LINE);

		//创建一个进程，可以用它来执行一个程序
		ret = CreateProcess(NULL, command, NULL, NULL, FALSE,
			CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
		if (ret == false)
			printf("进程创建失败");
	}

	return 0;
}
