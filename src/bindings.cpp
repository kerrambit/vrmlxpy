#include "vrmlxpy.hpp"

#include <pybind11/pybind11.h>
#include <pybind11/attr.h>
#include <pybind11/cast.h>

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