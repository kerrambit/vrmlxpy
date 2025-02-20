#pragma once

#include "FileReader.hpp"
#include "VrmlProcessingExport.hpp"

namespace vrml_proc::core::io {
	class VRMLPROCESSING_API SimpleFileReader : public FileReader {
	public:
		LoadFileResult LoadFile(const std::filesystem::path& filepath) override;
	};
}
