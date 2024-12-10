#pragma once

#include <vector>
#include <iostream>

#include "Vec3f.hpp"
#include "Printable.hpp"

namespace vrml_proc {
    namespace parser {
        struct Vec3fArray : Printable {

            Vec3fArray()
                : Printable(std::cout) { }

            std::vector<Vec3f> vectors;

            void Add(const Vec3f& vector) {
                vectors.push_back(vector);
            }

            void Print(Printable::IndentationLevel indentation_level) const override;
        };
    }
}

