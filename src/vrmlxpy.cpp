#include <pybind11/pybind11.h>
#include <pybind11/attr.h>
#include <pybind11/cast.h>

#include <filesystem>
#include <future>
#include <iostream>
#include <string>
#include <vector>

#include <BufferView.hpp>
#include <CalculatorResult.hpp>
#include <Logger.hpp>
#include <MemoryMappedFileReader.hpp>
#include <Mesh.hpp>
#include <MeshTaskConversionContext.hpp>
#include <StlActionMap.hpp>
#include <StlFileWriter.hpp>
#include <VrmlFileTraversor.hpp>
#include <VrmlNodeManager.hpp>
#include <VrmlParser.hpp>
#include <VrmlProcConfig.hpp>

namespace vrmlxpy {

  void PrintVersion() { std::cout << "(TMP): vrmlxpy 0.1 (tostl 0.1; vrmlproc 1.0)" << std::endl; }

  bool ConvertVrmlToStl(const std::string& inputFilename, const std::string& outputFilename,
                        const std::string& configFilename) {
    vrml_proc::core::logger::InitLogging(std::filesystem::current_path().string(), "vrmlxpy");

    std::cout << "Conversion from VRML to STL file has begun." << std::endl;

    vrml_proc::core::config::VrmlProcConfig config;
    auto configResult = config.LoadFromJsonFile(configFilename);
    if (configResult.has_error()) {
      std::cout << "Caught an application error:\n" << configResult.error()->GetMessage() << std::endl;
      return false;
    }

    std::cout << "Configuration file " << std::filesystem::path(configFilename).string() << " was succesfully parsed."
              << std::endl;

    vrml_proc::core::io::MemoryMappedFileReader reader;
    auto readResult = reader.Read(std::filesystem::path(inputFilename));
    if (readResult.has_error()) {
      std::cout << "Caught an application error:\n" << readResult.error()->GetMessage() << std::endl;
      return false;
    }

    std::cout << "File " << std::filesystem::path(inputFilename).string() << " was succesfully read." << std::endl;

    vrml_proc::parser::VrmlNodeManager manager;
    vrml_proc::parser::VrmlParser parser(manager);
    auto parseResult =
        parser.Parse(vrml_proc::parser::BufferView(readResult.value().GetBegin(), readResult.value().GetEnd()));
    if (parseResult.has_error()) {
      std::cout << "Caught an application error:\n" << parseResult.error()->GetMessage() << std::endl;
      return false;
    }

    std::cout << "File " << std::filesystem::path(inputFilename).string() << " was succesfully parsed." << std::endl;

    auto convertResult =
        vrml_proc::traversor::VrmlFileTraversor::Traverse<to_stl::conversion_context::MeshTaskConversionContext>(
            {parseResult.value(), manager, config}, to_stl::conversion_context::CreateActionMap());
    if (convertResult.has_error()) {
      std::cout << "Caught an application error:\n" << convertResult.error()->GetMessage() << std::endl;
      return false;
    }

    std::cout << "File " << std::filesystem::path(inputFilename).string() << " was succesfully traversed." << std::endl;

    std::vector<std::future<to_stl::calculator::CalculatorResult>> results;
    for (const auto& task : convertResult.value()->GetData()) {
      if (task) {
        results.emplace_back(std::async(std::launch::async, task));
      }
    }

    to_stl::core::Mesh mesh;
    for (auto& future : results) {
      auto meshResult = future.get();
      if (meshResult.has_value()) {
        mesh.join(*(meshResult.value()));
      } else {
        std::cout << "Error in mesh!" << std::endl;
      }
    };

    std::cout << "Generation of mesh has finished." << std::endl;

    to_stl::core::io::StlFileWriter writer;
    auto writeResult = writer.Write(std::filesystem::path(outputFilename), mesh);
    if (writeResult.has_error()) {
      std::cout << "Caught an aplication error:\n" << writeResult.error()->GetMessage() << std::endl;
      return false;
    }

    std::cout << "File " << std::filesystem::path(outputFilename).string() << " was succesfully written." << std::endl;

    std::cout << "Conversion finished succesfully!" << std::endl;
    return true;
  }

