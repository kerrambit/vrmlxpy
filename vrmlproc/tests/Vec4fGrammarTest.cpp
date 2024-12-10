#include <catch2/catch_test_macros.hpp>
#include <string>

#include <vrml_processing.hpp>

#include "test-data/Vec4fGrammarTestDataset.hpp"

TEST_CASE("Parse Vec4f - Valid Input", "[parsing]") {
    bool parseResult = vrml_proc::parseVec4f(vec4f_simple);
    REQUIRE(parseResult);
}