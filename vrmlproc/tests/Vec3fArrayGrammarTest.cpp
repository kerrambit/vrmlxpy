#include <catch2/catch_test_macros.hpp>
#include <string>

#include <vrml_processing.hpp>

#include "test-data/Vec3fArrayGrammarTestDataset.hpp"

TEST_CASE("Parse Vec3fArray - Valid Input", "[parsing]") {
    bool parseResult = vrml_proc::parseVec3fArray(vec3f_array_simple);
    REQUIRE(parseResult);
}   