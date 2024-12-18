#include <catch2/catch_test_macros.hpp>

#include <vrml_processing.hpp>

#include "test-data/VRMLFileGrammarTestDataset.hpp"

TEST_CASE("Boost static libraries have been linked.", "[linking]") {

    CHECK(vrml_proc::parser::test_boost() == ".");
}