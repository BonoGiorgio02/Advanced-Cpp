#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <future>
#include <chrono>
#include <thread>
#include <numeric>
#include <atomic>
#include <algorithm>

#include "test_utils.hpp"

using namespace std::literals;

/********
 * Demo *
 ********/

std::string demo(std::string input, const Graph1& graph) {  
  std::string a = graph.A(input);
  std::string b = graph.B(input);
  return graph.C(a, b);
}

void test0() {
  test_computation(demo, Graph1{ {{"A", 100ms}, {"B", 200ms}, {"C", 100ms}, {"total", 400ms}} });
}

/**************
 * Exercise 1 *
 **************/

// Function to complete, that implements the expected computation graph
std::string computation1(std::string input, const Graph1& graph) {
  return ""s;
}

// Test function. Do not modify.
void test1() {
  test_computation(computation1, Graph1{ {{"A", 100ms}, {"B", 200ms}, {"C", 100ms}, {"total", 300ms}} });
  test_computation(computation1, Graph1{ {{"A", 200ms}, {"B", 100ms}, {"C", 100ms}, {"total", 300ms}} });
}

/**************
 * Exercise 2 *
 **************/

// Function to complete, that implements the expected computation graph
std::string computation2(std::string input, const Graph2& graph) {
  return ""s;
}

// Test function. Do not modify.
void test2() {
  test_computation(computation2, Graph2{ {{"A", 100ms}, {"B", 200ms}, {"C1", 200ms}, {"C2", 100ms}, {"D1", 100ms}, {"D2", 100ms}, {"E", 100ms}, {"total", 500ms}} });
  test_computation(computation2, Graph2{ {{"A", 100ms}, {"B", 200ms}, {"C1", 200ms}, {"C2", 200ms}, {"D1", 100ms}, {"D2", 100ms}, {"E", 100ms}, {"total", 600ms}} });
  test_computation(computation2, Graph2{ {{"A", 100ms}, {"B", 200ms}, {"C1", 200ms}, {"C2", 100ms}, {"D1", 100ms}, {"D2", 200ms}, {"E", 100ms}, {"total", 600ms}} });
  test_computation(computation2, Graph2{ {{"A", 100ms}, {"B", 400ms}, {"C1", 200ms}, {"C2", 100ms}, {"D1",  50ms}, {"D2",  50ms}, {"E", 100ms}, {"total", 600ms}} });
  test_computation(computation2, Graph2{ {{"A", 400ms}, {"B", 200ms}, {"C1",  50ms}, {"C2",  50ms}, {"D1", 100ms}, {"D2", 100ms}, {"E", 100ms}, {"total", 600ms}} });
}

/**************
 * Exercise 3 *
 **************/

// Function to complete, that implements the expected computation graph
std::string computation3(std::string input, const Graph3& graph) {
  return ""s;
}


// Test function. Do not modify.
void test3() {
  test_computation(computation3, Graph3{ {{"A", 100ms}, {"B", 200ms}, {"C", 100ms}, {"total", 200ms}} });
  test_computation(computation3, Graph3{ {{"A", 200ms}, {"B", 100ms}, {"C", 100ms}, {"total", 200ms}} });

  // test_computation(computation3, Graph3{ {{"A", 100ms}, {"B", 300ms}, {"C", 100ms}, {"total", 200ms}} });
  // test_computation(computation3, Graph3{ {{"A", 300ms}, {"B", 100ms}, {"C", 100ms}, {"total", 200ms}} });
}

/**************
 * Exercise 4 *
 **************/

std::string computation4(std::string input, const Graph4& graph) {
  return ""s;
}

// Test function. Do not modify.
void test4() {
  test_computation(computation4, Graph4{ {{"A", 100ms}, {"B", 300ms}, {"C", 100ms}, {"total", 200ms}} });
  test_computation(computation4, Graph4{ {{"A", 300ms}, {"B", 100ms}, {"C", 100ms}, {"total", 200ms}} });
}

/**************
 * Exercise 5 *
 **************/

template<typename Iter, typename T> Iter find_multicores(Iter begin, Iter end, const T& elt) {
  return end;
}

void test5() {

  // Builds a vector containing 1, 2, ..., n
  const int n = 100000000;
  std::vector<int> vect(n);
  std::iota(vect.begin(), vect.end(), 1);

  // Parallel search on maximmal number of cores
  int nCores = std::thread::hardware_concurrency(); // Number of cores
  int value = n; // Value to search is the last (i.e. hardest) one

  // Measure processing time using find_multicores
  Chrono chrono;
  auto it = chrono([&vect, &value]() { return find_multicores(vect.begin(), vect.end(), value); });
  std::cout << "With " << nCores << " cores, value " << value << " found at position " << (it - vect.begin()) << " in " << chrono << std::endl;

  // Compare with processing time using standard find
  it = chrono([&vect, &value]() { return std::find(vect.begin(), vect.end(), value); });
  std::cout << "With one core, value " << value << " found at position " << (it - vect.begin()) << " in " << chrono << std::endl;
}


// Main runs the test function whose number is passed as a command line argument
int main( int argc, char *argv[]) {
  try {
    void(*tests[])() = { test0, test1, test2, test3, test4, test5 };
    constexpr int nTests = sizeof(tests)/sizeof(void(*)());

    if(argc >= 2) {
      int num = std::atoi(argv[1]);
      if(num >= 0 && num <= nTests-1) {
	tests[num]();
	return 0;
      }
    }
    throw std::invalid_argument{"expected syntax is "s + argv[0] + " <test number from 0 to " + std::to_string(nTests - 1) + ">"};
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return -1;
  }
}
