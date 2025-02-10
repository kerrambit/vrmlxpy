#include <catch2/catch_test_macros.hpp>

#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include "test_data/VrmlFileTraversorTestDataset.hpp"
#include <BoxAction.hpp>
#include <BoxCalculator.hpp>
#include <ConversionContextActionMap.hpp>
#include <GroupAction.hpp>
#include <Logger.hpp>
#include <MemoryMappedFileReader.hpp>
#include <MeshConversionContext.hpp>
#include <ModelValidationError.hpp>
#include <ParserResult.hpp>
#include <ShapeAction.hpp>
#include <test.hpp>
#include <Vec3f.hpp>
#include <VrmlFile.hpp>
#include <VrmlFileTraversor.hpp>
#include <VrmlNodeManager.hpp>
#include <VrmlParser.hpp>
#include <VrmlUnits.hpp>
#include <Transformation.hpp>
#include <TransformationMatrix.hpp>

template <typename T>
static bool CheckInnermostError(std::shared_ptr<vrml_proc::core::error::Error> error) {
    return std::dynamic_pointer_cast<T>(vrml_proc::core::error::Error::GetInnermostError(error)) != nullptr;
}

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

// ------------------------------------------------------------------------------------------------------------------------------------------------ //

TEST_CASE("Initialization") {

    vrml_proc::core::logger::InitLogging();

    vrml_proc::parser::Vec3f size;
    size.x = 2.0f; size.y = 2.0f; size.z = 2.0f;

    vrml_proc::math::Transformation transformationData;
    vrml_proc::math::TransformationMatrix matrix;

    transformationData.center = vrml_proc::parser::Vec3f(20.0f, 20.0f, 20.0f);
    vrml_proc::math::UpdateTransformationMatrix(matrix, transformationData);

    to_stl::calculator::BoxCalculator calculator = to_stl::calculator::BoxCalculator();
    auto result = (calculator.Generate3DMesh({ std::cref(size) }, matrix)).value();

    size.x = 10.0f; size.y = 20.0f; size.z = 30.0f;
    auto result2 = (calculator.Generate3DMesh({ std::cref(size) }, matrix)).value();

    transformationData.translation = vrml_proc::parser::Vec3f(20.0f, 20.0f, 20.0f);
    vrml_proc::math::UpdateTransformationMatrix(matrix, transformationData);

    size.x = 2.0f; size.y = 2.0f; size.z = 2.0f;
    auto result3 = (calculator.Generate3DMesh({ std::cref(size) }, matrix)).value();

    size.x = 10.0f; size.y = 20.0f; size.z = 30.0f;
    auto result4 = (calculator.Generate3DMesh({ std::cref(size) }, matrix)).value();

    to_stl::core::Mesh mesh;
    mesh.join(*result);
    mesh.join(*result2);
    mesh.join(*result3);
    mesh.join(*result4);
    export_to_stl(mesh, R"(C:\Users\marek\Documents\FI_MUNI\sem_05\SBAPR\vrmlxpy\export.stl)");
}

TEST_CASE("BoxCalculator - invalid", "[invalid]") {

    to_stl::calculator::BoxCalculator calculator = to_stl::calculator::BoxCalculator();

    vrml_proc::parser::Vec3f size;
    size.x = 0.0f; size.y = 20.0f; size.z = 30.0f;

    vrml_proc::math::TransformationMatrix matrix;

    {
        auto result = calculator.Generate3DMesh({ std::cref(size) }, matrix);
        REQUIRE(result.has_error());
        CHECK(CheckInnermostError<vrml_proc::parser::model::validator::error::NumberOutOfRangeError<vrml_proc::parser::float32_t>>(result.error()));
        HandleRootLevelError(result.error());
    }

    size.x = 50.5f; size.y = 20.0f; size.z = -0.0001f;
    {
        auto result = calculator.Generate3DMesh({ std::cref(size) }, matrix);
        REQUIRE(result.has_error());
        CHECK(CheckInnermostError<vrml_proc::parser::model::validator::error::NumberOutOfRangeError<vrml_proc::parser::float32_t>>(result.error()));
        HandleRootLevelError(result.error());
    }
}
