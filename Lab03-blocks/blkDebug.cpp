#include <memory>

#include "blkBlock.hpp"
#include "blkDebug.hpp"

blk::Debug::Debug(char label, int width, int height, int ref_width, int ref_height)
                : blk::Block::Block(width, height, ref_width, ref_height), label(label) {}

blk::Debug::~Debug() {}

void blk::Debug::print_inbounds_line(std::ostream& os, int line) const {
    for(int i = 0; i < this->get_width(); ++i)
        if(line == 0 && i == this->get_ref_width()) os << '#';
        else os << label;
}

blk::expr blk::debug(char label, int width, int height, int ref_width, int ref_height) {
    return std::make_shared<Debug>(label, width, height, ref_width, ref_height);
}