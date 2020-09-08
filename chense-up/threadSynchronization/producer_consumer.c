#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>

typedef struct MSG
{
    struct MSG *next;
    struct MSG *pre;
    int id;

}msg_t;

msg_t *phead = NULL, *ptail = NULL;

// 静态初始化，也可以用pthread_cond_init(&hasproduct,NULLL);进行初始化
pthread_cond_t hasProduct = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

// 生产者
void* producter(void *p)
{
    msg_t *mg = NULL;
    while(1)
    {
        pthread_mutex_lock(&lock);
        //生产产品
        mg = (msg_t*)malloc(sizeof(msg_t));
        scanf("%d",&mg->id);
        printf("product id = %d\n",mg->id);
        //加入到生产队列里
        mg->next = NULL;
        mg->pre = ptail;
        ptail->next = mg;
        ptail = mg;
        pthread_mutex_unlock(&lock);
        //唤醒至少一个阻塞在条件变量上的线程
        pthread_cond_signal(&hasProduct);
    }
    return NULL;
}

//消费者
void* consumer(void *p)
{
    msg_t *mg = NULL;
    while(1)
    {
        pthread_mutex_lock(&lock);
        while(phead == ptail)
        {
            //注意这里不能是if，因为会有多个消费者争夺资源
            //当队列没有产品的时候会阻塞且释放锁直到被唤醒
            // 有产品被唤醒的时候会返回且重新获得锁
            pthread_cond_wait(&hasProduct,&lock);
        }
        printf("consume id = %d\n",ptail->id);
        //消费一个产品
        ptail = ptail->pre;
        free(ptail->next);
        ptail->next = NULL;
        pthread_mutex_unlock(&lock);
    }
}

int main()
{
    phead = (msg_t*)malloc(sizeof(msg_t));
    ptail = phead;
    pthread_t pid,cid;
    pthread_create(&pid,NULL,producter,NULL);
    pthread_create(&cid,NULL,consumer,NULL);

    pthread_join(pid,NULL);
    pthread_join(cid,NULL);
    return 0;
}