#include<iostream>
#include<thread>
#include<mutex>

using namespace std;


int number;
mutex mutex_number;

const int MAXNUM = 10;

void func1()
{
    while(1)
    {
        mutex_number.lock();
        if(number >= MAXNUM)
        {
            mutex_number.unlock();
            break;
        }
        if(number % 2 == 0)
        {
            number++;
            cout<<"thread1: "<<number<<endl;
        }
        mutex_number.unlock();
    }
    cout<<"thread1 finished"<<endl;
}

void func2()
{
    while(1)
    {
        mutex_number.lock();
        if(number >= MAXNUM)
        {
            mutex_number.unlock();
            break;
        }
        if(number % 2 == 1)
        {
            number++;
            cout<<"thread2: "<<number<<endl;
        }
        mutex_number.unlock();
    }
    cout<<"thread2 finished"<<endl;
}


int main()
{
    number = 0;
    cout<<"create and start!"<<endl;

    thread thread1(func1);
    thread thread2(func2);
    
    thread1.join();
    thread2.join();

    cout<<"finished and exit"<<endl;
    
}