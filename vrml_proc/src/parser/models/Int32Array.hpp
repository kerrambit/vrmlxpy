#pragma once

#include <iostream>
#include <vector>
#include <cstdint>

#include "Printable.hpp"
#include "VrmlProcessingExport.hpp"

namespace vrml_proc {
  namespace parser {
    struct VRMLPROCESSING_API Int32Array : public Printable {
      Int32Array() : Printable(std::cout) {}

      std::vector<int32_t> integers;

      void Print(Printable::IndentationLevel indentationLevel) const override;
    };
  }  // namespace parser
}  // namespace vrml_proc
