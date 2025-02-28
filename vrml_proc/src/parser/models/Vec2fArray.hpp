#pragma once

#include <iostream>
#include <vector>

#include "Printable.hpp"
#include "Vec2f.hpp"
#include "VrmlProcessingExport.hpp"

namespace vrml_proc {
  namespace parser {
    struct VRMLPROCESSING_API Vec2fArray : Printable {
      Vec2fArray() : Printable(std::cout) {}

      std::vector<Vec2f> vectors;

      void Add(const Vec2f& vector) { vectors.push_back(vector); }

      void Print(Printable::IndentationLevel indentationLevel) const override;
    };
  }  // namespace parser
}  // namespace vrml_proc
