#pragma once

#include <string>
#include <optional>

#ifdef _WIN32
    #define VRMLPROCESSING_API __declspec(dllexport)
#else
    #define VRMLPROCESSING_API
#endif

namespace vrml_proc {

    VRMLPROCESSING_API void printMessage();

    VRMLPROCESSING_API bool parseVec3f(std::string& text);

    VRMLPROCESSING_API bool parseVec4f(std::string& text);

    VRMLPROCESSING_API bool parseVec3fArray(std::string& text);

    VRMLPROCESSING_API bool parseInt32Array(std::string& text);

    VRMLPROCESSING_API bool parseVRMLFile(std::string& text);

    using LoadFileResult = std::optional<std::string>;

    VRMLPROCESSING_API LoadFileResult LoadFile(const std::string& filepath);


    VRMLPROCESSING_API void test_boost();
}