#pragma once

#include <random>

namespace petri {
  struct Network {
    std::mt19937 gen;   // random generator 

    Network(unsigned int random_seed);
    Network() = delete; // We forbid the default construction.
  };
}
