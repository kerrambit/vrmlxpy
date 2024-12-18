#include <catch2/catch_test_macros.hpp>
#include <string>

#include <vrml_processing.hpp>

#include "test-data/Vec3fGrammarTestDataset.hpp"

TEST_CASE("Parse Vec3f - Valid Input", "[parsing]") {
    bool parseResult = vrml_proc::parser::parseVec3f(vec3f_simple);
    REQUIRE(parseResult);
}