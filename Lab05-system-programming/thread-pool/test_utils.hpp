#pragma once

#include <iostream>
#include <iomanip>
#include <type_traits>
#include <chrono>
#include <map>

using namespace std::literals;

template<typename Duration>
std::string duration_string(const Duration& duration) {
  double ns = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
  std::stringstream os;
  os << std::setprecision(3);
  if(ns < 1.E3) os << ns << " ns";
  else if(ns < 1.E6) os << ns / 1.E3 << " us";
  else if(ns < 1.E9) os << ns / 1.E6 << " ms";
  else os << ns / 1.E9 << " s";
  return os.str();
}

struct Chrono {
  using my_clock = std::chrono::high_resolution_clock;
  using duration_t = my_clock::duration;
  
private:
  duration_t duration;

public:

  Chrono() : duration{} {}
  operator duration_t () const { return duration; }
  operator std::string () const { return duration_string(duration); }
  
  template<typename Callable, typename... Args> 
  typename std::enable_if<
    std::is_void<
      typename std::result_of<Callable(Args...)>::type
      >::value,
    void
    >::type	operator() (const Callable& f, Args... args) {
    auto startTime = my_clock::now();
    f(args...);
    auto endTime = my_clock::now();
    duration += (endTime - startTime);
  }

  template<typename Callable, typename... Args> 
  typename std::enable_if<
    ! std::is_void<
      typename std::result_of<Callable(Args...)>::type
      >::value, 
    typename std::result_of<Callable(Args...)>::type
    >::type	operator() (const Callable& f, Args... args) {
    auto startTime = my_clock::now();
    auto res = f(args...);
    auto endTime = my_clock::now();
    duration = (endTime - startTime);
    return res;
  }

  // To output duration in a stream in the most adapted unit
  friend std::ostream& operator<<(std::ostream& os, const Chrono& chrono) {
    os << duration_string(chrono.duration);
    return os;
  }
};

template<typename Duration>
auto make_computation(const std::string& name, const Duration& delay) {
  return [name, delay] () {
	   std::this_thread::sleep_for(delay);
	   std::cout << name << "() completed" << std::endl;
	 };
}
