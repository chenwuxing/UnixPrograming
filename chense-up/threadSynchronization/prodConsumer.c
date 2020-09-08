#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>

sem_t empty,full;  //定义全局同步信号量empty，full
pthread_mutex_t mutex;  // 定义一个全局互斥量，用于缓冲区的互斥访问
int bufferCount = 0;        //定义一个全局变量，便是管道内的产品数目

void* producer(void *arg)
{
    while(1)
    {
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        printf("producer put a product to buffer.");
        bufferCount++;
        printf("the bufferCount is %d\n",bufferCount);
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    }
}
void* consumer(void *arg)
{
    while(1)
    {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        printf("consumer get a product from buffer.");
        bufferCount--;
        printf("the bufferCount is %d\n",bufferCount);
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
    }
}

int main()
{
    pthread_t thrdProd,thrdCons;
    pthread_mutex_init(&mutex,NULL);
    sem_init(&empty,0,5);
    sem_init(&full,0,0);

    if(pthread_create(&thrdProd,NULL,producer,NULL) != 0)
        printf("thread create error");
    if(pthread_create(&thrdCons,NULL,consumer,NULL) != 0)
        printf("thread create error");
    
    // 等待线程结束
    if(pthread_join(thrdProd,NULL) != 0)
        printf("wait thread failed.");
    if(pthread_join(thrdCons,NULL) != 0)
        printf("wait thread failed");
    sem_destroy(&full);
    sem_destroy(&empty);
    pthread_mutex_destroy(&mutex);
    return 0;
}