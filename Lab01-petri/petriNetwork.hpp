#pragma once

#include <random>

#include <petriTransition.hpp>

namespace petri {
  struct Network {
    std::mt19937 gen;   // random generator 
    std::vector<Transition> transitions;
    bool transition_verbosity = false;

    Network(unsigned int random_seed);
    Network() = delete; // We forbid the default construction.

    bool operator()();
  };
}
