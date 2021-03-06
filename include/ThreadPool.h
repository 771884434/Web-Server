#ifndef THREADPOOL_H_INCLUDED
#define THREADPOOL_H_INCLUDED

#include <queue>
#include "Task.h"
#include "Thread.h"
#include <mutex>
#include <memory>

class ThreadPool{
private:
    std::queue<Task *> task_queue;
    std::vector<Thread *> _pool;
    std::mutex _locker;
public:
    ThreadPool(int n=10);
    ~ThreadPool();
    void add_task(Task *task);
    //void add_task(std::shared_ptr<Task> task);
    void run();
};


#endif // THREADPOOL_H_INCLUDED
