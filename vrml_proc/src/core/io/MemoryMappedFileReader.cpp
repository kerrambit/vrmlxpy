#include "MemoryMappedFileReader.hpp"

#include <string>

#include <boost/iostreams/device/mapped_file.hpp>

#include "Logger.hpp"
#include "FormatString.hpp"
#include "ScopedTimer.hpp"

vrml_proc::core::io::FileReader::LoadFileResult vrml_proc::core::io::MemoryMappedFileReader::LoadFile(const std::filesystem::path& filepath)
{
    double time;
    std::size_t fileSize;
    std::string fileContents;
    {
        auto timer = vrmlproc::core::utils::ScopedTimer(time);
        boost::iostreams::mapped_file mappedFile(filepath.string(), boost::iostreams::mapped_file::readonly);
        fileContents = mappedFile.const_data();
        fileSize = mappedFile.size();
    }

    vrml_proc::core::logger::LogInfo(vrml_proc::core::utils::FormatString("Time to read and load file <", filepath.string(), "> of size ", fileSize, " bytes into string took ", time, " seconds."), LOGGING_INFO);
    return { std::move(fileContents) };
}
