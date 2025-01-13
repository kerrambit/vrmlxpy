#pragma once

#include "FileReader.hpp"
#include "VrmlProcessingExport.hpp"

namespace vrml_proc {
	namespace core {

		class VRMLPROCESSING_API SimpleFileReader : public FileReader {
		public:
			LoadFileResult LoadFile(const std::filesystem::path& filepath) override;
		};
	}
}
