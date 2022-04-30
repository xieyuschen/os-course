#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(void)
{
    pid_t pid;
    pid = fork();
    if(pid < 0)
        printf("子进程创建失败\n");
    else if(pid > 0 )
    {
        printf("我是父进程,我的进程号是: %d\n", getpid());
        printf("子进程号为: %d\n", pid);
        sleep(1);
    }
    else {
        printf("我是子进程,我的进程号是: %d\n", getpid());
        printf("我的父进程的进程号是: %d", getppid()) ;
    }
}