#pragma once

#include <iostream>

#include "Printable.hpp"
#include "VrmlProcessingExport.hpp"
#include "VrmlUnits.hpp"

namespace vrml_proc {
    namespace parser {
        struct VRMLPROCESSING_API Vec3f : public Printable {

            Vec3f() : Printable(std::cout) {}

            Vec3f(float32_t x, float32_t y, float32_t z)
                : Printable(std::cout), x(x), y(y), z(z) {}

            Vec3f(Vec3f&& other) noexcept
                : Printable(std::cout), x(std::exchange(other.x, 0.0f)),
                y(std::exchange(other.y, 0.0f)), z(std::exchange(other.z, 0.0f)) {}

            Vec3f& operator=(Vec3f&& other) noexcept {
                if (this != &other) {
                    x = std::exchange(other.x, 0.0f);
                    y = std::exchange(other.y, 0.0f);
                    z = std::exchange(other.z, 0.0f);
                }
                return *this;
            }

            Vec3f(const Vec3f&) = default;

            Vec3f& operator=(const Vec3f&) = default;

            float32_t x = 0.0f;
            float32_t y = 0.0f;
            float32_t z = 0.0f;

            void Print(Printable::IndentationLevel indentationLevel) const override;
        };
    }
}
