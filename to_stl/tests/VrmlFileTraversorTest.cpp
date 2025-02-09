#include <catch2/catch_test_macros.hpp>

#include <any>
#include <cassert>
#include <filesystem>
#include <memory>
#include <vector>

#include "test_data/VrmlFileTraversorTestDataset.hpp"
#include <BoxAction.hpp>
#include <BoxCalculator.hpp>
#include <ConversionContextActionMap.hpp>
#include <GroupAction.hpp>
#include <IndexedFaceSetAction.hpp>
#include <Logger.hpp>
#include <MemoryMappedFileReader.hpp>
#include <MeshConversionContext.hpp>
#include <ParserResult.hpp>
#include <ShapeAction.hpp>
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

static void HandleRootLevelError(const cpp::result<std::shared_ptr<vrml_proc::conversion_context::MeshConversionContext>, std::shared_ptr<vrml_proc::core::error::Error>>& result) {
    if (result.has_error()) {
        vrml_proc::core::logger::LogUnformattedText("caught application error", result.error()->GetMessage(), vrml_proc::core::logger::Level::Error, LOGGING_INFO);
    }
}

static void HandleRootLevelError(std::shared_ptr<vrml_proc::core::error::Error> error) {
    vrml_proc::core::logger::LogUnformattedText("caught application error", error->GetMessage(), vrml_proc::core::logger::Level::Error, LOGGING_INFO);
}

