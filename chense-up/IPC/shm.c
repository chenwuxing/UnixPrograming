#include<signal.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/shm.h>

int main()
{
    int shmid;
    int key;
    char *p;

    key = ftok("./a.c",'b');
    if(key < 0)
    {
        printf("create key failed\n");
        return -2;
    }
    printf("create key success,key=%X\n",key);
    shmid = shmget(key,128,IPC_CREAT|0777);
    if(shmid < 0)
    {
        printf("create share memory failure\n");
        return -1;
    }
    printf("create share memory success shmid=%d\n",shmid);
    system("ipcs -m");

    p = (char*)shmat(shmid,NULL,0);
    if(p == NULL)
    {
        printf("shmat function failed\n");
        return -3;
    }
    //write share memory
    fgets(p,128,stdin);

    // start read share memory
    printf("share memory data:%s",p);
    return 0;

}