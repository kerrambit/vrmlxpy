#include <catch2/catch_test_macros.hpp>
#include <catch2/interfaces/catch_interfaces_capture.hpp>

#include <algorithm>
#include <filesystem>
#include <future>
#include <memory>
#include <string>
#include <vector>

#include "test_data/VrmlFileTraversorTestDataset.hpp"
#include <BoxAction.hpp>
#include <BufferView.hpp>
#include <CalculatorResult.hpp>
#include <ConversionContextActionMap.hpp>
#include <Logger.hpp>
#include <MemoryMappedFileReader.hpp>
#include <Mesh.hpp>
#include <MeshTaskConversionContext.hpp>
#include <ParserResult.hpp>
#include <StlActionMap.hpp>
#include <StlFileWriter.hpp>
#include <VrmlFile.hpp>
#include <VrmlFileTraversor.hpp>
#include <VrmlNodeManager.hpp>
#include <VrmlParser.hpp>

#define BASE_OUTPUT_PATH R"(C:\Users\marek\Documents\FI_MUNI\sem_05\SBAPR\vrmlxpy\)"

#define GENERATE_TEST_OUTPUT_PATH(filepath) \
    std::string testName = Catch::getResultCapture().getCurrentTestName(); \
    std::replace(testName.begin(), testName.end(), ' ', '_'); \
    std::filesystem::path filepath = std::filesystem::path(BASE_OUTPUT_PATH) / (testName + ".stl")

static vrml_proc::parser::ParserResult<vrml_proc::parser::VrmlFile> ParseVrmlFile(std::string& text, vrml_proc::parser::VrmlNodeManager& manager) {

    vrml_proc::parser::VrmlParser parser(manager);
    return parser.Parse(vrml_proc::parser::BufferView(text.c_str(), text.c_str() + text.size()));
}

static vrml_proc::parser::ParserResult<vrml_proc::parser::VrmlFile> ParseVrmlFile(const std::filesystem::path& filepath, vrml_proc::parser::VrmlNodeManager& manager) {

    vrml_proc::core::io::MemoryMappedFileReader reader;
    auto readResult = reader.Read(filepath);
    if (!readResult.has_value()) {
        return {};
    }

    vrml_proc::parser::VrmlParser parser(manager);
    vrml_proc::core::io::MemoryMappedFile file = readResult.value();
    return parser.Parse(vrml_proc::parser::BufferView(file.GetBegin(), file.GetEnd()));
}

template <typename T>
static bool CheckInnermostError(std::shared_ptr<vrml_proc::core::error::Error> error) {
    return std::dynamic_pointer_cast<T>(vrml_proc::core::error::Error::GetInnermostError(error)) != nullptr;
}

template <typename T>
static std::shared_ptr<T> GetInnermostError(std::shared_ptr<vrml_proc::core::error::Error> error) {
    return std::dynamic_pointer_cast<T>(vrml_proc::core::error::Error::GetInnermostError(error));
}

static void HandleRootLevelError(const cpp::result<std::shared_ptr<to_stl::conversion_context::MeshTaskConversionContext>, std::shared_ptr<vrml_proc::core::error::Error>>& result) {
    if (result.has_error()) {
        vrml_proc::core::logger::LogUnformattedText("caught application error", result.error()->GetMessage(), vrml_proc::core::logger::Level::Error, LOGGING_INFO);
    }
}

static void HandleRootLevelError(std::shared_ptr<vrml_proc::core::error::Error> error) {
    vrml_proc::core::logger::LogUnformattedText("caught application error", error->GetMessage(), vrml_proc::core::logger::Level::Error, LOGGING_INFO);
}

