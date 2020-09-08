#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>

int bufferCount = 0;
// 对共享缓冲区的互斥访问
pthread_mutex_t mutex;
// 阻塞线程
pthread_cond_t cond;

void* producer(void *arg)
{
    while(1)
    {
        pthread_mutex_lock(&mutex);
        printf("producer put a product to buffer.");
        bufferCount++;
        printf("the bufferCount is %d\n",bufferCount);
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&cond);
      
    }
    
}

void* consumer(void *arg)
{
    while(1)
    {
        pthread_mutex_lock(&mutex);
        while(bufferCount == 0)
        {
            //函数会对互斥锁进行解锁
            pthread_cond_wait(&cond,&mutex);
            //解除阻塞后，会对互斥锁进行加锁
        }
        printf("consumer get a product from buffer.");
        bufferCount--;
        printf("the bufferCount is %d\n",bufferCount);
        pthread_mutex_unlock(&mutex);

    }
}

int main()
{
    pthread_t thrdProc,thrdCons;
    if(pthread_create(&thrdProc,NULL,producer,NULL) != 0)
        printf("create thread failed.");
    if(pthread_create(&thrdCons,NULL,consumer,NULL) != 0)
        printf("create thread failed.");
    
    //等待线程结束
    if(pthread_join(thrdProc,NULL) != 0)
        printf("wait thread failed.");
    if(pthread_join(thrdCons,NULL) != 0)
        printf("wait thread failed.");
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    
}



