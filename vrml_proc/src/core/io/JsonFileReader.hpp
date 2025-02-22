#pragma once

#include <fstream>

#include <nlohmann/json.hpp>

#include "FileReader.hpp"
#include "VrmlProcessingExport.hpp"

namespace vrml_proc::core::io {

	class VRMLPROCESSING_API JsonFileReader : public FileReader<nlohmann::json> {
	public:
		LoadFileResult Read(const std::filesystem::path& filepath) override {
			std::ifstream file(filepath.string());
			nlohmann::json config;
			file >> config;
			return config;
		}
	};
}
