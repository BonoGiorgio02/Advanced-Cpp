#pragma once

#include <gate.hpp>

namespace gate{
    inline auto bit_adder(ref_agenda agenda, ref_wire w_in1, ref_wire w_in2, ref_wire w_out, ref_wire c_in, ref_wire c_out) {
        ref_wire w1 = wire();
        ref_wire w2 = wire();
        ref_wire w3 = wire();

        auto xor1 = XOR(agenda, w_in1, w_in2, w1);
        auto xor2 = XOR(agenda, c_in, w1, w_out);
        auto and1 = AND(agenda, w1, c_in, w2);
        auto and2 = AND(agenda, w_in1, w_in2, w3);
        auto or1 = OR(agenda, w2, w3, c_out); 

        return action([xor1, xor2, and1, and2, or1](){});
    }

    inline auto adder(ref_agenda agenda, std::vector<ref_wire> v_in1, std::vector<ref_wire> v_in2, std::vector<ref_wire> v_out) {
        std::vector<ref_action> gates;
        
        ref_wire c_in = wire();
        ref_wire c_out = wire();

        auto ait = v_in1.begin();
        auto bit = v_in2.begin();
        auto cit = v_out.begin();

        for(auto gate_it = std::back_inserter(gates);
	ait != v_in1.end(); 
	++ait, ++bit, ++cit, c_in = c_out)
            *(gate_it++) = bit_adder(agenda, 
			       *ait, *bit, *cit,
			       c_in, c_out = wire());
        return action([gates](){});
    }
}