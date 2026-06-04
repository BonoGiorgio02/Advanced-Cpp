#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <vector>
#include <deque>

class ThreadPool {
  
public:

  // Constructor of a pool with a given number of threads
  ThreadPool(size_t nThreads);

  // Destructor to remove ressources
  ~ThreadPool();

  // Method template to post any callable with its arguments
  template<typename Func, typename... Args>
  ThreadPool& operator() (const Func& func, Args... args) {
    
    // To be completed
    
    return *this;
  }

  // Method to join all threads of the pool
  void join();
};
