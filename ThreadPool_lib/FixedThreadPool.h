#ifndef PC_CPP_INTEGRAL_IMAGE_THREADPOOL_H
#define PC_CPP_INTEGRAL_IMAGE_THREADPOOL_H

#include "Runnable.h"
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>

class FixedThreadPool {
private:
    std::vector<std::thread> pool;
    std::queue<std::function<void()>> tasks;
    std::mutex queue_mutex;
    std::condition_variable condition;
    size_t taskCount;
    bool terminated_pool;
    bool stopped;

    // adapter to pass a future to execute()
    template<class T>
    class RunnableFuture : public Runnable {
    private:
        std::shared_ptr<std::packaged_task<T()>> fut_ptr;
        // do not allow allocation on the stack
        ~RunnableFuture() override = default;

    public:
        explicit RunnableFuture(std::shared_ptr<std::packaged_task<T()>> fut_ptr) : fut_ptr(fut_ptr) {}

        void run() override {
            (*fut_ptr)();
            delete this;
        }
    };

public:
    /**
     * Creates a thread pool that reuses a fixed number of threads
     * operating off a shared unbounded queue.
     * At any point, at most <tt>n_threads</tt> threads will be active
     * processing tasks.
     * If additional tasks are submitted when all threads are
     * active, they will wait in the queue until a thread is
     * available.
     * The threads in the pool will
     * exist until it is explicitly <tt>shutdown</tt>.
     * @param n_threads the number of threads in the pool
     */
    explicit FixedThreadPool(unsigned int n_threads);

    /**
     * Calls <tt>shutdown</tt> if pool is not already shut down.
     */
    ~FixedThreadPool();

    /**
     * Initiates an orderly shutdown in which previously submitted
     * tasks are executed, but no new tasks will be accepted.
     */
    void shutdown();

    /**
     * Executes the given command at some time in the future.
     * @param command the runnable task
     */
     void execute(Runnable *command);

    /**
     * Submits a Runnable task for execution and returns a Future
     * representing that task. The Future's <tt>get</tt> method will
     * return the given result upon successful completion.
     * @param task the task to submit
     * @param result the result to return
     * @return a Future representing pending completion of the task
     */
    template<class T>
    std::future<T> submit(Runnable *task, T result);

    /**
     * Returns <tt>true</tt> if this pool has been shut down.
     *
     * @return <tt>true</tt> if this pool has been shut down
     */
    bool isShutdown() const;

    /**
     * Returns the current number of threads in the pool.
     * @return the current number of threads in the pool
     */
    size_t getPoolSize() const;

    /**
     * Returns the total number of tasks that have ever been scheduled for execution.
     * @return the total number of tasks that have ever been scheduled for execution
     */
    size_t getTaskCount() const;
};

template<class T>
std::future<T> FixedThreadPool::submit(Runnable *task, T result) {
    // create a future to pass back to the caller
    auto fut_ptr = std::make_shared< std::packaged_task<T()> >(
            [task, result] {
                task->run();
                return result;
            }
    );
    execute(new RunnableFuture<T>(fut_ptr));
    return fut_ptr->get_future();
}

#endif //PC_CPP_INTEGRAL_IMAGE_THREADPOOL_H