  bool ConvertVrmlToStl(const std::string& inputFilename, const std::string& outputFilename) {
    vrml_proc::core::logger::InitLogging(R"(C:\Users\marek\Documents\FI_MUNI\sem_05\SBAPR\vrmlxpy\out\build\Debug)",
                                         "vrmlxpy");

    std::cout << "Conversion from VRML to STL file has begun." << std::endl;

    vrml_proc::core::config::VrmlProcConfig config;

    vrml_proc::core::io::MemoryMappedFileReader reader;
    auto readResult = reader.Read(std::filesystem::path(inputFilename));
    if (readResult.has_error()) {
      std::cout << "Caught an application error:\n" << readResult.error()->GetMessage() << std::endl;
      return false;
    }

    std::cout << "File " << std::filesystem::path(inputFilename).string() << " was succesfully read." << std::endl;

    vrml_proc::parser::VrmlNodeManager manager;
    vrml_proc::parser::VrmlParser parser(manager);
    auto parseResult =
        parser.Parse(vrml_proc::parser::BufferView(readResult.value().GetBegin(), readResult.value().GetEnd()));
    if (parseResult.has_error()) {
      std::cout << "Caught an application error:\n" << parseResult.error()->GetMessage() << std::endl;
      return false;
    }

    std::cout << "File " << std::filesystem::path(inputFilename).string() << " was succesfully parsed." << std::endl;

    auto convertResult =
        vrml_proc::traversor::VrmlFileTraversor::Traverse<to_stl::conversion_context::MeshTaskConversionContext>(
            {parseResult.value(), manager, config}, to_stl::conversion_context::CreateActionMap());
    if (convertResult.has_error()) {
      std::cout << "Caught an application error:\n" << convertResult.error()->GetMessage() << std::endl;
      std::shared_ptr<vrml_proc::traversor::error::FileTraversorError> error =
          std::dynamic_pointer_cast<vrml_proc::traversor::error::FileTraversorError>(convertResult.error());
      if (error != nullptr) {
        std::cout << "VRML root node where the error occured:\n" << error->GetErrorNode() << std::endl;
      }
      return false;
    }

    std::cout << "File " << std::filesystem::path(inputFilename).string() << " was succesfully traversed." << std::endl;

    std::vector<std::future<to_stl::calculator::CalculatorResult>> results;
    for (const auto& task : convertResult.value()->GetData()) {
      if (task) {
        results.emplace_back(std::async(std::launch::async, task));
      }
    }

    to_stl::core::Mesh mesh;
    for (auto& future : results) {
      auto meshResult = future.get();
      if (meshResult.has_value()) {
        mesh.join(*(meshResult.value()));
      } else {
        std::cout << "Error in mesh!" << std::endl;
      }
    };

    std::cout << "Generation of mesh has finished." << std::endl;

    to_stl::core::io::StlFileWriter writer;
    auto writeResult = writer.Write(std::filesystem::path(outputFilename), mesh);
    if (writeResult.has_error()) {
      std::cout << "Caught an aplication error:\n" << writeResult.error()->GetMessage() << std::endl;
      return false;
    }

    std::cout << "File " << std::filesystem::path(outputFilename).string() << " was succesfully written." << std::endl;

    std::cout << "Conversion finished succesfully!" << std::endl;
    return true;
  }
}  // namespace vrmlxpy

namespace py = pybind11;

PYBIND11_MODULE(vrmlxpy, m) {
  m.doc() = "Python bindings for vrmlxpy.";

  m.def("print_version", &vrmlxpy::PrintVersion, "A function that prints a current vrmlxpy version.");

  m.def("convert_vrml_to_stl", py::overload_cast<const std::string&, const std::string&>(&vrmlxpy::ConvertVrmlToStl),
        "Converts a VRML file to STL", py::arg("input_filename"), py::arg("output_filename"));

  m.def("convert_vrml_to_stl",
        py::overload_cast<const std::string&, const std::string&, const std::string&>(&vrmlxpy::ConvertVrmlToStl),
        "Converts a VRML file to STL with a config file", py::arg("input_filename"), py::arg("output_filename"),
        py::arg("config_filename"));
}
