#pragma once

#include <string>

namespace vrmlxpy {

  void PrintVersion();

  std::string GetExpectedOutputFileExtension(const std::string& configFilename);

  bool ConvertVrmlToGeom(const std::string& inputFilename, const std::string& outputFilename,
                         const std::string& configFilename);
  bool ConvertVrmlToStl(const std::string& inputFilename, const std::string& outputFilename);

  bool ConvertVrmlToPly(const std::string& inputFilename, const std::string& outputFilename);

  bool ConvertVrmlToObj(const std::string& inputFilename, const std::string& outputFilename);

}  // namespace vrmlxpy
