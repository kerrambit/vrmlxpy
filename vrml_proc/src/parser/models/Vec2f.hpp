#pragma once

#include <iostream>
#include <utility>

#include "Printable.hpp"
#include "VrmlProcessingExport.hpp"
#include "VrmlUnits.hpp"

namespace vrml_proc {
    namespace parser {
        struct VRMLPROCESSING_API Vec2f : public Printable {

            Vec2f() : Printable(std::cout) {}

            Vec2f(float32_t u, float32_t v)
                : Printable(std::cout), u(u), v(v) {}

            Vec2f(Vec2f&& other) noexcept
                : Printable(std::cout), u(std::exchange(other.u, 0.0f)),
                v(std::exchange(other.v, 0.0f)) {}

            Vec2f& operator=(Vec2f&& other) noexcept {
                if (this != &other) {
                    u = std::exchange(other.u, 0.0f);
                    v = std::exchange(other.v, 0.0f);
                }
                return *this;
            }

            Vec2f(const Vec2f&) = default;

            Vec2f& operator=(const Vec2f&) = default;

            float32_t u = 0.0f;
            float32_t v = 0.0f;

            void Print(Printable::IndentationLevel indentationLevel) const override;
        };
    }
}
