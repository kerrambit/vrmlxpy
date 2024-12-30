#pragma once

#include "VrmlProcessingExport.hpp"

namespace vrml_proc {
	namespace core {

		template <typename T>
		class VRMLPROCESSING_API FileWriter {
		public:

			using FileWriteResult = bool;

			virtual ~FileWriter() = default;

			virtual FileWriteResult Write(const std::filesystem::path& filepath, const T& data) = 0;
		};
	}
}
