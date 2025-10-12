#include <petriTransition.hpp>

petri::Transition::Transition(std::string name, std::vector<petri::slot> inputs, std::vector<petri::slot> outputs) 
                                : name(name), inputs(inputs), outputs(outputs) {}

std::ostream& petri::operator<<(std::ostream& os, petri::Transition t){
    os << '{';
    for(auto input : t.inputs)
        os << input.first << '/' << input.second << ' ';
    os << "--> " << t.name << " -->";
    for(auto output : t.outputs)
        os << ' ' << output.first << '/' << output.second;
    os << '}';
    return os;
}

bool petri::Transition::is_activable(){
    for(auto input : this->inputs)
        if(input.first < input.second)
            return false;
    return true;
}

void petri::Transition::activate(){
    for(auto input : this->inputs)
        input.first->nb_tokens -= input.second;
    for(auto output : this->outputs)
        output.first->nb_tokens += output.second;
}

void petri::Transition::operator()(){
    this->activate();
}

petri::Transition::operator bool(){
    return this->is_activable();
}