cmake_minimum_required (VERSION 2.8.11)
project (Pool)

find_package (Threads)

set(PROJECT_CFLAGS "-Wall -std=c++17")

add_executable (pool_test pool_test.cpp  ThreadPool.cpp ThreadPool.hpp)
set_target_properties(pool_test PROPERTIES COMPILE_FLAGS "${PROJECT_CFLAGS}")
target_link_libraries (pool_test ${CMAKE_THREAD_LIBS_INIT})

