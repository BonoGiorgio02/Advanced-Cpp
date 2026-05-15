#pragma once

#include <iostream>
#include <iomanip>
#include <type_traits>
#include <chrono>
#include <map>
#include <vector>

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


using unary_task_t = std::function<std::string (const std::string&)>;
using binary_task_t = std::function<std::string (const std::string&, const std::string&)>;
using duration_t = std::chrono::milliseconds;
using duration_map_t = std::map<std::string, duration_t>;

void display_one_line(const std::vector<std::string> task_times, std::string expected, std::string measured, std::string status, char fill=' ') {
  std::cout << '|';
  for(const std::string& task_time : task_times)
    std::cout << std::setfill(fill) << std::setw(8) << task_time << '|';
  std::cout << std::setfill(fill) << std::setw(8) << expected << '|';
  std::cout << std::setfill(fill) << std::setw(8) << measured << '|';
  std::cout << std::setfill(fill) << std::setw(6) << status << '|' << std:: endl;
}

template<typename Graph> 
void test_computation(std::string function(std::string, const Graph&), const Graph& graph) {    
  static bool first_call = true;
  std::vector<std::string> keys, values;
  std::string total;
  for(auto& [key, value] : graph.durations) {
    if(key == "total")
      total = duration_string(value);
    else {
      keys.push_back(key);
      values.push_back(duration_string(value));
    }
  }
  if(first_call) {
    std::vector<std::string> emptyFields {keys.size(), ""};
    display_one_line(emptyFields, "", "", "", '-');
    display_one_line(emptyFields, "EXPECTED", "MEASURED", "");
    display_one_line(keys, "TIME  ", "TIME  ", "STATUS");
    display_one_line(emptyFields, "", "", "", '-');
    first_call = false;
  }
  Chrono chrono;
  std::string output = chrono(function, graph.input, graph);
  Chrono::duration_t duration = chrono, expected = graph.durations.at("total"), diff = duration - expected;
  if(output != graph.output)
    throw std::runtime_error(std::string("Output \"") + output + "\" should be \"" + graph.output + "\"");
  std::string status { "    \x1B[32mOK\033[0m"} ;
  if(diff > 10ms) status = "   \x1B[31mNOK\033[0m";
  display_one_line(values, total, chrono, status);
}

template<typename Duration>
auto make_unary_computation(const std::string& name, const Duration& delay) {
  // Return a unary lambda function simulating a computation task of given name & delay
  return [name, delay] (const std::string& arg) {
	   // Simulate some processing by sleeping for the given delay
	   std::this_thread::sleep_for(delay);

	   // Builds the expected std::string result
	   return name + '(' + arg + ')';
	 };
}

template<typename Duration>
auto make_binary_computation(const std::string& name, const Duration& delay) {
  // Return a binary lambda function simulating a computation task of given name & delay
  return [name, delay] (const std::string& arg1, const std::string& arg2) {
	   // Simulate some processing by sleeping for the given delay
	   std::this_thread::sleep_for(delay);
	   
	   // Builds the expected std::string result
	   return name + '(' + arg1 + ',' + arg2 + ')';
	 };
}

// Exception utilisée pour signaler qu'un thread abandonne son computation
struct abort_exception {};

template<typename Duration>
auto make_unary_computation_for_polling(const std::string& name, const Duration& delay) {
  auto count = 0ms;
  return [name, delay, count] (const std::string& arg) mutable {
	   std::this_thread::sleep_for(10ms);
	   count += 10ms;
	   if(count >= delay)
	     return name + '(' + arg + ')';
	   else
	     return ""s;
	 };
}

struct Graph1 {
  unary_task_t A, B;
  binary_task_t C;
  std::string input{"X"}, output{"C(A(X),B(X))"};
  duration_map_t durations;
  
  Graph1(const duration_map_t& durations) : durations{durations} {
    A = make_unary_computation("A", durations.at("A"));
    B = make_unary_computation("B", durations.at("B"));
    C = make_binary_computation("C", durations.at("C"));
  }
};

struct Graph2 {
  unary_task_t A, B, C1, D1;
  binary_task_t C2, D2, E;
  std::string input{"X"}, output{"E(C2(C1(A(X)),B(X)),D2(A(X),D1(B(X))))"};
  duration_map_t durations;
  
  Graph2(const duration_map_t& durations) : durations{durations} {
    A = make_unary_computation("A", durations.at("A"));
    B = make_unary_computation("B", durations.at("B"));
    C1 = make_unary_computation("C1", durations.at("C1"));
    C2 = make_binary_computation("C2", durations.at("C2"));
    D1 = make_unary_computation("D1", durations.at("D1"));
    D2 = make_binary_computation("D2", durations.at("D2"));
    E = make_binary_computation("E", durations.at("E"));
  }
};

struct Graph3 {
  unary_task_t A, B, C;
  std::string input{"X"}, output;
  duration_map_t durations;
  
  Graph3(const duration_map_t& durations) : durations{durations} {
    duration_t dA = durations.at("A"), dB = durations.at("B");
    if(dA < dB)
      output = "C(A(X))";
    else
      output = "C(B(X))";
      
    A = make_unary_computation("A", dA);
    B = make_unary_computation("B", dB);
    C = make_unary_computation("C", durations.at("C"));
  }
};

struct Graph4 {
  unary_task_t A, B, C;
  std::string input{"X"}, output;
  duration_map_t durations;
  
  Graph4(const duration_map_t& durations) : durations{durations} {
    duration_t dA = durations.at("A"), dB = durations.at("B");
    if(dA < dB)
      output = "C(A(X))";
    else
      output = "C(B(X))";
      
    A = make_unary_computation_for_polling("A", dA);
    B = make_unary_computation_for_polling("B", dB);
    C = make_unary_computation("C", durations.at("C"));
  }
};
