#include <stdio.h>
#include <windows.h>
HANDLE hFile;
CRITICAL_SECTION cs;
DWORD WINAPI ThreadA(LPVOID lpParam)
{
    DWORD dwWrite;
    for (int i = 0; i < 1001; i++)
    {
        printf("线程A: %d\n",i);
        Sleep(500);
    }

    return 0;
}
DWORD WINAPI ThreadB(LPVOID lpParam)
{
    DWORD dwWrite;
    for (int i = 1000; i > -1; i--)
    {
        printf("线程B: %d\n", i);
        Sleep(500);
    }

    return 0;
}
int main()
{
    DWORD ThreadID;
    HANDLE hThread[5];
    hThread[0] = CreateThread(NULL, 0, ThreadA, (LPVOID)1, 0, &ThreadID);
    hThread[1] = CreateThread(NULL, 0, ThreadB, (LPVOID)1, 0, &ThreadID);
    WaitForMultipleObjects(2, hThread, TRUE, INFINITE);
    return 0;
}
