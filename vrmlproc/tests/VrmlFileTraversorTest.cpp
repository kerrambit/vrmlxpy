#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <memory>
#include <vector>
#include <any>

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
#include <BaseConversionContextActionMap.hpp>
#include <SpotlightAction.hpp>
#include <GroupAction.hpp>

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

static vrml_proc::action::BaseConversionContextActionMap& GetActionMap() {

    static vrml_proc::action::BaseConversionContextActionMap actionMap;

    actionMap.AddAction("Spotlight", [](const std::vector<std::any>& args) {
        if (args.size() == 1 && args[0].type() == typeid(float)) {
            return std::make_shared<vrml_proc::action::SpotlightAction>(std::any_cast<float>(args[0]));
        }
        throw std::invalid_argument("Invalid arguments for SpotlightAction");
        });

    actionMap.AddAction("Group", [](const std::vector<std::any>& args) {

        if (args.size() == 3 &&
            args[0].type() == typeid(std::vector<std::shared_ptr<vrml_proc::conversion_context::BaseConversionContext>>) &&
            args[1].type() == typeid(vrml_proc::parser::Vec3f) &&
            args[2].type() == typeid(vrml_proc::parser::Vec3f)) {

            auto children = std::any_cast<std::vector<std::shared_ptr<vrml_proc::conversion_context::BaseConversionContext>>>(args[0]);
            auto bboxCenter = std::any_cast<vrml_proc::parser::Vec3f>(args[1]);
            auto bboxSize = std::any_cast<vrml_proc::parser::Vec3f>(args[2]);

            return std::make_shared<vrml_proc::action::GroupAction>(children, bboxCenter, bboxSize);
        }

        throw std::invalid_argument("Invalid arguments for GroupAction"); });

    return actionMap;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------ //

TEST_CASE("Parse VRML File - Valid Input - Simple VRML File", "[parsing][valid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(validGroup, manager);
    REQUIRE(parseResult);

    vrml_proc::action::BaseConversionContextActionMap actionMap = GetActionMap();

    vrml_proc::traversor::VrmlFileTraversor traversor;
    auto traversorResult = traversor.Traverse({ parseResult.value(), manager}, actionMap);

    std::shared_ptr<vrml_proc::conversion_context::MeshConversionContext> meshContextPtr = std::dynamic_pointer_cast<vrml_proc::conversion_context::MeshConversionContext>(traversorResult);
    REQUIRE(meshContextPtr);

    auto meshContext = meshContextPtr->GetData();
    REQUIRE(meshContext.size() == 1);
}