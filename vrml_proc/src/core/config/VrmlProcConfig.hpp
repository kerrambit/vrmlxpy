#pragma once

#include <filesystem>
#include <memory>
#include <optional>

#include "Error.hpp"
#include "JsonFileReader.hpp"

namespace vrml_proc::core::config {

	struct VrmlProcConfig {

		std::optional<std::shared_ptr<vrml_proc::core::error::Error>> LoadFromJsonFile(const std::filesystem::path& filepath) {

			vrml_proc::core::io::JsonFileReader reader;
			auto json = reader.Read(filepath);

			if (json.has_value()) {
				// TODO: Set up variables.
			}
			else {
				return json.error();
			}
		}
	};
}
