#include <catch2/catch_test_macros.hpp>
#include <string>

#include <vrml_processing.hpp>

#include "test-data/Int32ArrayGrammarTestDataset.hpp"

TEST_CASE("Parse Int32Array - Valid Input", "[parsing]") {
    bool parseResult = vrml_proc::parseInt32Array(int32_array_simple);
    REQUIRE(parseResult);
}