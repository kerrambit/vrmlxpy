#pragma once

#include <string>
#include <vector>
#include <iostream>

#include <boost/optional.hpp>

#include "VRMLField.hpp"
#include "Printable.hpp"

#include "VrmlProcessingExport.hpp"

namespace vrml_proc {
    namespace parser {
        struct VRMLPROCESSING_API VRMLNode : public Printable {

            VRMLNode()
                : Printable(std::cout) {}

            boost::optional<std::string> definition_name;
            std::string header;
            std::vector<VRMLField> fields;

            void Print(Printable::IndentationLevel indentation_level) const override;
        };
    }
}

