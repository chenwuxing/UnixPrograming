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

struct mybuf
{
    int pid;
    char buffer[124];
};

int main()
{
    int shmid;
    struct mybuf *p;
    int pid;
    int key;
    key = ftok("./a.c",'a');
    if(key < 0)
    {
        printf("create key failed\n");
        return -1;
    }
    printf("create key success\n");

    shmid = shmget(key,128,IPC_CREAT|0777);
    if(shmid < 0)
    {
        printf("create share memory failure\n");
        return -1;
    }
    printf("create share memory success shmid=%d\n",shmid);
  
    
    signal(SIGUSR2,myfun);
    p = (struct mybuf *)shmat(shmid,NULL,0);
    if(p == NULL)
    {
        printf("shmat function failed\n");
        return -3;
    }
    // 向共享内存中写入服务器pid，等待客户端读取
    p->pid = getpid();
    pause();
    pid = p->pid;

    while(1)
    {
        //write share memory
        signal(SIGUSR2,myfun);
        printf("parent process start write share memory:\n");
        fgets(p->buffer,128,stdin);   
        kill(pid,SIGUSR1); //child process read data
        pause(); // wait child read

    }

    
   
    shmdt(p);
    shmctl(shmid,IPC_RMID,NULL);
    system("ipcs -m"); 
   
   
    return 0;

}