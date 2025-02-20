#pragma once

#include <string>

#include "FileReader.hpp"
#include "VrmlProcessingExport.hpp"

namespace vrml_proc::core::io {
	class VRMLPROCESSING_API MemoryMappedFileReader : public FileReader<std::string> {
	public:
		LoadFileResult Read(const std::filesystem::path& filepath) override;
	};
}
