#include<signal.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/shm.h>

void myfun(int signum)
{
    return;
}

int main()
{
    int shmid;
    char *p;
    int pid;

    shmid = shmget(IPC_PRIVATE,128,IPC_CREAT|0777);
    if(shmid < 0)
    {
        printf("create share memory failure\n");
        return -1;
    }
    printf("create share memory success shmid=%d\n",shmid);
    pid = fork();
    if(pid > 0)
    {
       p = (char *)shmat(shmid,NULL,0);
       if(p == NULL)
       {
           printf("shmat function failed\n");
           return -3;
       }
       while(1)
       {
           //write share memory
           signal(SIGUSR2,myfun);
           printf("parent process start write share memory:\n");
           fgets(p,128,stdin);   
           kill(pid,SIGUSR1); //child process read data
           pause(); // wait child read

       }

    }
    if(pid == 0)
    {
        signal(SIGUSR1,myfun);
        // start read share memory
        p = (char*)shmat(shmid,NULL,0);
        if(p == NULL)
        {
            printf("child process shmat function failed\n");
            return -3;
        }
        while(1)
        {
            pause();
            printf("share memory data:%s",p);
            kill(getppid(),SIGUSR2);
        }

    }
    shmdt(p);
    shmctl(shmid,IPC_RMID,NULL);
    system("ipcs -m"); 
   
   
    return 0;

}