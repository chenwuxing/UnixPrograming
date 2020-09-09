#include<signal.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/msg.h>


struct msgbuf
{
    long type;
    char voltage[124];
    char ID[4];
};

int main()
{
    int msgid;
    int readret;
    int key;
    key = ftok("./a.c",'a');
    if(key < 0)
    {
        printf("create key failed\n");
        return -2;
    }

    struct msgbuf sendBuf,recvBuf;
    msgid = msgget(key,IPC_CREAT|0777);
    if(msgid < 0)
    {
        printf("create message queue failed\n");
        return -1;
    }
    printf("create message queue success msgid = %d\n",msgid);
    system("ipc -q");

    sendBuf.type = 100;
    // read message queue
    while(1)
    {
        memset(recvBuf.voltage,0,124);   //清空接收缓存
       
        //start read message to message queue
        // 124指想读多长的数据，100是消息类型，0是阻塞读
        msgrcv(msgid,(void*)&recvBuf,124,100,0);
        printf("receive data from message queue:%s",recvBuf.voltage);

    }

    msgctl(msgid,IPC_RMID,NULL);
    system("ipcs -q");
    return 0;
    

}