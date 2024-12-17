#include <vrml_processing.hpp>

#include <boost/filesystem.hpp>
#include <iostream>

void vrml_proc::test_boost() {
    boost::filesystem::path p(".");
    std::cout << "Current path is: " << p << std::endl;
}