static void ExportToStl(const std::vector<to_stl::core::MeshTask>& meshContext, const std::filesystem::path& filepath) {

    std::vector<std::future<to_stl::calculator::CalculatorResult>> results;

    for (const auto& task : meshContext) {
        if (task) {
            results.emplace_back(std::async(std::launch::async, task));
        }
    }

    to_stl::core::Mesh mesh;
    for (auto& future : results) {
        auto meshResult = future.get();
        if (meshResult.has_value()) {
            mesh.join(*(meshResult.value()));
        }
        else {
            std::cout << "Error in mesh!" << std::endl;
        }
    };

    to_stl::core::io::StlFileWriter writer;
    writer.Write(filepath, mesh);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------ //

TEST_CASE("Initialization") {

    vrml_proc::core::logger::InitLogging();
}

TEST_CASE("Parse VRML File - Valid Input - Simple VRML File - Box node", "[parsing][valid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(validBoxNode, manager);
    REQUIRE(parseResult);

    vrml_proc::action::ConversionContextActionMap<to_stl::conversion_context::MeshTaskConversionContext> actionMap = to_stl::conversion_context::CreateActionMap();

    auto traversorResult = vrml_proc::traversor::VrmlFileTraversor::Traverse<to_stl::conversion_context::MeshTaskConversionContext>({ parseResult.value(), manager }, actionMap);
    REQUIRE(traversorResult.has_value());

    auto& meshContext = traversorResult.value()->GetData();
    REQUIRE(meshContext.size() == 0);
}

TEST_CASE("Parse VRML File - Valid Input - Simple VRML File - Box node empty", "[parsing][valid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(validBoxNodeEmpty, manager);
    REQUIRE(parseResult);

    vrml_proc::action::ConversionContextActionMap<to_stl::conversion_context::MeshTaskConversionContext> actionMap = to_stl::conversion_context::CreateActionMap();

    auto traversorResult = vrml_proc::traversor::VrmlFileTraversor::Traverse<to_stl::conversion_context::MeshTaskConversionContext>({ parseResult.value(), manager }, actionMap);
    REQUIRE(traversorResult.has_value());

    auto& meshContext = traversorResult.value()->GetData();
    REQUIRE(meshContext.size() == 0);
}

TEST_CASE("Parse VRML File - Invalid Input - Simple VRML File - Box node - Wrong Field Type", "[parsing][invalid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(invalidBoxNodeWrongDataType, manager);
    REQUIRE(parseResult);

    vrml_proc::action::ConversionContextActionMap actionMap = to_stl::conversion_context::CreateActionMap();

    auto traversorResult = vrml_proc::traversor::VrmlFileTraversor::Traverse<to_stl::conversion_context::MeshTaskConversionContext>({ parseResult.value(), manager }, actionMap);
    REQUIRE(traversorResult.has_error());
    HandleRootLevelError(traversorResult);
}

TEST_CASE("Parse VRML File - Invalid Input - Simple VRML File - Box node - Wrong Field Name", "[parsing][invalid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(invalidBoxNodeWrongFieldName, manager);
    REQUIRE(parseResult);

    vrml_proc::action::ConversionContextActionMap actionMap = to_stl::conversion_context::CreateActionMap();

    auto traversorResult = vrml_proc::traversor::VrmlFileTraversor::Traverse<to_stl::conversion_context::MeshTaskConversionContext>({ parseResult.value(), manager }, actionMap);
    REQUIRE(traversorResult.has_error());
    HandleRootLevelError(traversorResult);
}

TEST_CASE("Parse VRML File - Valid Input - Simple VRML File - WorldInfo node", "[parsing][valid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(validWorldInfoNode, manager);
    REQUIRE(parseResult);

    vrml_proc::action::ConversionContextActionMap<to_stl::conversion_context::MeshTaskConversionContext> actionMap = to_stl::conversion_context::CreateActionMap();

    auto traversorResult = vrml_proc::traversor::VrmlFileTraversor::Traverse<to_stl::conversion_context::MeshTaskConversionContext>({ parseResult.value(), manager }, actionMap);
    REQUIRE(traversorResult.has_value());

    auto& meshContext = traversorResult.value()->GetData();
    REQUIRE(meshContext.size() == 0);
}

TEST_CASE("Parse VRML File - Invalid Input - Simple VRML File - WorldInfo node - Wrong Field Type", "[parsing][invalid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(invalidWorldInfoNodeWrongDataType, manager);
    REQUIRE(parseResult);

    vrml_proc::action::ConversionContextActionMap actionMap = to_stl::conversion_context::CreateActionMap();

    auto traversorResult = vrml_proc::traversor::VrmlFileTraversor::Traverse<to_stl::conversion_context::MeshTaskConversionContext>({ parseResult.value(), manager }, actionMap);
    REQUIRE(traversorResult.has_error());
    HandleRootLevelError(traversorResult);
}

TEST_CASE("Parse VRML File - Invalid Input - Simple VRML File - WorldInfo node - Wrong Field Name", "[parsing][invalid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(invalidWorldInfoNodeWrongFieldName, manager);
    REQUIRE(parseResult);

    vrml_proc::action::ConversionContextActionMap actionMap = to_stl::conversion_context::CreateActionMap();

    auto traversorResult = vrml_proc::traversor::VrmlFileTraversor::Traverse<to_stl::conversion_context::MeshTaskConversionContext>({ parseResult.value(), manager }, actionMap);
    REQUIRE(traversorResult.has_error());
    HandleRootLevelError(traversorResult);
}

TEST_CASE("Parse VRML File - Invalid Input - Simple VRML File - Group node - Unknown node", "[parsing][invalid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(invalidGroupUnknownNode, manager);
    REQUIRE(parseResult);

    vrml_proc::action::ConversionContextActionMap<to_stl::conversion_context::MeshTaskConversionContext> actionMap = to_stl::conversion_context::CreateActionMap();

    auto traversorResult = vrml_proc::traversor::VrmlFileTraversor::Traverse<to_stl::conversion_context::MeshTaskConversionContext>({ parseResult.value(), manager }, actionMap);
    REQUIRE(traversorResult.has_error());
    HandleRootLevelError(traversorResult);;
}

TEST_CASE("Parse VRML File - Valid Input - Simple VRML File - Group node", "[parsing][valid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(validGroup, manager);
    REQUIRE(parseResult);

    vrml_proc::action::ConversionContextActionMap<to_stl::conversion_context::MeshTaskConversionContext> actionMap = to_stl::conversion_context::CreateActionMap();

    auto traversorResult = vrml_proc::traversor::VrmlFileTraversor::Traverse<to_stl::conversion_context::MeshTaskConversionContext>({ parseResult.value(), manager }, actionMap);
    REQUIRE(traversorResult.has_value());

    auto& meshContext = traversorResult.value()->GetData();
    REQUIRE(meshContext.size() == 0);
}

TEST_CASE("Parse VRML File - Valid Input - Simple VRML File - Shape node", "[parsing][valid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(validShape, manager);
    REQUIRE(parseResult);

    vrml_proc::action::ConversionContextActionMap<to_stl::conversion_context::MeshTaskConversionContext> actionMap = to_stl::conversion_context::CreateActionMap();

    auto traversorResult = vrml_proc::traversor::VrmlFileTraversor::Traverse<to_stl::conversion_context::MeshTaskConversionContext>({ parseResult.value(), manager }, actionMap);
    REQUIRE(traversorResult.has_value());

    auto& meshContext = traversorResult.value()->GetData();
    REQUIRE(meshContext.size() == 1);
}

TEST_CASE("Parse VRML File - Valid Input - Simple VRML File - Shape node with Group merging", "[parsing][valid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(validShapeMerge, manager);
    REQUIRE(parseResult);

    vrml_proc::action::ConversionContextActionMap<to_stl::conversion_context::MeshTaskConversionContext> actionMap = to_stl::conversion_context::CreateActionMap();

    auto traversorResult = vrml_proc::traversor::VrmlFileTraversor::Traverse<to_stl::conversion_context::MeshTaskConversionContext>({ parseResult.value(), manager }, actionMap);
    REQUIRE(traversorResult.has_value());

    auto& meshContext = traversorResult.value()->GetData();
    REQUIRE(meshContext.size() == 3);
}

TEST_CASE("Parse VRML File - Valid Input - Simple VRML File - Shape empty node", "[parsing][valid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(validShapeEmpty, manager);
    REQUIRE(parseResult);

    vrml_proc::action::ConversionContextActionMap<to_stl::conversion_context::MeshTaskConversionContext> actionMap = to_stl::conversion_context::CreateActionMap();

    auto traversorResult = vrml_proc::traversor::VrmlFileTraversor::Traverse<to_stl::conversion_context::MeshTaskConversionContext>({ parseResult.value(), manager }, actionMap);
    REQUIRE(traversorResult.has_value());

    auto& meshContext = traversorResult.value()->GetData();
    REQUIRE(meshContext.size() == 0);
}

TEST_CASE("Parse VRML File - Invalid Input - Simple VRML File - Shape node", "[parsing][invalid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(invalidShapeWrongNodeForGeometryField, manager);
    REQUIRE(parseResult);

    vrml_proc::action::ConversionContextActionMap<to_stl::conversion_context::MeshTaskConversionContext> actionMap = to_stl::conversion_context::CreateActionMap();

    auto traversorResult = vrml_proc::traversor::VrmlFileTraversor::Traverse<to_stl::conversion_context::MeshTaskConversionContext>({ parseResult.value(), manager }, actionMap);
    REQUIRE(traversorResult.has_error());
    HandleRootLevelError(traversorResult);;
}

TEST_CASE("Parse VRML File - Valid Input - Simple VRML File - IndexedFaceSet node I.", "[parsing][valid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(validIndexedFaceSetNode, manager);
    REQUIRE(parseResult);
    vrml_proc::action::ConversionContextActionMap<to_stl::conversion_context::MeshTaskConversionContext> actionMap = to_stl::conversion_context::CreateActionMap();

    auto traversorResult = vrml_proc::traversor::VrmlFileTraversor::Traverse<to_stl::conversion_context::MeshTaskConversionContext>({ parseResult.value(), manager }, actionMap);
    REQUIRE(traversorResult.has_value());
    auto& meshContext = traversorResult.value()->GetData();
    REQUIRE(meshContext.size() == 1);
}

TEST_CASE("Parse VRML File - Valid Input - Simple VRML File - IndexedFaceSet node II.", "[parsing][valid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(validIndexedFaceSetNodeNotInShape, manager);
    REQUIRE(parseResult);
    vrml_proc::action::ConversionContextActionMap<to_stl::conversion_context::MeshTaskConversionContext> actionMap = to_stl::conversion_context::CreateActionMap();

    auto traversorResult = vrml_proc::traversor::VrmlFileTraversor::Traverse<to_stl::conversion_context::MeshTaskConversionContext>({ parseResult.value(), manager }, actionMap);
    REQUIRE(traversorResult.has_value());
    auto& meshContext = traversorResult.value()->GetData();
    REQUIRE(meshContext.size() == 0);
}

TEST_CASE("Parse VRML File - Valid Input - Simple VRML File - IndexedFaceSet node III.", "[parsing][valid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(validIndexedFaceSetPyramid, manager);
    REQUIRE(parseResult);
    vrml_proc::action::ConversionContextActionMap<to_stl::conversion_context::MeshTaskConversionContext> actionMap = to_stl::conversion_context::CreateActionMap();

    auto traversorResult = vrml_proc::traversor::VrmlFileTraversor::Traverse<to_stl::conversion_context::MeshTaskConversionContext>({ parseResult.value(), manager }, actionMap);
    REQUIRE(traversorResult.has_value());
    auto& meshContext = traversorResult.value()->GetData();
    REQUIRE(meshContext.size() == 1);
}

TEST_CASE("Parse VRML File - Invalid Input - Simple VRML File - IndexedFaceSet node I.", "[parsing][invalid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(invalidIndexedFaceSetNodeWrongFieldNodeHeader, manager);
    REQUIRE(parseResult);

    vrml_proc::action::ConversionContextActionMap<to_stl::conversion_context::MeshTaskConversionContext> actionMap = to_stl::conversion_context::CreateActionMap();

    auto traversorResult = vrml_proc::traversor::VrmlFileTraversor::Traverse<to_stl::conversion_context::MeshTaskConversionContext>({ parseResult.value(), manager }, actionMap);
    REQUIRE(traversorResult.has_error());
    HandleRootLevelError(traversorResult);
}

TEST_CASE("Parse VRML File - Valid Input - Simple VRML File - Transformation node I.", "[parsing][valid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(validTransformatioNode, manager);
    REQUIRE(parseResult);

    vrml_proc::action::ConversionContextActionMap<to_stl::conversion_context::MeshTaskConversionContext> actionMap = to_stl::conversion_context::CreateActionMap();

    auto traversorResult = vrml_proc::traversor::VrmlFileTraversor::Traverse<to_stl::conversion_context::MeshTaskConversionContext>({ parseResult.value(), manager }, actionMap);
    REQUIRE(traversorResult.has_value());
}

TEST_CASE("Parse VRML File - Valid Input - Simple VRML File - Transformation node II.", "[parsing][valid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(validTransformatioNodeEmpty, manager);
    REQUIRE(parseResult);

    vrml_proc::action::ConversionContextActionMap<to_stl::conversion_context::MeshTaskConversionContext> actionMap = to_stl::conversion_context::CreateActionMap();

    auto traversorResult = vrml_proc::traversor::VrmlFileTraversor::Traverse<to_stl::conversion_context::MeshTaskConversionContext>({ parseResult.value(), manager }, actionMap);
    REQUIRE(traversorResult.has_value());
}

TEST_CASE("Parse VRML File - Invalid Input - Simple VRML File - Transformation node I.", "[parsing][invalid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(invalidTransformatioNodeWrongFieldNameCenter, manager);
    REQUIRE(parseResult);

    vrml_proc::action::ConversionContextActionMap<to_stl::conversion_context::MeshTaskConversionContext> actionMap = to_stl::conversion_context::CreateActionMap();

    auto traversorResult = vrml_proc::traversor::VrmlFileTraversor::Traverse<to_stl::conversion_context::MeshTaskConversionContext>({ parseResult.value(), manager }, actionMap);
    REQUIRE(traversorResult.has_error());
    HandleRootLevelError(traversorResult);
}

TEST_CASE("Parse VRML File - Invalid Input - Simple VRML File - Transformation node II.", "[parsing][invalid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(invalidTransformatioNodeWrongFieldNameChildren, manager);
    REQUIRE(parseResult);

    vrml_proc::action::ConversionContextActionMap<to_stl::conversion_context::MeshTaskConversionContext> actionMap = to_stl::conversion_context::CreateActionMap();

    auto traversorResult = vrml_proc::traversor::VrmlFileTraversor::Traverse<to_stl::conversion_context::MeshTaskConversionContext>({ parseResult.value(), manager }, actionMap);
    REQUIRE(traversorResult.has_error());
    HandleRootLevelError(traversorResult);
}

TEST_CASE("Parse VRML File - Invalid Input - Simple VRML File - Transformation node III.", "[parsing][invalid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(invalidTransformatioNodeWrongFieldTypeChildren, manager);
    REQUIRE(parseResult);

    vrml_proc::action::ConversionContextActionMap<to_stl::conversion_context::MeshTaskConversionContext> actionMap = to_stl::conversion_context::CreateActionMap();

    auto traversorResult = vrml_proc::traversor::VrmlFileTraversor::Traverse<to_stl::conversion_context::MeshTaskConversionContext>({ parseResult.value(), manager }, actionMap);
    REQUIRE(traversorResult.has_error());
    HandleRootLevelError(traversorResult);
}

TEST_CASE("Parse VRML File - Invalid Input - Simple VRML File - Transformation node IV.", "[parsing][invalid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(invalidTransformatioNodeWrongFieldTypeScaleOrientation, manager);
    REQUIRE(parseResult);

    vrml_proc::action::ConversionContextActionMap<to_stl::conversion_context::MeshTaskConversionContext> actionMap = to_stl::conversion_context::CreateActionMap();

    auto traversorResult = vrml_proc::traversor::VrmlFileTraversor::Traverse<to_stl::conversion_context::MeshTaskConversionContext>({ parseResult.value(), manager }, actionMap);
    REQUIRE(traversorResult.has_error());
    HandleRootLevelError(traversorResult);
}


TEST_CASE("Parse VRML File - Valid Input - Simple VRML File - Transformation node III.", "[parsing][valid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(validTransformatioNodeNested, manager);
    REQUIRE(parseResult);

    vrml_proc::action::ConversionContextActionMap<to_stl::conversion_context::MeshTaskConversionContext> actionMap = to_stl::conversion_context::CreateActionMap();

    auto traversorResult = vrml_proc::traversor::VrmlFileTraversor::Traverse<to_stl::conversion_context::MeshTaskConversionContext>({ parseResult.value(), manager }, actionMap);
    REQUIRE(traversorResult.has_value());
}

TEST_CASE("Parse VRML File - Valid Input - Simple VRML File - Switch node I.", "[parsing][valid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(validSwitchNodeWithOneChoiceEmpty, manager);
    REQUIRE(parseResult);

    vrml_proc::action::ConversionContextActionMap<to_stl::conversion_context::MeshTaskConversionContext> actionMap = to_stl::conversion_context::CreateActionMap();

    auto traversorResult = vrml_proc::traversor::VrmlFileTraversor::Traverse<to_stl::conversion_context::MeshTaskConversionContext>({ parseResult.value(), manager }, actionMap);
    REQUIRE(traversorResult.has_value());
    auto& meshContext = traversorResult.value()->GetData();
    REQUIRE(meshContext.size() == 0);
}

TEST_CASE("Parse VRML File - Valid Input - Simple VRML File - Switch node II.", "[parsing][valid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(validSwitchNodeWithMulipleChoicesEmpty, manager);
    REQUIRE(parseResult);

    vrml_proc::action::ConversionContextActionMap<to_stl::conversion_context::MeshTaskConversionContext> actionMap = to_stl::conversion_context::CreateActionMap();

    auto traversorResult = vrml_proc::traversor::VrmlFileTraversor::Traverse<to_stl::conversion_context::MeshTaskConversionContext>({ parseResult.value(), manager }, actionMap);
    REQUIRE(traversorResult.has_value());
    auto& meshContext = traversorResult.value()->GetData();
    REQUIRE(meshContext.size() == 0);
}

TEST_CASE("Parse VRML File - Valid Input - Simple VRML File - Switch node III.", "[parsing][valid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(validSwitchNodeEmpty, manager);
    REQUIRE(parseResult);

    vrml_proc::action::ConversionContextActionMap<to_stl::conversion_context::MeshTaskConversionContext> actionMap = to_stl::conversion_context::CreateActionMap();

    auto traversorResult = vrml_proc::traversor::VrmlFileTraversor::Traverse<to_stl::conversion_context::MeshTaskConversionContext>({ parseResult.value(), manager }, actionMap);
    REQUIRE(traversorResult.has_value());
    auto& meshContext = traversorResult.value()->GetData();
    REQUIRE(meshContext.size() == 0);
}

TEST_CASE("Parse VRML File - Valid Input - Simple VRML File - Switch node IV.", "[parsing][valid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(validSwitchNodeWithMulipleChoices, manager);
    REQUIRE(parseResult);

    vrml_proc::action::ConversionContextActionMap<to_stl::conversion_context::MeshTaskConversionContext> actionMap = to_stl::conversion_context::CreateActionMap();

    auto traversorResult = vrml_proc::traversor::VrmlFileTraversor::Traverse<to_stl::conversion_context::MeshTaskConversionContext>({ parseResult.value(), manager }, actionMap);
    REQUIRE(traversorResult.has_value());
    auto& meshContext = traversorResult.value()->GetData();
    REQUIRE(meshContext.size() == 2);
}

TEST_CASE("Parse VRML File - Invalid Input - Simple VRML File - Switch node I.", "[parsing][invalid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(invalidSwitchNodeWithWrongMulipleChoices, manager);
    REQUIRE(parseResult);

    vrml_proc::action::ConversionContextActionMap<to_stl::conversion_context::MeshTaskConversionContext> actionMap = to_stl::conversion_context::CreateActionMap();

    auto traversorResult = vrml_proc::traversor::VrmlFileTraversor::Traverse<to_stl::conversion_context::MeshTaskConversionContext>({ parseResult.value(), manager }, actionMap);
    REQUIRE(traversorResult.has_error());
    HandleRootLevelError(traversorResult);
}

TEST_CASE("Parse VRML File - Invalid Input - Simple VRML File - Switch node II.", "[parsing][invalid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(invalidSwitchNodeInvalidFieldType, manager);
    REQUIRE(parseResult);

    vrml_proc::action::ConversionContextActionMap<to_stl::conversion_context::MeshTaskConversionContext> actionMap = to_stl::conversion_context::CreateActionMap();

    auto traversorResult = vrml_proc::traversor::VrmlFileTraversor::Traverse<to_stl::conversion_context::MeshTaskConversionContext>({ parseResult.value(), manager }, actionMap);
    REQUIRE(traversorResult.has_error());
    HandleRootLevelError(traversorResult);
}

TEST_CASE("Parse VRML File - Invalid Input - Simple VRML File - Switch node III.", "[parsing][invalid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(invalidSwitchNodeWrongIndexUpper, manager);
    REQUIRE(parseResult);

    vrml_proc::action::ConversionContextActionMap<to_stl::conversion_context::MeshTaskConversionContext> actionMap = to_stl::conversion_context::CreateActionMap();

    auto traversorResult = vrml_proc::traversor::VrmlFileTraversor::Traverse<to_stl::conversion_context::MeshTaskConversionContext>({ parseResult.value(), manager }, actionMap);
    REQUIRE(traversorResult.has_value());
    auto& meshContext = traversorResult.value()->GetData();
    REQUIRE(meshContext.size() == 0);
}

TEST_CASE("Parse VRML File - Invalid Input - Simple VRML File - Switch node IV.", "[parsing][invalid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(invalidSwitchNodeWrongIndexLower, manager);
    REQUIRE(parseResult);

    vrml_proc::action::ConversionContextActionMap<to_stl::conversion_context::MeshTaskConversionContext> actionMap = to_stl::conversion_context::CreateActionMap();

    auto traversorResult = vrml_proc::traversor::VrmlFileTraversor::Traverse<to_stl::conversion_context::MeshTaskConversionContext>({ parseResult.value(), manager }, actionMap);
    REQUIRE(traversorResult.has_value());
    auto& meshContext = traversorResult.value()->GetData();
    REQUIRE(meshContext.size() == 0);
}

TEST_CASE("Parse VRML File - Invalid Input - Simple VRML File - Switch node V.", "[parsing][invalid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(invalidSwitchNodeEmptyChoice, manager);
    REQUIRE(parseResult);

    vrml_proc::action::ConversionContextActionMap<to_stl::conversion_context::MeshTaskConversionContext> actionMap = to_stl::conversion_context::CreateActionMap();

    auto traversorResult = vrml_proc::traversor::VrmlFileTraversor::Traverse<to_stl::conversion_context::MeshTaskConversionContext>({ parseResult.value(), manager }, actionMap);
    REQUIRE(traversorResult.has_value());
    auto& meshContext = traversorResult.value()->GetData();
    REQUIRE(meshContext.size() == 0);
}

//TEST_CASE("Parse VRMLFile From File - Valid Input - Actin", "[parsing][valid][fromfile]") {
//
//    vrml_proc::parser::VrmlNodeManager manager;
//    auto parseResult = ParseVrmlFile(std::filesystem::path(R"(C:\Users\marek\Documents\FI_MUNI\sem_05\SBAPR\Datasets\Segmented cells\actin.wrl)"), manager);
//    REQUIRE(parseResult);
//
//    vrml_proc::action::ConversionContextActionMap<to_stl::conversion_context::MeshTaskConversionContext> actionMap = to_stl::conversion_context::CreateActionMap();
//
//    auto traversorResult = vrml_proc::traversor::VrmlFileTraversor::Traverse<to_stl::conversion_context::MeshTaskConversionContext>({ parseResult.value(), manager }, actionMap);
//    REQUIRE(traversorResult.has_value());
//    auto& meshContext = traversorResult.value()->GetData();
//    REQUIRE(meshContext.size() == 5);
//
//    GENERATE_TEST_OUTPUT_PATH(filepath);
//    ExportToStl(meshContext, filepath);
//}
//
//TEST_CASE("Parse VRMLFile From File - Valid Input - Nucleus", "[parsing][valid][fromfile]") {
//
//    vrml_proc::parser::VrmlNodeManager manager;
//    auto parseResult = ParseVrmlFile(std::filesystem::path(R"(C:\Users\marek\Documents\FI_MUNI\sem_05\SBAPR\Datasets\Segmented cells\nucleus.wrl)"), manager);
//    REQUIRE(parseResult);
//
//    vrml_proc::action::ConversionContextActionMap<to_stl::conversion_context::MeshTaskConversionContext> actionMap = to_stl::conversion_context::CreateActionMap();
//
//    auto traversorResult = vrml_proc::traversor::VrmlFileTraversor::Traverse<to_stl::conversion_context::MeshTaskConversionContext>({ parseResult.value(), manager }, actionMap);
//    REQUIRE(traversorResult.has_value());
//    auto& meshContext = traversorResult.value()->GetData();
//    REQUIRE(meshContext.size() == 2);
//
//    GENERATE_TEST_OUTPUT_PATH(filepath);
//    ExportToStl(meshContext, filepath);
//}
//
//TEST_CASE("Parse VRMLFile From File - Valid Input - Tubulin", "[parsing][valid][fromfile]") {
//
//    vrml_proc::parser::VrmlNodeManager manager;
//    auto parseResult = ParseVrmlFile(std::filesystem::path(R"(C:\Users\marek\Documents\FI_MUNI\sem_05\SBAPR\Datasets\Segmented cells\tubulin.wrl)"), manager);
//    REQUIRE(parseResult);
//
//    for (const auto& root : parseResult.value()) {
//        root.Print(0);
//    }
//
//    vrml_proc::action::ConversionContextActionMap<to_stl::conversion_context::MeshTaskConversionContext> actionMap = to_stl::conversion_context::CreateActionMap();
//
//    auto traversorResult = vrml_proc::traversor::VrmlFileTraversor::Traverse<to_stl::conversion_context::MeshTaskConversionContext>({ parseResult.value(), manager }, actionMap);
//    REQUIRE(traversorResult.has_value());
//    auto& meshContext = traversorResult.value()->GetData();
//    REQUIRE(meshContext.size() == 399);
//
//    GENERATE_TEST_OUTPUT_PATH(filepath);
//    ExportToStl(meshContext, filepath);
//}
//
//TEST_CASE("Parse VRMLFile From File - Valid Input - Tile 3x5 - 5 Stitch", "[parsing][valid][fromfile]") {
//
//    vrml_proc::parser::VrmlNodeManager manager;
//    auto parseResult = ParseVrmlFile(std::filesystem::path(R"(C:\Users\marek\Documents\FI_MUNI\sem_05\SBAPR\Datasets\Segmented cells\tile3x5-5_Stitch.wrl)"), manager);
//    REQUIRE(parseResult);
//
//    vrml_proc::action::ConversionContextActionMap<to_stl::conversion_context::MeshTaskConversionContext>& actionMap = to_stl::conversion_context::CreateActionMap();
//
//    auto traversorResult = vrml_proc::traversor::VrmlFileTraversor::Traverse<to_stl::conversion_context::MeshTaskConversionContext>({ parseResult.value(), manager }, actionMap);
//    
//    REQUIRE(traversorResult.has_value());
//    auto& meshContext = traversorResult.value()->GetData();
//    REQUIRE(meshContext.size() == 64611);
//
//    GENERATE_TEST_OUTPUT_PATH(filepath);
//    ExportToStl(meshContext, filepath);
//}