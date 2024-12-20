#include <SetupIntegrity.hpp>

#include <boost/filesystem.hpp>

std::string vrml_proc::parser::test_boost() {
    boost::filesystem::path path(".");
    return path.string();
}