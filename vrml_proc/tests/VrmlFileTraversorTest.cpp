#include <catch2/catch_test_macros.hpp>

#include <any>
#include <memory>
#include <vector>

#include "test_data/VrmlFileTraversorTestDataset.hpp"
#include <BaseConversionContextActionMap.hpp>
#include <GroupAction.hpp>
#include <MemoryMappedFileReader.hpp>
#include <MeshConversionContext.hpp>
#include <ParserResult.hpp>
#include <SpotlightAction.hpp>
#include <Vec3f.hpp>
#include <VrmlFile.hpp>
#include <VrmlFileTraversor.hpp>
#include <VrmlNodeManager.hpp>
#include <VrmlParser.hpp>

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