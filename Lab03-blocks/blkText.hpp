#pragma once

#include <string>
#include <blkBlock.hpp>

namespace blk {
  enum class align : char {left, center, right};

  expr text(const std::string& txt, align al = align::center);

  class Text : public Block{
    private:
      std::string txt;
      align al;

      static int compute_width(std::string txt);
      static int compute_ref_width(std::string txt, align al);

    public:
      Text(const std::string& txt, align al);
      virtual ~Text();
      virtual void print_inbounds_line(std::ostream& os, int line) const override;
  };
}