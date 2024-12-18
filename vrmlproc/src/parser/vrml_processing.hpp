#pragma once

#include <string>
#include <optional>

#include "ParserResult.hpp"
#include "VRMLFile.hpp"

#include "VrmlProcessingExport.hpp"

namespace vrml_proc {
    namespace parser {

        VRMLPROCESSING_API void printMessage();

        VRMLPROCESSING_API bool parseVec3f(std::string& text);

        VRMLPROCESSING_API bool parseVec4f(std::string& text);

        VRMLPROCESSING_API bool parseVec3fArray(std::string& text);

        VRMLPROCESSING_API bool parseInt32Array(std::string& text);

        VRMLPROCESSING_API ParserResult<VRMLFile> ParseVrmlFile(std::string& text);

        using LoadFileResult = std::optional<std::string>;

        VRMLPROCESSING_API LoadFileResult LoadFile(const std::string& filepath);

        VRMLPROCESSING_API std::string test_boost();
    }
}