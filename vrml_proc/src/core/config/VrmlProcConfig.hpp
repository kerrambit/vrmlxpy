#pragma once

#include <filesystem>
#include <memory>

#include <result.hpp>

#include <nlohmann/json.hpp>

#include "Error.hpp"
#include "JsonError.hpp"
#include "JsonFileReader.hpp"

namespace vrml_proc::core::config {

  struct VrmlProcConfig {
    VrmlProcConfig() = default;

    bool ignoreUnknownNode = false;
    std::string logFileDirectory = std::filesystem::current_path().string();
    std::string logFileName = "vrmlproc";

    cpp::result<void, std::shared_ptr<vrml_proc::core::error::Error>> LoadFromJsonFile(
        const std::filesystem::path& filepath) {
      vrml_proc::core::io::JsonFileReader reader;
      auto json = reader.Read(filepath);

      if (json.has_value()) {
        try {
          ignoreUnknownNode = json.value().value("ignoreUnknownNode", false);
          logFileDirectory = json.value().value("logFileDirectory", std::filesystem::current_path().string());
          logFileName = json.value().value("logFileName", "vrmlproc");
        } catch (const nlohmann::json::exception& e) {
          return cpp::fail(std::make_shared<vrml_proc::core::error::JsonError>(e.what()));
        }

        return {};
      }

      return cpp::fail(json.error());
    }
  };
}  // namespace vrml_proc::core::config
