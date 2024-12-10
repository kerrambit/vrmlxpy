#pragma once

#include <iostream>

#include "VRMLUnits.hpp"
#include "Printable.hpp"

namespace vrml_proc {
    namespace parser {
        struct Vec3f : public Printable {

            Vec3f()
                : Printable(std::cout) { }

            float32_t x = 0.0f;
            float32_t y = 0.0f;
            float32_t z = 0.0f;

            void Print(Printable::IndentationLevel indentation_level) const override;
        };
    }
}
