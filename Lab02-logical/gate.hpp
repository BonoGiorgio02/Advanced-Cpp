#pragma once

#include <iomanip>
#include <iostream>
#include <string>
#include <memory>
#include <functional>
#include <list>
#include <utility>
#include <iterator>
#include <tuple>
#include <map>

namespace gate {

  /************************/
  /* test-001-001-display */
  /************************/
  inline void ____() {
    std::cout << '+' << std::string(22, '-') << '+' << std::string(10, '-') << '+' << std::string(5, '-') << '+' << std::endl;
  }

  inline void ____display(double time, std::string name, bool status) {
    std::cout.precision(2);
    std::cout << "| " << std::left << std::setw(20) << name 
              << " | " << std::right << std::fixed << std::setw(8) << time 
              << " | " << std::left << std::setw(3) << (status ? "on" : "off") << " |" << std::endl;
  }

  /************************/
  /* test-001-002-actions */
  /************************/
  using action_type = std::function<void ()>;
  using ref_action = std::shared_ptr<action_type>;
  // using wref_action = std::weak_ptr<ref_action>; // ok, but prefer the following, it is nicer.
  using wref_action = ref_action::weak_type;

  inline ref_action action(action_type action) {
    return std::make_shared<action_type>(action);
  }

  inline void execute(ref_action ref_a) {
    (*ref_a)();
  }

  /**********************/
  /* test-001-003-wires */
  /**********************/
  using wire_type = std::tuple<bool, std::list<wref_action>>;
  using ref_wire = std::shared_ptr<wire_type>;

  inline auto wire() {
    // ref_wire wire {false, std::list<wref_action> {}};
    return std::make_shared<std::tuple<bool, std::list<wref_action>>>(false, std::list<wref_action>());
    // == std::make_shared<wire_type>(false, std::list<wref_action>);
    // == std::make_shared<std::tuple<bool, std::list<wref_action>>>(std::tuple<bool, std::list<wref_action>>(false, std::list<wref_action>()));
    // but less efficient
  }

  inline bool& value(const ref_wire wire){
    return std::get<0>(*wire);
  }

  inline void link(ref_wire wire, const ref_action action){
    std::get<1>(*wire).emplace_back(action);

  }

  inline void set(ref_wire wire, const bool status){
    if(value(wire) != status){
      value(wire) = status;
      auto actions = std::get<1>(*wire);
      for(auto it = actions.begin(); it != actions.end(); ++it)
        if(auto action = it->lock(); action)
          execute(it->lock());
    }
  }

  /***********************/
  /* test-002-001-agenda */
  /***********************/
  using agenda_type = std::tuple<double, std::multimap<double, std::tuple<ref_wire, bool>>>;
  using ref_agenda = std::shared_ptr<agenda_type>;

  inline ref_agenda agenda(){
    return std::make_shared<agenda_type>(0.0, std::multimap<double, std::tuple<ref_wire, bool>>());
  }

  inline void notify(ref_agenda agenda, ref_wire wire, bool status, double duration){
    std::get<1>(*agenda).emplace(duration + std::get<0>(*agenda), std::make_tuple(wire, status));
  }

  inline double& now(ref_agenda agenda){
    return std::get<0>(*agenda);
  }

  inline bool next(ref_agenda agenda){
    auto& requests = std::get<1>(*agenda);
    if(requests.empty())
      return false;
    auto it = requests.begin();
    auto [time, request] = *it;
    requests.erase(it);
    now(agenda) = time;
    auto [w, status] = request;
    set(w, status);
    return true;
  }

  /***********************/
  /* test-002-002-agenda */
  /***********************/
  inline void flush(ref_agenda agenda, int steps){
    int n = 0;
    while(n < steps && next(agenda)){++n;};
  }


  /***********************/
  /* test-003-001-probes */
  /***********************/
  inline auto PROBE(ref_agenda agenda, std::string s, ref_wire wire){
    auto print_wire = action([agenda, s, wire](){____display(now(agenda), s, value(wire));});
    link(wire, print_wire);
    return print_wire;
  }

  /********************/
  /* test-003-002-and */
  /********************/
  inline auto AND(ref_agenda agenda, ref_wire w_in1, ref_wire w_in2, ref_wire w_out){
    auto and_operator = action([agenda, w_in1, w_in2, w_out](){notify(agenda, w_out, value(w_in1) && value(w_in2), .5);});
    link(w_in1, and_operator);
    link(w_in2, and_operator);
    execute(and_operator);
    return and_operator;
  }

  inline void flush(ref_agenda agenda){
    while(next(agenda));
  }

  inline auto OR(ref_agenda agenda, ref_wire w_in1, ref_wire w_in2, ref_wire w_out){
    auto or_operator = action([agenda, w_in1, w_in2, w_out](){notify(agenda, w_out, value(w_in1) || value(w_in2), .5);});
    link(w_in1, or_operator);
    link(w_in2, or_operator);
    execute(or_operator);
    return or_operator;
  }

  inline auto NOT(ref_agenda agenda, ref_wire w_in, ref_wire w_out){
    auto not_operator = action([agenda, w_in, w_out](){notify(agenda, w_out, !value(w_in), .1);});
    link(w_in, not_operator);
    execute(not_operator);
    return not_operator;
  }

  /********************/
  /* test-003-002-and */
  /********************/
  inline auto XOR(ref_agenda agenda, ref_wire w_in1, ref_wire w_in2, ref_wire w_out) {
    ref_wire w1 = wire();
    ref_wire w2 = wire();
    ref_wire w3 = wire();
    ref_wire w4 = wire();

    auto not_a = NOT(agenda, w_in1, w1);
    auto not_b = NOT(agenda, w_in2, w2);
    auto and_a = AND(agenda, w1, w_in2, w3);
    auto and_b = AND(agenda, w2, w_in1, w4);
    auto or_a = OR(agenda, w3, w4, w_out);
    
    return action([not_a, not_b, and_a, and_b, or_a](){});
  }
}

