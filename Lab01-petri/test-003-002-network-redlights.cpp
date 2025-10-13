#include <iostream>
#include <iterator>
#include <random>
#include <fstream>

#include <petri.hpp>

int main(int argc, char* argv[]){
    std::random_device rd;
    petri::Network redlights(rd());

    // Here are the pools
    auto red1 = petri::make_pool("red1", 1);
    auto orange1 = petri::make_pool("orange1", 0);
    auto green1 = petri::make_pool("green1", 0);
    auto red2 = petri::make_pool("red2", 1);
    auto orange2 = petri::make_pool("orange2", 0);
    auto green2 = petri::make_pool("green2", 0);
    auto allred = petri::make_pool("allred", 1);

    // Let us add transitions
    auto out_trans_iter = std::back_inserter(redlights.transitions);
    *(out_trans_iter++) = {"turns green1",
			 {{red1,    1}, {allred, 1}},
			 {{green1,  1}}};
    *(out_trans_iter++) = {"turns orange1",
			 {{green1,  1}},
			 {{orange1, 1}}};
    *(out_trans_iter++) = {"turns red1",
			 {{orange1, 1}},
			 {{red1, 1}, {allred, 1}}};

    *(out_trans_iter++) = {"turns green2",
			 {{red2,    1}, {allred, 1}},
			 {{green2,  1}}};
    *(out_trans_iter++) = {"turns orange2",
			 {{green2,  1}},
			 {{orange2, 1}}};
    *(out_trans_iter++) = {"turns red2",
			 {{orange2, 1}},
			 {{red2, 1}, {allred, 1}}};

    // Let us run the sumulation
    redlights.transition_verbosity = true;
    for(unsigned int nb_steps = 0; (nb_steps < 10) && redlights(); ++nb_steps);

    std::ofstream file("redlight.dot");
    file << redlights; // redlights is a petri::Network

    std::cout << std::endl
	    << "File redlight.dot generated" << std::endl
	    << "Make a pdf from it : " << std::endl
	    << "  dot -Tpdf redlight.dot -o redlight.pdf" << std::endl;
    
    return 0;
}