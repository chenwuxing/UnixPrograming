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
  
    
    signal(SIGUSR1,myfun);
    p = (struct mybuf *)shmat(shmid,NULL,0);
    if(p == NULL)
    {
        printf("shmat function failed\n");
        return -3;
    }

    // 读取服务器端放入共享内存的pid,然后写入自己的pid
    pid = p->pid;
    // 将客户端pid写入共享内存
    p->pid = getpid();
    // kill signal
    kill(pid,SIGUSR2);


    // 客户端开始读
    while(1)
    {
        pause();
        printf("client process receive data from share memory:%s\n",p->buffer);
        kill(pid,SIGUSR2);  // 服务器端可以写了
    }
   

    shmdt(p);
    shmctl(shmid,IPC_RMID,NULL);
    system("ipcs -m"); 
   
   
    return 0;

}