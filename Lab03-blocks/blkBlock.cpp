#include <iomanip>
#include <string>

#include "blkBlock.hpp"

int blk::Block::min_line() const { return -this->ref_height; }

int blk::Block::max_line() const { return this->height - this->ref_height; }

void blk::Block::print_line(std::ostream& os, int line) const {
    if(line < this->min_line() || line >= this->max_line()) os << std::string(this->width, ' ');
    else this->print_inbounds_line(os, line);
}

std::ostream& blk::operator<<(std::ostream& os, blk::expr block) {
    if(block != nullptr) {
        int max = block->max_line();
        for(int line = block->min_line(); line < max; ++line){
                os << std::right << std::setw(2) << line << ": ";
                block->print_line(os, line);
                os << std::endl;
            }
    }
    else os << "null";
    return os;
}

blk::Block::Block(int width, int height, int ref_width, int ref_height) : width(width), height(height), ref_width(ref_width), ref_height(ref_height) {}

blk::Block::~Block() {}

int blk::Block::get_width() const { return this->width; }

int blk::Block::get_height() const { return this->height; }

int blk::Block::get_ref_width() const { return this->ref_width; }

int blk::Block::get_ref_height() const { return this->ref_height; }