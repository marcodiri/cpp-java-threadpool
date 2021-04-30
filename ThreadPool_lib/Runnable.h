#ifndef PC_CPP_JAVA_THREADPOOL_RUNNABLE_H
#define PC_CPP_JAVA_THREADPOOL_RUNNABLE_H

class Runnable {
public:
    virtual ~Runnable()=0;

    virtual void run()=0;
};

inline Runnable::~Runnable() = default;

#endif //PC_CPP_JAVA_THREADPOOL_RUNNABLE_H
