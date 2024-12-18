#include <vrml_processing.hpp>

#include <boost/filesystem.hpp>
#include <iostream>

std::string vrml_proc::test_boost() {
    boost::filesystem::path p(".");
    return p.string();
}