#pragma once

#include <vector>
#include <iostream>

#include "Vec3f.hpp"
#include "Printable.hpp"
#include "VrmlProcessingExport.hpp"

namespace vrml_proc {
    namespace parser {
        struct VRMLPROCESSING_API Vec3fArray : Printable {

            Vec3fArray()
                : Printable(std::cout) { }

            std::vector<Vec3f> vectors;

            void Add(const Vec3f& vector) {
                vectors.push_back(vector);
            }

            void Print(Printable::IndentationLevel indentationLevel) const override;
        };
    }
}

