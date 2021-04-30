#include "gtest/gtest.h"
#include <FixedThreadPool.h>

class RunnableTask : public Runnable {
public:
    std::atomic_int timesRun;

    explicit RunnableTask() : timesRun(0) {}
    ~RunnableTask() override = default;

    void run() override {
        timesRun++;
    }
};

TEST(FixedThreadPoolTestSuite, TestPoolCreatesCorrectNumberOfThreads) {
    FixedThreadPool pool(5);
    ASSERT_EQ(pool.getPoolSize(), 5);
}

TEST(FixedThreadPoolTestSuite, TestPoolShutdown) {
    FixedThreadPool pool(1);
    ASSERT_FALSE(pool.isShutdown());
    pool.shutdown();
    ASSERT_TRUE(pool.isShutdown());
}

TEST(FixedThreadPoolTestSuite, TestPoolExecute) {
    FixedThreadPool pool(1);
    RunnableTask task;
    pool.execute(&task);
    ASSERT_EQ(pool.getTaskCount(), 1);
    pool.shutdown();
    ASSERT_EQ(task.timesRun, 1);
}

TEST(FixedThreadPoolTestSuite, TestPoolSubmit) {
    FixedThreadPool pool(1);
    RunnableTask task;
    auto fut = pool.submit(&task, true);
    ASSERT_EQ(pool.getTaskCount(), 1);
    ASSERT_TRUE(fut.get());
    ASSERT_EQ(task.timesRun, 1);
}

TEST(FixedThreadPoolTestSuite, TestPoolMultipleExecute) {
    FixedThreadPool pool(3);
    RunnableTask task;
    for (int i=0; i<5; ++i)
        pool.execute(&task);
    ASSERT_EQ(pool.getTaskCount(), 5);
    pool.shutdown();
    ASSERT_EQ(task.timesRun, 5);
}

TEST(FixedThreadPoolTestSuite, TestPoolMultipleTasks) {
    FixedThreadPool pool(2);
    RunnableTask task1, task2;
    auto fut1 = pool.submit(&task1, true);
    auto fut2 = pool.submit(&task2, 42);
    ASSERT_EQ(pool.getTaskCount(), 2);
    EXPECT_TRUE(fut1.get());
    EXPECT_EQ(fut2.get(), 42);
    EXPECT_EQ(task1.timesRun, 1);
    EXPECT_EQ(task2.timesRun, 1);
}

TEST(FixedThreadPoolTestSuite, TestAddingTaskAfterShutdown) {
    FixedThreadPool pool(1);
    RunnableTask task;
    pool.shutdown();
    EXPECT_THROW(
            {
                try {
                    pool.execute(&task);
                }
                catch (const std::runtime_error &e) {
                    EXPECT_STREQ("Cannot enqueue on terminated pool", e.what());
                    throw;
                }
                }, std::runtime_error);
    ASSERT_EQ(task.timesRun, 0);
}
