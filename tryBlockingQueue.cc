#include<iostream>
#include<thread>
#include<mutex>
#include<condition_variable>
#include<queue>
#include<vector>
#include<sstream>
#include "tryBlockingQueue.h"

// 产品标记
static unsigned int prod = 0;

// 递归打印
void showall() { std::cout<<std::endl; }

template <typename R1 ,typename...  Args>
void showall(R1 var, Args...args) {
    std::cout << var;
    showall(args...);
}

void tryBlockingQueue::consume() {
    while (true) {
        std::unique_lock<std::mutex> lg(mut);
        if (Q.size() == 0) {
            showall("buffer empty. waiting...");
            empty.wait(lg);
        }
        showall("consume ", Q.front());
        // 为了让线程有机会切换，睡眠一段时间
        Q.pop();
        std::this_thread::sleep_for( std::chrono::milliseconds(80));
        full.notify_one();
    }
}

void tryBlockingQueue::produce() {
    while (true) {
        std::unique_lock<std::mutex> lg(mut);
        showall("produce ", prod);
        Q.push(prod++);
        std::this_thread::sleep_for( std::chrono::milliseconds(80));
        empty.notify_one();
        if (Q.size() == bufferSize) {
            showall("buffer full. waiting...");
            full.wait(lg);
        }
    }
}


int main() {
    tryBlockingQueue bg;
    // 传递对象的引用，才不会拷贝
    std::thread Producer(&tryBlockingQueue::produce, &bg );
    std::thread Consumer(&tryBlockingQueue::consume, &bg );

    Producer.join();
    Consumer.join();


    return 0;
}
