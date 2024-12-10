#pragma once

#include <iostream>
#include <vector>

#include "VRMLUnits.hpp"
#include "Printable.hpp"

namespace vrml_proc {
    namespace parser {
        struct Int32Array : public Printable {

            Int32Array()
                : Printable(std::cout) { }

            std::vector<int32_t> integers;

            void Add(int32_t integer) {
                integers.push_back(integer);
            }

            void Print(Printable::IndentationLevel indentation_level) const override;
        };
    }
}
