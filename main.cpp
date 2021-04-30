#include <iostream>
#include "Runnable.h"
#include "FixedThreadPool.h"

class Test : public Runnable {
public:
    int id;
    explicit Test(int id) : id(id) {}

    ~Test() override = default;

    void run() override {
        std::cout << "task: " << id << std::endl;
    }
};

int main() {
    FixedThreadPool pool(5);
    auto t1 = Test(1);
    auto t2 = Test(2);
    pool.execute(&t1);
    auto f = pool.submit(&t2, 42);
    pool.shutdown();
    auto res = f.get();
    std::cout << "res: " << res << std::endl;

    return 0;
}
