#pragma once

#include <string>
#include <iostream>

#include <boost/variant.hpp>
#include <boost/variant/recursive_wrapper.hpp>

#include "VrmlUnits.hpp"
#include "Vec3f.hpp"
#include "Vec4f.hpp"
#include "Vec3fArray.hpp"
#include "Int32Array.hpp"
#include "UseNode.hpp"

#include "Printable.hpp"
#include "VrmlProcessingExport.hpp"

namespace vrml_proc {
    namespace parser {

        struct VrmlNode;
        using VrmlFieldValue = boost::variant<std::string, bool, Vec3fArray, Int32Array, float32_t, int32_t, Vec4f, Vec3f, UseNode, boost::recursive_wrapper<struct VrmlNode>, std::vector<boost::variant<boost::recursive_wrapper<struct VrmlNode>, boost::recursive_wrapper<struct UseNode>>>>;
        using VrmlNodeArray = std::vector<boost::variant<boost::recursive_wrapper<struct VrmlNode>, boost::recursive_wrapper<struct UseNode>>>;

        struct VRMLPROCESSING_API VrmlField : public Printable {
            std::string name;
            VrmlFieldValue value;

            VrmlField()
                : Printable(std::cout) {}

            VrmlField(const std::string& n, const VrmlFieldValue& v) : Printable(std::cout), name(n), value(v) {}

            void Print(Printable::IndentationLevel indentationLevel) const override;
        };
    }
}
