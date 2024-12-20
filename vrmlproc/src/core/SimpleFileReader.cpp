#include "SimpleFileReader.hpp"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

vrml_proc::core::FileReader::LoadFileResult vrml_proc::core::SimpleFileReader::LoadFile(const std::filesystem::path& filepath)
{
    std::ifstream file(filepath.string(), std::ios::in);

    if (!file) {
        return { };
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string file_contents = buffer.str();

    return { std::move(file_contents) };
}
