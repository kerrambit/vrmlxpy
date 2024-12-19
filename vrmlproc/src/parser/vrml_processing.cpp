#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "vrml_processing.hpp"

namespace vrml_proc::parser {
    LoadFileResult LoadFile(const std::string& filepath) {

        std::ifstream file(filepath, std::ios::in);

        if (!file) {
            return { };
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string file_contents = buffer.str();

        return { std::move(file_contents) };
    }
}