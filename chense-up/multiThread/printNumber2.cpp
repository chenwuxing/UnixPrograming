#include<iostream>
#include<pthread.h>

using namespace std;

int number;
pthread_mutex_t mutex;
const int MAXNUM = 10;

void* func1(void *arg)
{
    while(1)
    {
        pthread_mutex_lock(&mutex);
        if(number >= MAXNUM)
        {
            pthread_mutex_unlock(&mutex);
            break;
        }
        if(number % 2 == 0)
        {
            number++;
            cout<<"thread1: "<<number<<endl;
        }
        pthread_mutex_unlock(&mutex);
        
    }
    cout<<"thread1 finished"<<endl;

}

void* func2(void *arg)
{
    while(1)
    {
        pthread_mutex_lock(&mutex);
        if(number >= MAXNUM)
        {
            pthread_mutex_unlock(&mutex);
            break;
        }
        if(number % 2 != 0)
        {
            number++;
            cout<<"thread2: "<<number<<endl;
        }
        pthread_mutex_unlock(&mutex);
    }
    cout<<"thread2 finished"<<endl;
}


int main()
{
    pthread_t thread1,thread2;
    pthread_mutex_init(&mutex,NULL);

    if(pthread_create(&thread1,NULL,func1,NULL) != 0)
        cout<<"thread create error"<<endl;
    if(pthread_create(&thread2,NULL,func2,NULL) != 0)
        cout<<"thread create error"<<endl;
    
    if(pthread_join(thread1,NULL) != 0)
        cout<<"wait thread failed"<<endl;
    if(pthread_join(thread2,NULL) != 0)
        cout<<"wait thread failed"<<endl;
    pthread_mutex_destroy(&mutex);

}