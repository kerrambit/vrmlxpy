#pragma once

#include <filesystem>
#include <memory>

#include <result.hpp>

#include <nlohmann/json.hpp>

#include "Error.hpp"
#include "ExportFormats.hpp"
#include "JsonError.hpp"
#include "JsonFileReader.hpp"
#include "Logger.hpp"
#include "VrmlProcConfig.hpp"
#include <FormatString.hpp>

namespace to_geom::core::config {

  inline to_geom::core::io::ExportFormat StringToExportFormat(const std::string& str) {
    if (str == "stl" || str == "STL") return to_geom::core::io::ExportFormat::Stl;
    if (str == "ply" || str == "PLY") return to_geom::core::io::ExportFormat::Ply;
    if (str == "obj" || str == "OBJ") return to_geom::core::io::ExportFormat::Obj;

    vrml_proc::core::logger::LogWarning(vrml_proc::core::utils::FormatString(
                                            "While parsing JSON field <exportFormat>, an error occured. The value <",
                                            str, "> is not an expected value! The default value (<stl>) will be used."),
                                        LOGGING_INFO);

    return to_geom::core::io::ExportFormat::Stl;
  }

  struct ToGeomConfig {
    ToGeomConfig() = default;

    vrml_proc::core::config::VrmlProcConfig vrmlProcConfig;
    to_geom::core::io::ExportFormat exportFormat = to_geom::core::io::ExportFormat::Stl;

    cpp::result<void, std::shared_ptr<vrml_proc::core::error::Error>> LoadFromJsonFile(
        const std::filesystem::path& filepath) {
      vrml_proc::core::io::JsonFileReader reader;
      auto json = reader.Read(filepath);

      if (json.has_value()) {
        try {
          vrmlProcConfig.ignoreUnknownNode = json.value().value("ignoreUnknownNode", false);
          exportFormat = StringToExportFormat(json.value().value("exportFormat", "stl"));
        } catch (const nlohmann::json::exception& e) {
          return cpp::fail(std::make_shared<vrml_proc::core::error::JsonError>(e.what()));
        }

        return {};
      }

      return cpp::fail(json.error());
    }
  };
}  // namespace to_geom::core::config
