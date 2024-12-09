#pragma once

#include <iostream>

#include "VRMLUnits.hpp"
#include "Printable.hpp"

struct Vec3f : public Printable {

    Vec3f()
        : Printable(std::cout) { }

    float32_t x;
    float32_t y;
    float32_t z;

    void Print(Printable::IndentationLevel indentation_level) const override;
};