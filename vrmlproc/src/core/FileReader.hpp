#pragma once

#include <string>
#include <optional>
#include <filesystem>

#include "VrmlProcessingExport.hpp"

namespace vrml_proc {
	namespace core {

		class VRMLPROCESSING_API FileReader {
		public:

			using LoadFileResult = std::optional<std::string>;

			virtual ~FileReader() = default;

			virtual LoadFileResult LoadFile(const std::filesystem::path& filepath) = 0;
		};
	}
}
