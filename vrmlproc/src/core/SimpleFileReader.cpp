#include "SimpleFileReader.hpp"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#ifdef DEBUG
#include "ScopedTimer.hpp"
#endif

vrml_proc::core::FileReader::LoadFileResult vrml_proc::core::SimpleFileReader::LoadFile(const std::filesystem::path& filepath)
{
#ifdef DEBUG
    double time;
#endif
    std::string fileContents;
    {
#ifdef DEBUG
        auto timer = vrmlproc::core::utils::ScopedTimer(time);
#endif
        std::ifstream file(filepath.string(), std::ios::in);

        if (!file) {
            return { };
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        fileContents = buffer.str();
    }
#ifdef DEBUG
    std::cout << "Time to read and load file <" << filepath.string() << "> into string took " << time << " seconds." << std::endl;
#endif
    return { std::move(fileContents) };
}
