#include <iostream>
#include <easyx.h>
#include <graphics.h>
#include <conio.h>

void init(void);
void eatA();
void eatB();
void eatC();
void eatD();
void eatE();
void thinkA();
void thinkB();
void thinkC();
void thinkD();
void thinkE();
void stick1_down();
void stick2_down();
void stick3_down();
void stick4_down();
void stick5_down();
void stick1_up();
void stick2_up();
void stick3_up();
void stick4_up();
void stick5_up();
void eat(int i);
void think(int i);
void fork_down(int i);
void fork_up(int i);
void lockfunc(void);
void unlockfunc(void);
IMAGE eat_a,think_a,eat_b,think_b,eat_c,think_c,eat_d,think_d,eat_e,think_e,background,
        stick1down,stick1up,stick2down,stick2up,stick3up,stick3down,stick4down,stick4up,stick5down,stick5up,
        lock,unlock,exitprogram;
HANDLE fork[5];
//用于限制请求资源的哲学家数量的信号量
HANDLE mutex;
//定义哲学家吃饭最大次数
int times = 500;
//鼠标消息定义
MOUSEMSG m;
//线程句柄数组
HANDLE philosopherThread[5];
HANDLE philosopherunlockThread[5];
//定义存储线程ID的变量
DWORD ThreadID;
//会引发死锁的哲学家线程
DWORD WINAPI philosopher_lock(int i)
{
    int x= i;
    if(i == 5)
    x=0;
    while(times--){
        WaitForSingleObject(fork[i-1], INFINITE);
        fork_up(i-1);
        printf("第%d个哲学家拿起第 %d个叉子\n",i,i-1);
        Sleep((rand()%5+1)*1000);
        WaitForSingleObject(fork[x], INFINITE);
        fork_up(x);
        printf("第%d个哲学家拿起第 %d个叉子\n",i,x);
        Sleep((rand()%5+1)*1000);
        //进入临界区
        printf("第%d个哲学家正在吃饭\n",i);
        eat(i);
        //离开临界区
        ReleaseSemaphore(fork[i-1],1,NULL);
        fork_up(i-1);
        printf("第%d个哲学家放下第%d个叉子\n",i,i-1);
        Sleep((rand()%5+1)*1000);

        ReleaseSemaphore(fork[x],1,NULL);
        fork_up(x);
        printf("第%d个哲学家放下第%d个叉子\n",i,x);
        Sleep((rand()%5+1)*1000);
        think(i);
        printf("第%d个哲学家正在思考\n",i);
        Sleep((rand()%5+1)*1000);
        m=GetMouseMsg();
        if(m.x< 41+98 && m.x>41 && m.y>34 && m.y<34+67)
            break;
    }
    return 0;
}
//不会产生死锁的哲学家线程
DWORD WINAPI philosopher_unlock(int i)
{
    int x= i;
    if(i == 5)
        x=0;
    while(times--){
        WaitForSingleObject(mutex,INFINITE);

        WaitForSingleObject(fork[i-1], INFINITE);
            fork_up(i-1);
        printf("第%d个哲学家拿起第%d个叉子\n",i,i-1);
        Sleep((rand()%5+1)*100);
        WaitForSingleObject(fork[x], INFINITE);
            fork_up(x);
        printf("第%d个哲学家拿起第%d个叉子\n",i,x);
        Sleep((rand()%5+1)*100);
        //进入临界区
        printf("\t\t\t第%d个哲学家正在吃饭\n",i);
        eat(i);
        //离开临界区
        ReleaseSemaphore(fork[i-1],1,NULL);
            fork_down(i-1);
        printf("第%d个哲学家放下第%d个叉子\n",i,i-1);
        Sleep((rand()%5+1)*100);

        ReleaseSemaphore(fork[x],1,NULL);
            fork_down(x);
        printf("第%d个哲学家放下第%d个叉子\n",i,x);
        Sleep((rand()%5+1)*100);
        ReleaseSemaphore(mutex,1,NULL);
        think(i);
        printf("\t\t\t\t\t\t第%d个哲学家正在思考\n",i);
        Sleep((rand()%5+1)*100);

    }
    return 0;
}
int main() {
    init();
    //创建代表叉子的信号量
    for(int i=0; i< 5; i++)
    {
        fork[i] = CreateSemaphoreA(NULL,1, 1,NULL );
    }
    mutex = CreateSemaphoreA(NULL,4, 4,NULL );
    while(1)
    {
        m = GetMouseMsg();
        if(m.uMsg==WM_LBUTTONDOWN)
        {
            if(m.x<590+98 && m.x>590 && m.y>410 && m.y<410+67)
                unlockfunc();
            else if(m.x< 475+98 && m.x>475 && m.y>410 &&m.y<410+67)
                lockfunc();
        }

    }
    _getch();
    closegraph();
    return 0;
}
void lockfunc(void)
{
    for(int i=0; i < 5; i++)
    {
        philosopherThread[i]=CreateThread(NULL, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(philosopher_lock), (LPVOID)(i+1), 0, &ThreadID);
    }
    WaitForMultipleObjects(5,philosopherThread, TRUE, INFINITE);
}
void unlockfunc(void)
{
    for(int i=0 ; i < 5; i++) {
        philosopherunlockThread[i] = CreateThread(NULL, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(philosopher_unlock),(LPVOID) (i + 1), 0, &ThreadID);
    }
    WaitForMultipleObjects(5,philosopherunlockThread, TRUE, INFINITE);
}
void eat(int i)
{
    if (i == 0)
        eatA();
    else if(i == 1)
        eatB();
    else if(i == 2)
        eatC();
    else if(i == 3)
        eatD();
    else if(i == 4)
        eatE();
}
void think(int i)
{
    if(i ==0)
        thinkA();
    else if(i ==1)
        thinkB();
    else if(i == 2)
        thinkC();
    else if(i == 3)
        thinkD();
    else if(i == 4)
        thinkE();
}
void init(void)
{
    initgraph(700 ,510);
    loadimage(&background,_T("../image/test.png"));
    loadimage(&eat_a,_T("../image/eat_a.png"));
    loadimage(&think_a,_T("../image/think_a.png"));
    loadimage(&eat_b,_T("../image/eat_b.png"));
    loadimage(&think_b,_T("../image/think_b.png"));
    loadimage(&eat_c,_T("../image/eat_c.png"));
    loadimage(&think_c,_T("../image/think_c.png"));
    loadimage(&eat_d,_T("../image/eat_d.png"));
    loadimage(&think_d,_T("../image/think_d.png"));
    loadimage(&eat_e,_T("../image/eat_e.png"));
    loadimage(&think_e,_T("../image/think_e.png"));
    loadimage(&stick1down,_T("../image/stick1_down.png"));
    loadimage(&stick1up,_T("../image/stick1_up.png"));
    loadimage(&stick2down,_T("../image/stick2_down.png"));
    loadimage(&stick2up,_T("../image/stick2_up.png"));
    loadimage(&stick3up,_T("../image/stick3_up.png"));
    loadimage(&stick3down,_T("../image/stick3_down.png"));
    loadimage(&stick4down,_T("../image/stick4_down.png"));
    loadimage(&stick4up,_T("../image/stick4_up.png"));
    loadimage(&stick5up,_T("../image/stick5_up.png"));
    loadimage(&stick5down,_T("../image/stick5_down.png"));
    loadimage(&lock,_T("../image/lock.png"));
    loadimage(&unlock,_T("../image/unlock.png"));
    loadimage(&exitprogram,_T("../image/exit.png"));
    putimage(0,0,&background);
    putimage(590,410,&unlock);
    putimage(475,410,&lock);
    //putimage(41,34,&exitprogram);
    thinkA();
    thinkB();
    thinkC();
    thinkD();
    thinkE();
    stick1_down();
    stick2_down();
    stick3_down();
    stick4_down();
    stick5_down();
}
void eatE()
{
    putimage(30,166,&eat_e);
}
void thinkE()
{
    putimage(30,166,&think_e);
}
void eatD()
{
    putimage(230,41.5,&eat_d);
}
void thinkD()
{
    putimage(230,41.5,&think_d);
}
void eatC()
{
    putimage(420,162,&eat_c);
}
void thinkC()
{
    putimage(420,162,&think_c);
}
void eatB()
{
    putimage(385,387,&eat_b);
}
void thinkB()
{
    putimage(385,387,&think_b);
}
void eatA()
{
    putimage(64,383,&eat_a);
}
void thinkA()
{
    putimage(64,383,&think_a);
}
void stick1_down()
{
    putimage(268,332,&stick1down);
}
void stick1_up()
{
    putimage(268,332,&stick1up);
}
void stick2_down()
{
    putimage(340,268,&stick2down);
}
void stick2_up()
{
    putimage(340,268,&stick2up);
}
void stick3_down()
{
    putimage(306,145,&stick3down);
}
void stick3_up()
{
    putimage(306,145,&stick3up);
}
void stick4_down()
{
    putimage(192,141.5,&stick4down);
}
void stick4_up()
{
    putimage(192,141.5,&stick4up);
}
void stick5_down()
{
    putimage(136,268,&stick5down);
}
void stick5_up()
{
    putimage(136,268,&stick5up);
}
void fork_down(int i)
{
    if(i == 1)
        stick1_down();
    else if(i == 2)
        stick2_down();
    else if(i == 3)
        stick3_down();
    else if(i == 4)
        stick4_down();
    else stick5_down();
}
void fork_up(int i)
{
    if(i == 0)
        stick1_up();
    else if(i == 1)
        stick2_up();
    else if(i == 2)
        stick3_up();
    else if(i == 3)
        stick4_up();
    else stick5_up();
}

