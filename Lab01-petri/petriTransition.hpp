#pragma once

#include <utility> // for std::pair
#include <vector>
#include <string>

#include <petriPool.hpp> // For talking about pools.

namespace petri {
  using slot = std::pair<pool_ref, unsigned int>;
 
  struct Transition {
    std::string       name;
    std::vector<slot> inputs;
    std::vector<slot> outputs;

    Transition(std::string name, std::vector<slot> inputs, std::vector<slot> outputs);

    bool is_activable();
    void activate();

    void operator()();

    operator bool();
  };

  // Print the transition
  std::ostream& operator<<(std::ostream& os, Transition t);
}
