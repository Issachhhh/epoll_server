#ifndef THREAD_POOL_H
#define THREAD_POOL_H
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>
#include "../include/my_event.h"
#include <functional>
class ThreadPool {
public:
    ThreadPool(int size);

    ~ThreadPool();
    void addTask(MyEvent* task);
private:
    std::mutex lck, stoplock;
    std::condition_variable cv;
    std::vector<std::thread> threads;
    std::queue<MyEvent*> tasks;
    bool stop;
    static void work(void* arg);
};
#endif