#pragma once

#include <string>
#include <blkBlock.hpp>
#include <blkOver.hpp>
#include <blkBeside.hpp>
#include <blkText.hpp>

namespace blk {
  expr operator/(expr a, expr b);
  expr operator+(expr a, expr b);
  expr operator==(expr a, expr b);

  expr operator/(double a, expr b);
  expr operator+(double a, expr b);
  expr operator==(double a, expr b);

  expr operator/(expr a, double b);
  expr operator+(expr a, double b);
  expr operator==(expr a, double b);
}
