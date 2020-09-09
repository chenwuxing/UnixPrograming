#include<stdio.h>
#include<sys/types.h>
#include<signal.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>

void myfunc(int signum)
{
    int i;
    i = 0;
    while(i < 5)
    {
        printf("receive signum=%d,i=%d\n",signum,i);
        sleep(1);
        i++;

    }
    return;
}

void myfunc1(int signum)
{
    printf("receive signum=%d\n",signum);
    wait(NULL);
    return;
}
int main()
{
    pid_t pid;
    pid = fork();
    if(pid > 0)
    {
        int i;
        i = 0;
        signal(10,myfunc);
        signal(17,myfunc1);
        while(1)
        {
            printf("parent process things ,i = %d\n",i);
            sleep(1);
            i++;
        }
    }
    if(pid == 0)
    {
        sleep(10);
        kill(getppid(),10);
        sleep(10);
        kill(getppid(),17);

    }
}