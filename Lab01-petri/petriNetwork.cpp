#include <petriNetwork.hpp>

petri::Network::Network(unsigned int seed)
  : gen(seed) {}

bool petri::Network::operator()(){
  std::vector<Transition*> activableTransitions;
  bool active = false;
  for(auto& t : this->transitions)
    if(t){
      activableTransitions.push_back(&t);
      active = true;
    }
  if(active){
    std::uniform_int_distribution<int> random_index(0, activableTransitions.size()-1);
    auto t = *(activableTransitions.at(random_index(gen)));
    t();
    if(this->transition_verbosity)
      std::cout << t.name << std::endl;
  }
  return active;
}