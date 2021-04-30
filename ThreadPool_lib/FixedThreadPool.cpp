#include "FixedThreadPool.h"

FixedThreadPool::FixedThreadPool(unsigned int n_threads)
: taskCount(0), terminated_pool(false), stopped(false) {
    for (int i=0; i<n_threads; i++)
        // generate threads that consume jobs in the tasks queue
        pool.emplace_back(std::thread(
                [this] {
                    for (;;) { // infinite loop waiting for a task
                        std::function<void()> task;
                        {   // declare the lock in a compound so that it gets released as soon as
                            // the task is assigned, so tasks can be executed simultaneously
                            std::unique_lock<std::mutex> lock(this->queue_mutex);
                            this->condition.wait(lock,
                                                 [this] { return this->terminated_pool || !this->tasks.empty(); });
                            if (this->terminated_pool && this->tasks.empty())
                                return;
                            task = std::move(this->tasks.front());
                            this->tasks.pop();
                        }
                        task();
                    }
                }
        ));
}

FixedThreadPool::~FixedThreadPool() {
    if (!stopped)
        shutdown();
}

void FixedThreadPool::shutdown() {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        terminated_pool = true; // unlock threads waiting on empty queue
    }
    condition.notify_all();
    for(std::thread &worker: pool)
        worker.join();
    stopped = true;
}

void FixedThreadPool::execute(Runnable *command) {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);

        // don't allow enqueueing after stopping the pool
        if(terminated_pool)
            throw std::runtime_error("Cannot enqueue on terminated pool");

        // enqueue the task
        tasks.emplace([command]{command->run();});
    }
    condition.notify_one();
    ++taskCount;
}

bool FixedThreadPool::isShutdown() const {
    return stopped;
}

size_t FixedThreadPool::getPoolSize() const {
    return pool.size();
}

size_t FixedThreadPool::getTaskCount() const {
    return taskCount;
}
