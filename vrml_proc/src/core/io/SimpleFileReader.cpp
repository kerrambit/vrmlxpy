#include "SimpleFileReader.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "FormatString.hpp"
#include "Logger.hpp"
#include "ScopedTimer.hpp"

vrml_proc::core::io::FileReader::LoadFileResult vrml_proc::core::io::SimpleFileReader::LoadFile(const std::filesystem::path& filepath)
{
    double time;
    std::size_t fileSize;
    std::string fileContents;
    {
        auto timer = vrmlproc::core::utils::ScopedTimer(time);
        std::ifstream file(filepath.string(), std::ios::in);

        if (!file) {
            return { };
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        fileContents = buffer.str();
        fileSize = fileContents.size();
    }

    vrml_proc::core::logger::LogInfo(vrml_proc::core::utils::FormatString("Time to read and load file <", filepath.string(), "> of size ", fileSize, " bytes into string took ", time, " seconds."), LOGGING_INFO);
    return { std::move(fileContents) };
}
