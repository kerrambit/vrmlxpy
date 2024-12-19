#include <vrml_processing.hpp>

#include <boost/filesystem.hpp>
#include <iostream>

std::string vrml_proc::parser::test_boost() {
    boost::filesystem::path path(".");
    return path.string();
}