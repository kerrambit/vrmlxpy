#pragma once

#include <string>
#include <iostream>

#include <boost/variant.hpp>
#include <boost/variant/recursive_wrapper.hpp>

#include "VRMLUnits.hpp"
#include "Vec3f.hpp"
#include "Vec3fArray.hpp"
#include "Int32Array.hpp"
#include "USENode.hpp"

#include "Printable.hpp"

namespace vrml_proc {
    namespace parser {

        struct VRMLNode;
        using VRMLFieldValue = boost::variant<std::string, bool, Vec3fArray, Int32Array, float32_t, int32_t, Vec3f, USENode, boost::recursive_wrapper<struct VRMLNode>, std::vector<boost::variant<boost::recursive_wrapper<struct VRMLNode>, boost::recursive_wrapper<struct USENode>>>>;

        struct VRMLField : public Printable {
            std::string name;
            VRMLFieldValue value;

            VRMLField()
                : Printable(std::cout) {}

            VRMLField(const std::string& n, const VRMLFieldValue& v) : Printable(std::cout), name(n), value(v) {}

            void Print(Printable::IndentationLevel indentation_level) const override;
        };
    }
}
