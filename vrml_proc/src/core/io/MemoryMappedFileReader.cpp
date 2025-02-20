#include "MemoryMappedFileReader.hpp"

#include <memory>
#include <string>

#include <boost/iostreams/device/mapped_file.hpp>

#include "Error.hpp"
#include "FileReader.hpp"
#include "FormatString.hpp"
#include "IoError.hpp"
#include "Logger.hpp"
#include "ScopedTimer.hpp"

vrml_proc::core::io::FileReader<std::string>::LoadFileResult vrml_proc::core::io::MemoryMappedFileReader::Read(const std::filesystem::path& filepath)
{
    using namespace vrml_proc::core::error;
    using namespace vrml_proc::core::io::error;
    using namespace vrml_proc::core::logger;
    using namespace vrml_proc::core::utils;

    LogInfo(FormatString("Read file <", filepath.string(), ">."), LOGGING_INFO);

    if (!std::filesystem::exists(filepath)) {
        std::shared_ptr<Error> error = std::make_shared<IoError>();
        return cpp::fail(error << (std::make_shared<FileNotFoundError>(filepath.string())));
    }

    double time;
    std::size_t fileSize;
    std::string fileContents;
    {
        auto timer = vrmlproc::core::utils::ScopedTimer(time);
        boost::iostreams::mapped_file mappedFile(filepath.string(), boost::iostreams::mapped_file::readonly);
        fileContents = mappedFile.const_data();
        fileSize = mappedFile.size();
    }

    LogInfo(FormatString("Time to read and load file <", filepath.string(), "> of size ", fileSize, " bytes into string took ", time, " seconds."), LOGGING_INFO);
    return std::move(fileContents);
}
