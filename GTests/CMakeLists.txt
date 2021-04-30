project(GTests)
set(EXECUTABLE_NAME GTests_run)

# https://github.com/google/googletest/tree/master/googletest#incorporating-into-an-existing-cmake-project
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(SOURCE_FILES FixedThreadPoolTest.cpp)
add_executable(${EXECUTABLE_NAME} ${SOURCE_FILES})
target_link_libraries(${EXECUTABLE_NAME} ThreadPool_lib)
target_link_libraries(${EXECUTABLE_NAME} gtest gtest_main)