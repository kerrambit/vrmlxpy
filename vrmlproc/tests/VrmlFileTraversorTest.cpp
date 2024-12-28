#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <memory>
#include <vector>

#include <boost/variant/get.hpp>
#include <boost/variant/recursive_wrapper.hpp>
#include <boost/variant/static_visitor.hpp>
#include <boost/variant/variant.hpp>

#include "test-data/VrmlFileTraversorTestDataset.hpp"
#include <boost/optional/optional.hpp>
#include <boost/variant/detail/apply_visitor_unary.hpp>
#include <Int32Array.hpp>
#include <MemoryMappedFileReader.hpp>
#include <MeshConversionContext.hpp>
#include <ParserResult.hpp>
#include <SimpleFileReader.hpp>
#include <UseNode.hpp>
#include <Vec3f.hpp>
#include <Vec3fArray.hpp>
#include <VrmlField.hpp>
#include <VrmlFile.hpp>
#include <VrmlFileTraversor.hpp>
#include <VrmlNode.hpp>
#include <VrmlNodeManager.hpp>
#include <VrmlParser.hpp>
#include <VrmlUnits.hpp>

static vrml_proc::parser::ParserResult<vrml_proc::parser::VrmlFile> ParseVrmlFile(std::string& text, vrml_proc::parser::VrmlNodeManager& manager) {

    vrml_proc::parser::VrmlParser parser(manager);
    return parser.Parse(text);
}

static vrml_proc::parser::ParserResult<vrml_proc::parser::VrmlFile> ParseVrmlFile(const std::filesystem::path& filepath, vrml_proc::parser::VrmlNodeManager& manager) {

    vrml_proc::core::MemoryMappedFileReader reader;
    auto readResult = reader.LoadFile(filepath);
    if (!readResult.has_value()) {
        return {};
    }

    vrml_proc::parser::VrmlParser parser(manager);
    return parser.Parse(readResult.value());
}

// ------------------------------------------------------------------------------------------------------------------------------------------------ //

//TEST_CASE("Parse VRML File - Valid Input - Simple VRML File", "[parsing][valid]") {
//
//    vrml_proc::parser::VrmlNodeManager manager;
//    auto parseResult = ParseVrmlFile(validGroup, manager);
//    REQUIRE(parseResult);
//
//    vrml_proc::traversor::VrmlFileTraversor traversor;
//    auto traversorResult = traversor.Traverse({ parseResult.value(), manager});
//
//    std::shared_ptr<vrml_proc::conversion_context::MeshConversionContext> meshContextPtr = std::dynamic_pointer_cast<vrml_proc::conversion_context::MeshConversionContext>(traversorResult);
//    REQUIRE(meshContextPtr);
//}