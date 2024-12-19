#pragma once

#include <iostream>

#include "VRMLUnits.hpp"
#include "Printable.hpp"
#include "VrmlProcessingExport.hpp"

namespace vrml_proc {
    namespace parser {
        struct VRMLPROCESSING_API Vec4f : public Printable {

            Vec4f()
                : Printable(std::cout) { }

            float32_t x = 0.0f;
            float32_t y = 0.0f;
            float32_t z = 0.0f;
            float32_t w = 0.0f;

            void Print(Printable::IndentationLevel indentation_level) const override;
        };
    }
}
