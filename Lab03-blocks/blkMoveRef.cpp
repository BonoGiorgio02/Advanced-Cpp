#include <memory>
#include <algorithm>

#include "blkMoveRef.hpp"

blk::expr blk::move_ref(blk::expr op, int new_ref_width, int new_ref_height) {
    return std::make_shared<MoveRef>(op, new_ref_width, new_ref_height);
}

blk::MoveRef::MoveRef(blk::expr op, int new_ref_width, int new_ref_height) 
                    :Block(op->get_width(), op->get_height(), new_ref_width, new_ref_height), op(op) {}

blk::MoveRef::~MoveRef() {}

void blk::MoveRef::print_inbounds_line(std::ostream& os, int line) const {
    op->print_line(os, line + (get_ref_height() - op->get_ref_height()));
}