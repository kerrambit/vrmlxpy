#include <pybind11/pybind11.h>

#include <iostream>

namespace test {
    void test() {
        std::cout << "Hello" << std::endl;
    }
}

#include <VrmlToStlConvertor.hpp>
#include <MemoryMappedFileReader.hpp>

namespace vrmlxpy {
    bool ConvertVrmlToStl(const std::string& inputFilename, const std::string& outputFilename) {
        return false;
    }
}

namespace py = pybind11;

PYBIND11_MODULE(vrmlxpy, m) {
    m.doc() = "Python bindings for vrmlxpy.";
    m.def("print_message", &test::test, "A testing function that prints a message.");
}
