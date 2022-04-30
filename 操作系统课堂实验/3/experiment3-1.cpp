#include<stdio.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<signal.h>
#include<unistd.h>

#define BUF_SIZE 100
#define SHM_SIZE 1024

void handler1(int sig)
{
    if ( SIGUSR1 == sig )
    {
        printf("父进程信号传递成功\n");
    }
}

void handler2(int sig)
{
    if ( SIGUSR2 == sig )
    {
        printf("子进程信号传递成功\n");
    }
}

int main(void)
{
    key_t key = 0;
    int ret = 0;
    int shmid = 0;
    pid_t pid;
    char * pShm = NULL;
    char buf[BUF_SIZE] = {'\0'};

    //key
    key = ftok( "/tmp", 23 );
    if ( 0 > key )
    {
        perror("ftok error");
        return -1;
    }

    //创建并且打开共享内存
    shmid = shmget(key, SHM_SIZE, 0644 | IPC_CREAT);
    if ( 0 > shmid )
    {
        perror("shmget error");
        return -1;
    }

    //映射
    pShm = static_cast<char *>(shmat(shmid, NULL, 0));
    if ( (void *)-1  == pShm )
    {
        perror("shmat error");
        shmctl(shmid, IPC_RMID, NULL);
        return -1;
    }

    //创建子进程
    pid = fork();
    if ( 0 > pid )
    {
        perror("fork error");
        shmctl(shmid, IPC_RMID, NULL);
        return -1;
    }
    else if ( 0 < pid )
    {
        //安装信号
        signal(SIGUSR2, handler2);
        while(1)
        {
            printf("父进程对子进程说:");
            //向共享内存中写入数据
            fgets(pShm, SHM_SIZE, stdin);
            //数据写入完成，向子进程发送信号
            kill(pid,SIGUSR1);

            //等待子进程信号
            pause();

            //输入#时结束父程序，并向子进程发送信号
            if ( '#' == pShm[0] )
            {
                kill(pid, SIGUSR1);
                break;
            }
        }
        printf("父进程结束!\n");

        //映射
        shmdt(pShm);

        //删除共享内存
        shmctl(shmid, IPC_RMID, NULL);
    }
    else
    {
        //获取父进程ID
        pid_t ppid;
        ppid = getppid();
        //初始化累加和
        int sum = 0;
        //安装信号
        signal(SIGUSR1, handler1);
        //等待父进程信号
        while(1)
        {
            //等待父进程写数据
            pause();
            //读取输出共享内存中的数据
            printf("子进程读取:%s\n",pShm);
            //数据写入完成，向父进程发送信号
            kill(ppid,SIGUSR2);
            //输入#时结束父程序，并向子进程发送信号
            if ( '#' == pShm[0] )
            {
                kill(ppid, SIGUSR2);
                break;
            }
        }
        printf("子进程结束!\n");
    }
    return 0;
}


