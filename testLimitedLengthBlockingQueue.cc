#include "limitedLengthBlockingQueue.h"

// 产品标记
static  int prod = 0;

void getOne(limitedLengthBlockingQueue< int>* q) {
    while(true) {
        q->push(prod++);
    }
}

void putOne(limitedLengthBlockingQueue< int>* q) {
    while(true) {
        q->pop();
    }
}

int main() {
    limitedLengthBlockingQueue< int> bg;
    // 传递对象的引用，才不会拷贝
    std::thread Producer(getOne, &bg);
    std::thread Consumer(putOne, &bg);

    Producer.join();
    Consumer.join();


    return 0;
}
