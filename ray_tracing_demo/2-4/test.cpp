#include<windows.h>
#include <iostream>
#include <chrono>
#include<fstream>
#include <thread>
using namespace std;

int number = 1;

int ThreadProc1()
{
    while (number < 100)
    {
        cout << "thread 1 :" << number << endl;
        ++number;
      this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    return 0;
}

int ThreadProc2()
{
    while (number < 100)
    {
        cout << "thread 2 :" << number << endl;
        ++number;
        this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    return 0;
}

int main()
{
    thread t1(ThreadProc1);
    thread t2(ThreadProc2);

    t1.join();
    t2.join();
    for(int i=1;i<100;i++)
    {
        cout<<"main "<<i<<endl;;
        this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    ofstream out("ray_tracing.dat");
    double a1=0.0213;
    int t=10;
    out<<t<<" "<<a1;
    system("pause");
    return 0;
}