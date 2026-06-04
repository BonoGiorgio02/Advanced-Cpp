#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <future>
#include <chrono>
#include <thread>
#include <numeric>
#include <atomic>
#include <algorithm>

#include "ThreadPool.hpp"
#include "test_utils.hpp"

using namespace std::literals;

void test1() {
  // Simulate three tasks
  auto A = make_computation("A", 100ms);
  auto B = make_computation("B", 200ms);
  auto C = make_computation("C", 100ms);

  // Create a pool containing two threads
  ThreadPool pool{2};

  // Assign computations A, B and C to the thread pool
  pool(A)(B)(C);
}

void test2() {
  auto A = make_computation("A", 100us);
  auto B = make_computation("B", 100us);
  auto C = make_computation("C", 100us);

  Chrono chrono;

  // La création du réservoir ne doit pas faire partie du code testé.
  // Time required to build the thread pool should not be measured (as the pool is created once for all)
  ThreadPool pool{3};

  // Measure processing time to compte A, B and C using the pool
  std::cout << "Running A, B and C using the thread pool:\n" << std::endl;
  chrono([&A, &B, &C, &pool] () -> void {
	   pool(A)(B)(C);
	   // Wait that all computations are terminated
	   pool.join();
	 });
  std::cout << "\nterminated in " << chrono << "\n" << std::endl;

  // Measure processing time to compte A, B and C without using the pool
  std::cout << "Running A, B and C using standard threads:\n" << std::endl;
  chrono([&A, &B, &C] () -> void {
	   std::thread a(A);
	   std::thread b(B);
	   std::thread c(C);
	   a.join(); b.join(); c.join();
	 });
  std::cout << "\nterminated in " << chrono << std::endl;
}

// Main runs the test function whose number is passed as a command line argument
int main( int argc, char *argv[]) {
  try {
    void(*tests[])() = { test1, test2 };
    constexpr int nTests = sizeof(tests)/sizeof(void(*)());

    if(argc >= 2) {
      int num = std::atoi(argv[1]);
      if(num >= 1 && num <= nTests) {
	tests[num - 1]();
	return 0;
      }
    }
    throw std::invalid_argument{"expected syntax is "s + argv[0] + " <test number from 1 to " + std::to_string(nTests) + ">"};
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return -1;
  }
}
