#include <catch2/catch_test_macros.hpp>

#include <any>
#include <filesystem>
#include <memory>
#include <vector>

#include "test_data/VrmlFileTraversorTestDataset.hpp"
#include <ConversionContextActionMap.hpp>
#include <BoxAction.hpp>
#include <GroupAction.hpp>
#include <MemoryMappedFileReader.hpp>
#include <MeshConversionContext.hpp>
#include <ParserResult.hpp>
#include <ShapeAction.hpp>
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

static vrml_proc::action::ConversionContextActionMap<vrml_proc::conversion_context::MeshConversionContext>& GetActionMap() {

    static vrml_proc::action::ConversionContextActionMap<vrml_proc::conversion_context::MeshConversionContext> actionMap;

    /*actionMap.AddAction("Spotlight", [](const std::vector<std::any>& args) {
        if (args.size() == 1 && args[0].type() == typeid(float)) {
            return std::make_shared<vrml_proc::action::SpotlightAction>(std::any_cast<float>(args[0]));
        }
        throw std::invalid_argument("Invalid arguments for SpotlightAction");
        });*/

    actionMap.AddAction("Box", [](const std::vector<std::any>& args) {
        if (args.size() == 2 && args[0].type() == typeid(vrml_proc::parser::Vec3f) && args[1].type() == typeid(bool)) {
            return std::make_shared<vrml_proc::action::BoxAction>(std::any_cast<vrml_proc::parser::Vec3f>(args[0]), std::any_cast<bool>(args[1]));
        }
        throw std::invalid_argument("Invalid arguments for BoxAction");
        });

   /* actionMap.AddAction("Group", [](const std::vector<std::any>& args) {

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

    actionMap.AddAction("Shape", [](const std::vector<std::any>& args) {

        if (args.size() == 2 &&
            args[0].type() == typeid(std::shared_ptr<vrml_proc::conversion_context::BaseConversionContext>) &&
            args[1].type() == typeid(std::shared_ptr<vrml_proc::conversion_context::BaseConversionContext>)) {

            auto appearance = std::any_cast<std::shared_ptr<vrml_proc::conversion_context::BaseConversionContext>>(args[0]);
            auto geometry = std::any_cast<std::shared_ptr<vrml_proc::conversion_context::BaseConversionContext>>(args[1]);

            return std::make_shared<vrml_proc::action::ShapeAction>(appearance, geometry);
        }

        throw std::invalid_argument("Invalid arguments for ShapeAction"); });
        */
    return actionMap;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------ //

TEST_CASE("Parse VRML File - Valid Input - Simple VRML File - Box node", "[parsing][valid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(validBoxNode, manager);
    REQUIRE(parseResult);

    vrml_proc::action::ConversionContextActionMap<vrml_proc::conversion_context::MeshConversionContext> actionMap = GetActionMap();

    auto traversorResult = vrml_proc::traversor::VrmlFileTraversor::Traverse<vrml_proc::conversion_context::MeshConversionContext>({ parseResult.value(), manager}, actionMap);
    REQUIRE(traversorResult.has_value());

    auto& meshContext = traversorResult.value()->GetData();
    REQUIRE(meshContext.size() == 0);
}

TEST_CASE("Parse VRML File - Invalid Input - Simple VRML File - Box node - Wrong Field Type", "[parsing][invalid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(invalidBoxNodeWrondDataType, manager);
    REQUIRE(parseResult);

    vrml_proc::action::ConversionContextActionMap actionMap = GetActionMap();

    auto traversorResult = vrml_proc::traversor::VrmlFileTraversor::Traverse<vrml_proc::conversion_context::MeshConversionContext>({ parseResult.value(), manager }, actionMap);
    REQUIRE(traversorResult.has_error());
    std::cout << traversorResult.error()->GetMessage() << std::endl;
}

TEST_CASE("Parse VRML File - Invalid Input - Simple VRML File - Box node - Wrong Field Name", "[parsing][invalid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(invalidBoxNodeWrongFieldName, manager);
    REQUIRE(parseResult);

    vrml_proc::action::ConversionContextActionMap actionMap = GetActionMap();

    auto traversorResult = vrml_proc::traversor::VrmlFileTraversor::Traverse<vrml_proc::conversion_context::MeshConversionContext>({ parseResult.value(), manager }, actionMap);
    REQUIRE(traversorResult.has_error());
    std::cout << traversorResult.error()->GetMessage() << std::endl;
}