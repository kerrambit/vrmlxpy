#include <pybind11/pybind11.h>
#include <pybind11/attr.h>
#include <pybind11/cast.h>

#include <memory>
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
#include <ToGeomActionMap.hpp>
#include <StlFileWriter.hpp>
#include <ObjFileWriter.hpp>
#include <PlyFileWriter.hpp>
#include <VrmlFileTraversor.hpp>
#include <VrmlNodeManager.hpp>
#include <VrmlParser.hpp>
#include <VrmlProcConfig.hpp>
#include <ToGeomConfig.hpp>
#include <ExportFormats.hpp>
#include <FileWriter.hpp>

namespace vrmlxpy {

  static void PrintVersion() { std::cout << "(TMP): vrmlxpy 0.1 (togeom 0.1; vrmlproc 1.0)" << std::endl; }

  static bool HelperConvertVrmlToGeom(const std::string& inputFilename, const std::string& outputFilename,
                                      const to_geom::core::config::ToGeomConfig& config) {
    vrml_proc::core::io::MemoryMappedFileReader reader;
    auto readResult = reader.Read(std::filesystem::path(inputFilename));
    if (readResult.has_error()) {
      std::cout << "Caught an application error:\n" << readResult.error()->GetMessage() << std::endl;
      return false;
    }

    std::cout << "2/6: file <" << std::filesystem::path(inputFilename).string() << "> was succesfully read."
              << std::endl;

    vrml_proc::parser::VrmlNodeManager manager;
    vrml_proc::parser::VrmlParser parser(manager);
    auto parseResult =
        parser.Parse(vrml_proc::parser::BufferView(readResult.value().GetBegin(), readResult.value().GetEnd()));
    if (parseResult.has_error()) {
      std::cout << "Caught an application error:\n" << parseResult.error()->GetMessage() << std::endl;
      return false;
    }

    std::cout << "3/6: file <" << std::filesystem::path(inputFilename).string() << "> was succesfully parsed."
              << std::endl;

    auto convertResult =
        vrml_proc::traversor::VrmlFileTraversor::Traverse<to_geom::conversion_context::MeshTaskConversionContext>(
            {parseResult.value(), manager, config.vrmlProcConfig}, to_geom::conversion_context::CreateActionMap());
    if (convertResult.has_error()) {
      std::cout << "Caught an application error:\n" << convertResult.error()->GetMessage() << std::endl;
      return false;
    }

    std::cout << "4/6: file <" << std::filesystem::path(inputFilename).string() << "> was succesfully traversed."
              << std::endl;

    std::vector<std::future<to_geom::calculator::CalculatorResult>> results;
    for (const auto& task : convertResult.value()->GetData()) {
      if (task) {
        results.emplace_back(std::async(std::launch::async, task));
      }
    }

    to_geom::core::Mesh mesh;
    for (auto& future : results) {
      auto meshResult = future.get();
      if (meshResult.has_value()) {
        mesh.join(*(meshResult.value()));
      } else {
        std::cout << "Warning: invalid submesh!" << std::endl;
      }
    };

    std::cout << "5/6: mesh was succesfully generated." << std::endl;

    std::unique_ptr<vrml_proc::core::io::FileWriter<to_geom::core::Mesh>> writer;
    switch (config.exportFormat) {
      case to_geom::core::io::ExportFormat::Stl:
        writer = std::make_unique<to_geom::core::io::StlFileWriter>();
        break;
      case to_geom::core::io::ExportFormat::Ply:
        writer = std::make_unique<to_geom::core::io::PlyFileWriter>();
        break;
      case to_geom::core::io::ExportFormat::Obj:
        writer = std::make_unique<to_geom::core::io::ObjFileWriter>();
        break;
      default:
        writer = std::make_unique<to_geom::core::io::StlFileWriter>();
        break;
    }

    auto writeResult = writer->Write(std::filesystem::path(outputFilename), mesh);
    if (writeResult.has_error()) {
      std::cout << "Caught an aplication error:\n" << writeResult.error()->GetMessage() << std::endl;
      return false;
    }

    std::cout << "6/6: file <" << std::filesystem::path(outputFilename).string() << "> was succesfully written."
              << std::endl;

    std::cout << "Converting VRML to geometry format finished succesfully." << std::endl;
    return true;
  }

  static bool ConvertVrmlToGeom(const std::string& inputFilename, const std::string& outputFilename,
                                const std::string& configFilename) {
    vrml_proc::core::logger::InitLogging(std::filesystem::current_path().string(), "vrmlxpy");

    std::cout << "Converting VRML to geometry format..." << std::endl;

    to_geom::core::config::ToGeomConfig config;
    auto configResult = config.LoadFromJsonFile(configFilename);
    if (configResult.has_error()) {
      std::cout << "Caught an application error:\n" << configResult.error()->GetMessage() << std::endl;
      return false;
    }

    std::cout << "1/6: configuration file <" << std::filesystem::path(configFilename).string()
              << "> was succesfully parsed." << std::endl;

    return HelperConvertVrmlToGeom(inputFilename, outputFilename, config);
  }

  static bool ConvertVrmlToStl(const std::string& inputFilename, const std::string& outputFilename) {
    to_geom::core::config::ToGeomConfig config;
    config.exportFormat = to_geom::core::io::ExportFormat::Stl;
    return HelperConvertVrmlToGeom(inputFilename, outputFilename, config);
  }

  static bool ConvertVrmlToPly(const std::string& inputFilename, const std::string& outputFilename) {
    to_geom::core::config::ToGeomConfig config;
    config.exportFormat = to_geom::core::io::ExportFormat::Ply;
    return HelperConvertVrmlToGeom(inputFilename, outputFilename, config);
  }

  static bool ConvertVrmlToObj(const std::string& inputFilename, const std::string& outputFilename) {
    to_geom::core::config::ToGeomConfig config;
    config.exportFormat = to_geom::core::io::ExportFormat::Obj;
    return HelperConvertVrmlToGeom(inputFilename, outputFilename, config);
  }
}  // namespace vrmlxpy

namespace py = pybind11;

PYBIND11_MODULE(vrmlxpy, m) {
  m.doc() = "Python bindings for vrmlxpy.";

  m.def("print_version", &vrmlxpy::PrintVersion, "A function that prints a current vrmlxpy version.");

  m.def("convert_vrml_to_stl", py::overload_cast<const std::string&, const std::string&>(&vrmlxpy::ConvertVrmlToStl),
        "Converts a VRML file to STL", py::arg("input_filename"), py::arg("output_filename"));

  m.def("convert_vrml_to_ply", py::overload_cast<const std::string&, const std::string&>(&vrmlxpy::ConvertVrmlToPly),
        "Converts a VRML file to PLY", py::arg("input_filename"), py::arg("output_filename"));

  m.def("convert_vrml_to_obj", py::overload_cast<const std::string&, const std::string&>(&vrmlxpy::ConvertVrmlToObj),
        "Converts a VRML file to OBJ", py::arg("input_filename"), py::arg("output_filename"));

  m.def("convert_vrml",
        py::overload_cast<const std::string&, const std::string&, const std::string&>(&vrmlxpy::ConvertVrmlToGeom),
        "Converts a VRML file to a geometry format based in a config file", py::arg("input_filename"),
        py::arg("output_filename"), py::arg("config_filename"));
}
