#ifndef LIMITEDLENTHBLOCKINGQUEUE_H
#define LIMITEDLENTHBLOCKINGQUEUE_H

#include "print.h"

// 有限长度的Queue
template<typename T>
class limitedLengthBlockingQueue {
public:
    limitedLengthBlockingQueue(int bufSize = 500) : bufferSize(bufSize) {};
    //防止拷贝
    limitedLengthBlockingQueue(const limitedLengthBlockingQueue&) = delete;
    limitedLengthBlockingQueue& operator=(const limitedLengthBlockingQueue &) = delete;

    void pop() {
        std::unique_lock<std::mutex> lg(mut);
        if (Q.size() == 0) {
            showall("buffer empty. waiting...");
            empty.wait(lg);
        }
        showall("consume ", Q.front());
        T t = Q.front();
        // 为了让线程有机会切换，睡眠一段时间
        Q.pop();
        std::this_thread::sleep_for( std::chrono::milliseconds(80));
        full.notify_one();
       // return t;
    }

    void push(T t) {
        std::unique_lock<std::mutex> lg(mut);
        showall("produce ", t);
        Q.push(t);
        std::this_thread::sleep_for( std::chrono::milliseconds(80));
        empty.notify_one();
        if (Q.size() == bufferSize) {
            showall("buffer full. waiting...");
            full.wait(lg);
        }
    }
private:
    std::mutex mut;
    std::condition_variable empty;
    std::condition_variable full;
    std::queue<T> Q;
    int bufferSize = 500;
};

#endif
