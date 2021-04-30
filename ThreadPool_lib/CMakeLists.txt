project(ThreadPool)

set(HEADER_FILES
        Runnable.h
        FixedThreadPool.h
        )

set(SOURCE_FILES
        FixedThreadPool.cpp
        )

add_library(ThreadPool_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})