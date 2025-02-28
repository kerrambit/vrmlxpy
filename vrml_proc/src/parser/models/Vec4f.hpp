#pragma once

#include <iostream>

#include "VrmlUnits.hpp"
#include "Printable.hpp"
#include "VrmlProcessingExport.hpp"

namespace vrml_proc {
  namespace parser {
    struct VRMLPROCESSING_API Vec4f : public Printable {
      Vec4f() : Printable(std::cout) {}

      Vec4f(float32_t x, float32_t y, float32_t z, float32_t w) : Printable(std::cout), x(x), y(y), z(z), w(w) {}

      float32_t x = 0.0f;
      float32_t y = 0.0f;
      float32_t z = 0.0f;
      float32_t w = 0.0f;

      void Print(Printable::IndentationLevel indentationLevel) const override;
    };
  }  // namespace parser
}  // namespace vrml_proc
