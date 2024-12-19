#pragma once

#include <string>
#include <optional>

#include "VrmlProcessingExport.hpp"

namespace vrml_proc {
    namespace parser {

        using LoadFileResult = std::optional<std::string>;

        VRMLPROCESSING_API LoadFileResult LoadFile(const std::string& filepath);

        VRMLPROCESSING_API std::string test_boost();
    }
}