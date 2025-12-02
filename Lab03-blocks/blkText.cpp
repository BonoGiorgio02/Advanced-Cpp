#include "blkText.hpp"

blk::expr blk::text(const std::string& txt, align al) {
  return std::make_shared<Text>(txt, al);
}

int blk::Text::compute_width(std::string txt) {
  return txt.length();
}

int blk::Text::compute_ref_width(std::string txt, align al) {
  if(al == align::center)
    return txt.length()/2;
  else if(al == align::left)
    return 0;
  else
    return txt.length() - 1;
}

blk::Text::Text(const std::string& txt, align al) 
                : Block(blk::Text::compute_width(txt), 1, 
                        blk::Text::compute_ref_width(txt, al), 0),
                  txt(txt), al(al) {}

blk::Text::~Text() {}

void blk::Text::print_inbounds_line(std::ostream& os, int line) const {
  os << txt;
}