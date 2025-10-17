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
  // using wref_action = std::weak_ptr<action_type>; // ok, but prefer the following, it is nicer.
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
  using ref_wire = std::pair<bool, std::list<wref_action>>;

  inline ref_wire wire() {
    // ref_wire wire {false, std::list<wref_action> {}};
    return {false, std::list<wref_action>{}};
  }

  inline bool value(const ref_wire wire){
    return wire.first;
  }

  inline void link(ref_wire& wire, const ref_action action){
    wire.second.emplace_back(action);

  }

  inline void set(ref_wire& wire, const bool status){
    if(wire.first != status){
      wire.first = status;
      for(auto it = wire.second.begin(); it != wire.second.end(); ++it)
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
    return std::make_shared<agenda_type>();
  }

  inline void notify(ref_agenda agenda, ref_wire wire, bool status, double duration){

  }

  inline double now(ref_agenda agenda){

  }

  inline bool next(ref_agenda agenda){

  }

  inline void flush(ref_agenda agenda, int steps){

  }
}
