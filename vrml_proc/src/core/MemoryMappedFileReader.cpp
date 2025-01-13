#include "MemoryMappedFileReader.hpp"

#include <boost/iostreams/device/mapped_file.hpp>

#include <iostream>
#include <string>

#ifdef RELEASE
#include "ScopedTimer.hpp"
#endif

vrml_proc::core::FileReader::LoadFileResult vrml_proc::core::MemoryMappedFileReader::LoadFile(const std::filesystem::path& filepath)
{
#ifdef RELEASE
    double time;
#endif
    std::string fileContents;
    {
#ifdef RELEASE
        auto timer = vrmlproc::core::utils::ScopedTimer(time);
#endif
        boost::iostreams::mapped_file mappedFile(filepath.string(), boost::iostreams::mapped_file::readonly);
        fileContents =  mappedFile.const_data();
    }
#ifdef RELEASE
    std::cout << "Time to read and load file <" << filepath.string() << "> into string took " << time << " seconds." << std::endl;
#endif
    return { std::move(fileContents) };
}
