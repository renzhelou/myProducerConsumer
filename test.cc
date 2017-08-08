#include<iostream>
#include<thread>
#include<mutex>
#include<condition_variable>
#include<queue>
#include<vector>
#include<sstream>

std::mutex mut;
std::condition_variable empty, full;
std::queue<int> Q;
int flag;
bool over = false;
int bufferSize = 500;

// 递归打印
void showall() { std::cout<<std::endl; }

template <typename R1 ,typename...  Args>
void showall(R1 var, Args...args) {
    std::cout << var;
    showall(args...);
}

void consume() {
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
        if (Q.size() == (bufferSize - 1)) {
            full.notify_one();
        }
    }
}

void produce() {
    while (true) {
        std::unique_lock<std::mutex> lg(mut);
        if (Q.size() == bufferSize) {
            showall("buffer full. waiting...");
            full.wait(lg);
        }
        showall("produce ", Q.size());
        Q.push(Q.size());
        std::this_thread::sleep_for( std::chrono::milliseconds(80));
        if (Q.size() == 1) {
            empty.notify_one();
        }
    }
}

int main() {
    std::thread Producer(produce);
    std::thread Consumer(consume);

    Producer.join();
    Consumer.join();


    return 0;
}