static vrml_proc::action::ConversionContextActionMap<vrml_proc::conversion_context::MeshConversionContext>& GetActionMap() {

    static vrml_proc::action::ConversionContextActionMap<vrml_proc::conversion_context::MeshConversionContext> actionMap;

    actionMap.AddAction("Box", [](const vrml_proc::action::ConversionContextActionMap<vrml_proc::conversion_context::MeshConversionContext>::ReferencedArguments& refArgs,
        const vrml_proc::action::ConversionContextActionMap<vrml_proc::conversion_context::MeshConversionContext>::CopiedArguments& copyArgs) {
            if (refArgs.size() == 1 && refArgs[0].get().type() == typeid(std::reference_wrapper<const vrml_proc::parser::Vec3f>) &&
                copyArgs.size() == 1 && copyArgs[0].type() == typeid(bool)) {
                return std::make_shared<to_stl::action::BoxAction>(std::any_cast<std::reference_wrapper<const vrml_proc::parser::Vec3f>>(refArgs[0]), std::any_cast<bool>(copyArgs[0]));
            }
            assert(false && "Invalid arguments for BoxAction");
        });

    actionMap.AddAction("Group", [](const vrml_proc::action::ConversionContextActionMap<vrml_proc::conversion_context::MeshConversionContext>::ReferencedArguments& refArgs,
        const vrml_proc::action::ConversionContextActionMap<vrml_proc::conversion_context::MeshConversionContext>::CopiedArguments& copyArgs) {

            if (refArgs.size() == 2 &&
                refArgs[0].get().type() == typeid(std::reference_wrapper<const vrml_proc::parser::Vec3f>) &&
                refArgs[1].get().type() == typeid(std::reference_wrapper<const vrml_proc::parser::Vec3f>) &&
                copyArgs.size() == 1 &&
                copyArgs[0].type() == typeid(std::vector<std::shared_ptr<vrml_proc::conversion_context::MeshConversionContext>>)) {

                auto children = std::any_cast<std::vector<std::shared_ptr<vrml_proc::conversion_context::MeshConversionContext>>>(copyArgs[0]);
                auto bboxCenter = std::any_cast<std::reference_wrapper<const vrml_proc::parser::Vec3f>>(refArgs[0]);
                auto bboxSize = std::any_cast<std::reference_wrapper<const vrml_proc::parser::Vec3f>>(refArgs[1]);

                return std::make_shared<to_stl::action::GroupAction>(children, bboxCenter, bboxSize);
            }

            assert(false && "Invalid arguments for GroupAction");
        });

    actionMap.AddAction("Shape", [](const vrml_proc::action::ConversionContextActionMap<vrml_proc::conversion_context::MeshConversionContext>::ReferencedArguments& refArgs,
        const vrml_proc::action::ConversionContextActionMap<vrml_proc::conversion_context::MeshConversionContext>::CopiedArguments& copyArgs) {

            if (refArgs.size() == 0 && copyArgs.size() == 2 &&
                copyArgs[0].type() == typeid(std::shared_ptr<vrml_proc::conversion_context::MeshConversionContext>) &&
                copyArgs[1].type() == typeid(std::shared_ptr<vrml_proc::conversion_context::MeshConversionContext>)) {

                auto appearance = std::any_cast<std::shared_ptr<vrml_proc::conversion_context::MeshConversionContext>>(copyArgs[0]);
                auto geometry = std::any_cast<std::shared_ptr<vrml_proc::conversion_context::MeshConversionContext>>(copyArgs[1]);

                return std::make_shared<to_stl::action::ShapeAction>(appearance, geometry);
            }

            assert(false && "Invalid arguments for ShapeAction"); });

    actionMap.AddAction("IndexedFaceSet", [](const vrml_proc::action::ConversionContextActionMap<vrml_proc::conversion_context::MeshConversionContext>::ReferencedArguments& refArgs,
        const vrml_proc::action::ConversionContextActionMap<vrml_proc::conversion_context::MeshConversionContext>::CopiedArguments& copyArgs) {

            using vrml_proc::parser::VrmlNode;
            using vrml_proc::parser::Int32Array;
            using vrml_proc::parser::float32_t;

            if (refArgs.size() == 14 && copyArgs.size() == 1 &&
                copyArgs[0].type() == typeid(bool) &&
                refArgs[0].get().type() == typeid(std::reference_wrapper<const VrmlNode>) &&
                refArgs[1].get().type() == typeid(std::reference_wrapper<const VrmlNode>) &&
                refArgs[2].get().type() == typeid(std::reference_wrapper<const VrmlNode>) &&
                refArgs[3].get().type() == typeid(std::reference_wrapper<const VrmlNode>) &&
                refArgs[4].get().type() == typeid(std::reference_wrapper<const bool>) &&
                refArgs[5].get().type() == typeid(std::reference_wrapper<const Int32Array>) &&
                refArgs[6].get().type() == typeid(std::reference_wrapper<const bool>) &&
                refArgs[7].get().type() == typeid(std::reference_wrapper<const bool>) &&
                refArgs[8].get().type() == typeid(std::reference_wrapper<const Int32Array>) &&
                refArgs[9].get().type() == typeid(std::reference_wrapper<const float32_t>) &&
                refArgs[10].get().type() == typeid(std::reference_wrapper<const Int32Array>) &&
                refArgs[11].get().type() == typeid(std::reference_wrapper<const bool>) &&
                refArgs[12].get().type() == typeid(std::reference_wrapper<const bool>) &&
                refArgs[13].get().type() == typeid(std::reference_wrapper<const Int32Array>)
                ) {
                vrml_proc::traversor::handler::IndexedFaceSetHandler::IndexedFaceSetProperties properties{
                    std::any_cast<std::reference_wrapper<const VrmlNode>>(refArgs[0]),
                    std::any_cast<std::reference_wrapper<const VrmlNode>>(refArgs[1]),
                    std::any_cast<std::reference_wrapper<const VrmlNode>>(refArgs[2]),
                    std::any_cast<std::reference_wrapper<const VrmlNode>>(refArgs[3]),
                    std::any_cast<std::reference_wrapper<const bool>>(refArgs[4]),
                    std::any_cast<std::reference_wrapper<const Int32Array>>(refArgs[5]),
                    std::any_cast<std::reference_wrapper<const bool>>(refArgs[6]),
                    std::any_cast<std::reference_wrapper<const bool>>(refArgs[7]),
                    std::any_cast<std::reference_wrapper<const Int32Array>>(refArgs[8]),
                    std::any_cast<std::reference_wrapper<const float32_t>>(refArgs[9]),
                    std::any_cast<std::reference_wrapper<const Int32Array>>(refArgs[10]),
                    std::any_cast<std::reference_wrapper<const bool>>(refArgs[11]),
                    std::any_cast<std::reference_wrapper<const bool>>(refArgs[12]),
                    std::any_cast<std::reference_wrapper<const Int32Array>>(refArgs[13])
                };

                return std::make_shared<to_stl::action::IndexedFaceSetAction>(
                    properties,
                    std::any_cast<bool>(copyArgs[0])
                );
            }

            assert(false && "Invalid arguments for IndexedFaceSet");
        });


    return actionMap;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------ //

TEST_CASE("Initialization") {

    vrml_proc::core::logger::InitLogging();
}

TEST_CASE("Parse VRML File - Valid Input - Simple VRML File - Box node", "[parsing][valid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(validBoxNode, manager);
    REQUIRE(parseResult);

    vrml_proc::action::ConversionContextActionMap<vrml_proc::conversion_context::MeshConversionContext> actionMap = GetActionMap();

    auto traversorResult = vrml_proc::traversor::VrmlFileTraversor::Traverse<vrml_proc::conversion_context::MeshConversionContext>({ parseResult.value(), manager }, actionMap);
    REQUIRE(traversorResult.has_value());

    auto& meshContext = traversorResult.value()->GetData();
    REQUIRE(meshContext.size() == 0);
}

TEST_CASE("Parse VRML File - Valid Input - Simple VRML File - Box node empty", "[parsing][valid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(validBoxNodeEmpty, manager);
    REQUIRE(parseResult);

    vrml_proc::action::ConversionContextActionMap<vrml_proc::conversion_context::MeshConversionContext> actionMap = GetActionMap();

    auto traversorResult = vrml_proc::traversor::VrmlFileTraversor::Traverse<vrml_proc::conversion_context::MeshConversionContext>({ parseResult.value(), manager }, actionMap);
    REQUIRE(traversorResult.has_value());

    auto& meshContext = traversorResult.value()->GetData();
    REQUIRE(meshContext.size() == 0);
}

TEST_CASE("Parse VRML File - Invalid Input - Simple VRML File - Box node - Wrong Field Type", "[parsing][invalid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(invalidBoxNodeWrongDataType, manager);
    REQUIRE(parseResult);

    vrml_proc::action::ConversionContextActionMap actionMap = GetActionMap();

    auto traversorResult = vrml_proc::traversor::VrmlFileTraversor::Traverse<vrml_proc::conversion_context::MeshConversionContext>({ parseResult.value(), manager }, actionMap);
    REQUIRE(traversorResult.has_error());
    HandleRootLevelError(traversorResult);
}

TEST_CASE("Parse VRML File - Invalid Input - Simple VRML File - Box node - Wrong Field Name", "[parsing][invalid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(invalidBoxNodeWrongFieldName, manager);
    REQUIRE(parseResult);

    vrml_proc::action::ConversionContextActionMap actionMap = GetActionMap();

    auto traversorResult = vrml_proc::traversor::VrmlFileTraversor::Traverse<vrml_proc::conversion_context::MeshConversionContext>({ parseResult.value(), manager }, actionMap);
    REQUIRE(traversorResult.has_error());
    HandleRootLevelError(traversorResult);
}

TEST_CASE("Parse VRML File - Valid Input - Simple VRML File - WorldInfo node", "[parsing][valid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(validWorldInfoNode, manager);
    REQUIRE(parseResult);

    vrml_proc::action::ConversionContextActionMap<vrml_proc::conversion_context::MeshConversionContext> actionMap = GetActionMap();

    auto traversorResult = vrml_proc::traversor::VrmlFileTraversor::Traverse<vrml_proc::conversion_context::MeshConversionContext>({ parseResult.value(), manager }, actionMap);
    REQUIRE(traversorResult.has_value());

    auto& meshContext = traversorResult.value()->GetData();
    REQUIRE(meshContext.size() == 0);
}

TEST_CASE("Parse VRML File - Invalid Input - Simple VRML File - WorldInfo node - Wrong Field Type", "[parsing][invalid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(invalidWorldInfoNodeWrongDataType, manager);
    REQUIRE(parseResult);

    vrml_proc::action::ConversionContextActionMap actionMap = GetActionMap();

    auto traversorResult = vrml_proc::traversor::VrmlFileTraversor::Traverse<vrml_proc::conversion_context::MeshConversionContext>({ parseResult.value(), manager }, actionMap);
    REQUIRE(traversorResult.has_error());
    HandleRootLevelError(traversorResult);
}

TEST_CASE("Parse VRML File - Invalid Input - Simple VRML File - WorldInfo node - Wrong Field Name", "[parsing][invalid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(invalidWorldInfoNodeWrongFieldName, manager);
    REQUIRE(parseResult);

    vrml_proc::action::ConversionContextActionMap actionMap = GetActionMap();

    auto traversorResult = vrml_proc::traversor::VrmlFileTraversor::Traverse<vrml_proc::conversion_context::MeshConversionContext>({ parseResult.value(), manager }, actionMap);
    REQUIRE(traversorResult.has_error());
    HandleRootLevelError(traversorResult);
}

TEST_CASE("Parse VRML File - Invalid Input - Simple VRML File - Group node - Unknown node", "[parsing][invalid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(invalidGroupUnknownNode, manager);
    REQUIRE(parseResult);

    vrml_proc::action::ConversionContextActionMap<vrml_proc::conversion_context::MeshConversionContext> actionMap = GetActionMap();

    auto traversorResult = vrml_proc::traversor::VrmlFileTraversor::Traverse<vrml_proc::conversion_context::MeshConversionContext>({ parseResult.value(), manager }, actionMap);
    REQUIRE(traversorResult.has_error());
    HandleRootLevelError(traversorResult);;
}

TEST_CASE("Parse VRML File - Valid Input - Simple VRML File - Group node", "[parsing][valid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(validGroup, manager);
    REQUIRE(parseResult);

    vrml_proc::action::ConversionContextActionMap<vrml_proc::conversion_context::MeshConversionContext> actionMap = GetActionMap();

    auto traversorResult = vrml_proc::traversor::VrmlFileTraversor::Traverse<vrml_proc::conversion_context::MeshConversionContext>({ parseResult.value(), manager }, actionMap);
    REQUIRE(traversorResult.has_value());

    auto& meshContext = traversorResult.value()->GetData();
    REQUIRE(meshContext.size() == 0);
}

TEST_CASE("Parse VRML File - Valid Input - Simple VRML File - Shape node", "[parsing][valid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(validShape, manager);
    REQUIRE(parseResult);

    vrml_proc::action::ConversionContextActionMap<vrml_proc::conversion_context::MeshConversionContext> actionMap = GetActionMap();

    auto traversorResult = vrml_proc::traversor::VrmlFileTraversor::Traverse<vrml_proc::conversion_context::MeshConversionContext>({ parseResult.value(), manager }, actionMap);
    REQUIRE(traversorResult.has_value());

    auto& meshContext = traversorResult.value()->GetData();
    REQUIRE(meshContext.size() == 1);
}

TEST_CASE("Parse VRML File - Valid Input - Simple VRML File - Shape node with Group merging", "[parsing][valid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(validShapeMerge, manager);
    REQUIRE(parseResult);

    vrml_proc::action::ConversionContextActionMap<vrml_proc::conversion_context::MeshConversionContext> actionMap = GetActionMap();

    auto traversorResult = vrml_proc::traversor::VrmlFileTraversor::Traverse<vrml_proc::conversion_context::MeshConversionContext>({ parseResult.value(), manager }, actionMap);
    REQUIRE(traversorResult.has_value());

    auto& meshContext = traversorResult.value()->GetData();
    REQUIRE(meshContext.size() == 3);
}

TEST_CASE("Parse VRML File - Valid Input - Simple VRML File - Shape empty node", "[parsing][valid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(validShapeEmpty, manager);
    REQUIRE(parseResult);

    vrml_proc::action::ConversionContextActionMap<vrml_proc::conversion_context::MeshConversionContext> actionMap = GetActionMap();

    auto traversorResult = vrml_proc::traversor::VrmlFileTraversor::Traverse<vrml_proc::conversion_context::MeshConversionContext>({ parseResult.value(), manager }, actionMap);
    REQUIRE(traversorResult.has_value());

    auto& meshContext = traversorResult.value()->GetData();
    REQUIRE(meshContext.size() == 0);
}

TEST_CASE("Parse VRML File - Invalid Input - Simple VRML File - Shape node", "[parsing][invalid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(invalidShapeWrongNodeForGeometryField, manager);
    REQUIRE(parseResult);

    vrml_proc::action::ConversionContextActionMap<vrml_proc::conversion_context::MeshConversionContext> actionMap = GetActionMap();

    auto traversorResult = vrml_proc::traversor::VrmlFileTraversor::Traverse<vrml_proc::conversion_context::MeshConversionContext>({ parseResult.value(), manager }, actionMap);
    REQUIRE(traversorResult.has_error());
    HandleRootLevelError(traversorResult);;
}

TEST_CASE("Parse VRML File - Valid Input - Simple VRML File - IndexedShapeSet node I.", "[parsing][valid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(validIndexedFaceSetNode, manager);
    REQUIRE(parseResult);
    vrml_proc::action::ConversionContextActionMap<vrml_proc::conversion_context::MeshConversionContext> actionMap = GetActionMap();

    auto traversorResult = vrml_proc::traversor::VrmlFileTraversor::Traverse<vrml_proc::conversion_context::MeshConversionContext>({ parseResult.value(), manager }, actionMap);
    REQUIRE(traversorResult.has_value());
    auto& meshContext = traversorResult.value()->GetData();
    REQUIRE(meshContext.size() == 1);
}

TEST_CASE("Parse VRML File - Valid Input - Simple VRML File - IndexedShapeSet node II.", "[parsing][valid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(validIndexedFaceSetNodeNotInShape, manager);
    REQUIRE(parseResult);
    vrml_proc::action::ConversionContextActionMap<vrml_proc::conversion_context::MeshConversionContext> actionMap = GetActionMap();

    auto traversorResult = vrml_proc::traversor::VrmlFileTraversor::Traverse<vrml_proc::conversion_context::MeshConversionContext>({ parseResult.value(), manager }, actionMap);
    REQUIRE(traversorResult.has_value());
    auto& meshContext = traversorResult.value()->GetData();
    REQUIRE(meshContext.size() == 0);
}

TEST_CASE("Parse VRML File - Invalid Input - Simple VRML File - IndexedShapeSet node I.", "[parsing][invalid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(invalidIndexedFaceSetNodeWrongFieldNodeHeader, manager);
    REQUIRE(parseResult);

    vrml_proc::action::ConversionContextActionMap<vrml_proc::conversion_context::MeshConversionContext> actionMap = GetActionMap();

    auto traversorResult = vrml_proc::traversor::VrmlFileTraversor::Traverse<vrml_proc::conversion_context::MeshConversionContext>({ parseResult.value(), manager }, actionMap);
    REQUIRE(traversorResult.has_error());
    HandleRootLevelError(traversorResult);
}

TEST_CASE("Parse VRML File - Valid Input - Simple VRML File - Transformation node I.", "[parsing][valid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(validTransformatioNode, manager);
    REQUIRE(parseResult);

    vrml_proc::action::ConversionContextActionMap<vrml_proc::conversion_context::MeshConversionContext> actionMap = GetActionMap();

    auto traversorResult = vrml_proc::traversor::VrmlFileTraversor::Traverse<vrml_proc::conversion_context::MeshConversionContext>({ parseResult.value(), manager }, actionMap);
    REQUIRE(traversorResult.has_value());
}

TEST_CASE("Parse VRML File - Valid Input - Simple VRML File - Transformation node II.", "[parsing][valid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(validTransformatioNodeEmpty, manager);
    REQUIRE(parseResult);

    vrml_proc::action::ConversionContextActionMap<vrml_proc::conversion_context::MeshConversionContext> actionMap = GetActionMap();

    auto traversorResult = vrml_proc::traversor::VrmlFileTraversor::Traverse<vrml_proc::conversion_context::MeshConversionContext>({ parseResult.value(), manager }, actionMap);
    REQUIRE(traversorResult.has_value());
}

TEST_CASE("Parse VRML File - Invalid Input - Simple VRML File - Transformation node I.", "[parsing][invalid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(invalidTransformatioNodeWrongFieldNameCenter, manager);
    REQUIRE(parseResult);

    vrml_proc::action::ConversionContextActionMap<vrml_proc::conversion_context::MeshConversionContext> actionMap = GetActionMap();

    auto traversorResult = vrml_proc::traversor::VrmlFileTraversor::Traverse<vrml_proc::conversion_context::MeshConversionContext>({ parseResult.value(), manager }, actionMap);
    REQUIRE(traversorResult.has_error());
    HandleRootLevelError(traversorResult);
}

TEST_CASE("Parse VRML File - Invalid Input - Simple VRML File - Transformation node II.", "[parsing][invalid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(invalidTransformatioNodeWrongFieldNameChildren, manager);
    REQUIRE(parseResult);

    vrml_proc::action::ConversionContextActionMap<vrml_proc::conversion_context::MeshConversionContext> actionMap = GetActionMap();

    auto traversorResult = vrml_proc::traversor::VrmlFileTraversor::Traverse<vrml_proc::conversion_context::MeshConversionContext>({ parseResult.value(), manager }, actionMap);
    REQUIRE(traversorResult.has_error());
    HandleRootLevelError(traversorResult);
}

TEST_CASE("Parse VRML File - Invalid Input - Simple VRML File - Transformation node III.", "[parsing][invalid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(invalidTransformatioNodeWrongFieldTypeChildren, manager);
    REQUIRE(parseResult);

    vrml_proc::action::ConversionContextActionMap<vrml_proc::conversion_context::MeshConversionContext> actionMap = GetActionMap();

    auto traversorResult = vrml_proc::traversor::VrmlFileTraversor::Traverse<vrml_proc::conversion_context::MeshConversionContext>({ parseResult.value(), manager }, actionMap);
    REQUIRE(traversorResult.has_error());
    HandleRootLevelError(traversorResult);
}

TEST_CASE("Parse VRML File - Invalid Input - Simple VRML File - Transformation node IV.", "[parsing][invalid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(invalidTransformatioNodeWrongFieldTypeScaleOrientation, manager);
    REQUIRE(parseResult);

    vrml_proc::action::ConversionContextActionMap<vrml_proc::conversion_context::MeshConversionContext> actionMap = GetActionMap();

    auto traversorResult = vrml_proc::traversor::VrmlFileTraversor::Traverse<vrml_proc::conversion_context::MeshConversionContext>({ parseResult.value(), manager }, actionMap);
    REQUIRE(traversorResult.has_error());
    HandleRootLevelError(traversorResult);
}

//TEST_CASE("Parse VRMLFile From File - Valid Input - Tubulin", "[parsing][valid][fromfile]") {
//
//    vrml_proc::parser::VrmlNodeManager manager;
//    auto parseResult = ParseVrmlFile(std::filesystem::path(R"(C:\Users\marek\Documents\FI_MUNI\sem_05\SBAPR\Datasets\classic - more data.wrl)"), manager);
//    REQUIRE(parseResult);
//
//    vrml_proc::action::ConversionContextActionMap<vrml_proc::conversion_context::MeshConversionContext> actionMap = GetActionMap();
//
//    auto traversorResult = vrml_proc::traversor::VrmlFileTraversor::Traverse<vrml_proc::conversion_context::MeshConversionContext>({ parseResult.value(), manager }, actionMap);
//    REQUIRE(traversorResult.has_value());
//    auto& meshContext = traversorResult.value()->GetData();
//    REQUIRE(meshContext.size() == 1);
//}