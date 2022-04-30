#include <windows.h>
#include <cstdlib>
#include <time.h>
#include <easyx.h>
#include <graphics.h>
#include <conio.h>
#define MAX 5
#define RADIUS 100
//圆的中心坐标
#define circle_x 480
#define circle_y 180
//画方线程
DWORD WINAPI draw_rectangle(void)
{
    for(int i=0;i< 180; i=i+1.5)
    {
        //画横线
        line(30+i,30,31.5+i,30);

        line(30+i,210,31.5+i,210);

        //画竖线
        line(30,30+i,30,31.5+i);

        line(210,30+i,210,31.5+i);
        Sleep(100*(rand() % 4));
    }
    return 0;
}
//画圆线程
DWORD WINAPI draw_circle(void)
{
    for(int i=0;i< 180; i++)
    {
        putpixel(circle_x+RADIUS*cos(0.5*i),circle_y+RADIUS*sin(0.5*i),RED);
        putpixel(circle_x-RADIUS*cos(0.5*i),circle_y-RADIUS*sin(0.5*i),RED);
        putpixel(circle_x+RADIUS*cos(0.5*i),circle_y-RADIUS*sin(0.5*i),RED);
        putpixel(circle_x-RADIUS*cos(0.5*i),circle_y+RADIUS*sin(0.5*i),RED);
        Sleep(100*(rand() % 4));
    }
    return 0;
}
int main(void)
{
    //设置随机数
    srand((unsigned) time(NULL));
    //初始化画图区域
    initgraph(640, 480);
    //设置颜色
    setcolor(WHITE);
    fillrectangle(0,0,640,480);
    setlinecolor(RED);
    //准备创建多线程
    HANDLE rectThread[MAX];
    HANDLE circleThread[MAX];
    DWORD ThreadID;
    for(int i=0; i < MAX; i++)
    {
        rectThread[i]=CreateThread(NULL, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(draw_circle), NULL, 0, &ThreadID);
        circleThread[i]=CreateThread(NULL, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(draw_rectangle), NULL, 0, &ThreadID);
    }
    //等待线程结束
    WaitForMultipleObjects(5,rectThread, TRUE, INFINITE);
    WaitForMultipleObjects(5, circleThread, TRUE,INFINITE);
    //按下按键关闭图像
    getch();
    closegraph();
    return 0;
}

