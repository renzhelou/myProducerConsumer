#ifndef TRYBLOCKINGQUEUE_H
#define TRYBLOCKINGQUEUE_H

class tryBlockingQueue {
public:
    void consume();
    void produce();
    tryBlockingQueue(const tryBlockingQueue&) = delete;
    tryBlockingQueue() {};
private:
    std::mutex mut;
    std::condition_variable empty;
    std::condition_variable full;
    std::queue<int> Q;
    int flag;
    bool over = false;
    int bufferSize = 500;
};

#endif
