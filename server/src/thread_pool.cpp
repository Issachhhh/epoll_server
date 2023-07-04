#include "../include/thread_pool.h"

ThreadPool::ThreadPool(int size): stop(false) {
    for(int i = 0; i < size; ++i) {
        this->threads.push_back(std::thread(work, this));
    }
}

ThreadPool::~ThreadPool() {
    this->stoplock.lock();
    stop = true;
    this->stoplock.unlock();
    this->cv.notify_all();
    for(int i = 0; i < this->threads.size(); ++i) {
        this->threads[i].join();
    }
}

void ThreadPool::addTask(MyEvent* task) {
    this->lck.lock();
    this->tasks.push(task);
    this->lck.unlock();
    this->cv.notify_one();
}

void ThreadPool::work(void* arg) {
    ThreadPool* tp = static_cast<ThreadPool*>(arg);
    while(true) {
        std::unique_lock<std::mutex> ul(tp->lck);
        tp->cv.wait(ul, [&]{
            return tp->stop || !tp->tasks.empty();
        });
        if(tp->stop && tp->tasks.empty())
            return;
        auto task = tp->tasks.front();
        tp->tasks.pop();
        ul.unlock();
        task->call_back(task->fd, task->events, task->args, task->sock);
    }
}