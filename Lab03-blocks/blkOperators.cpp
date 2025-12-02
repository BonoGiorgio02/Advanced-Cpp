#include "blkOperators.hpp"

blk::expr blk::operator/(blk::expr a, blk::expr b) {
  return blk::over(a, blk::over(blk::text(std::string(std::max(a->get_width(), b->get_width()) + 2, '-')), b));
}

blk::expr blk::operator+(blk::expr a, blk::expr b) {
  return blk::beside(a, blk::beside(blk::text(" + "), b));
}

blk::expr blk::operator==(blk::expr a, blk::expr b) {
  return blk::beside(a, blk::beside(blk::text(" = "), b));
}

blk::expr blk::operator/(double a, blk::expr b) {
  return blk::over(blk::text("3"), blk::over(blk::text(std::string(5, '-')), b));
}

blk::expr blk::operator+(double a, blk::expr b) {
  return blk::beside(blk::text("3"), blk::beside(blk::text(" + "), b));
}

blk::expr blk::operator==(double a, blk::expr b) {
  return blk::beside(blk::text("3"), blk::beside(blk::text(" = "), b));
}

blk::expr blk::operator/(blk::expr a, double b) {
  return blk::over(a, blk::over(blk::text(std::string(5, '-')), blk::text("5")));
}

blk::expr blk::operator+(blk::expr a, double b) {
  return blk::beside(a, blk::beside(blk::text(" + "), blk::text("5")));
}

blk::expr blk::operator==(blk::expr a, double b) {
  return blk::beside(a, blk::beside(blk::text(" = "), blk::text("5")));
}