#include <catch2/catch_test_macros.hpp>

#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include <BoxCalculator.hpp>
#include <IndexedFaceSetCalculator.hpp>
#include <Logger.hpp>
#include <MemoryMappedFileReader.hpp>
#include <MeshConversionContext.hpp>
#include <ModelValidationError.hpp>
#include <ParserResult.hpp>
#include <test.hpp>
#include <Transformation.hpp>
#include <TransformationMatrix.hpp>
#include <Vec3f.hpp>
#include <VrmlFile.hpp>
#include <VrmlNodeManager.hpp>
#include <VrmlParser.hpp>
#include <Int32Array.hpp>
#include <VrmlUnits.hpp>
#include <CalculatorError.hpp>
#include <BufferView.hpp>

template <typename T>
static bool CheckInnermostError(std::shared_ptr<vrml_proc::core::error::Error> error) {
    return std::dynamic_pointer_cast<T>(vrml_proc::core::error::Error::GetInnermostError(error)) != nullptr;
}

static vrml_proc::parser::ParserResult<vrml_proc::parser::VrmlFile> ParseVrmlFile(std::string& text, vrml_proc::parser::VrmlNodeManager& manager) {

    vrml_proc::parser::VrmlParser parser(manager);
    parser.Parse(vrml_proc::parser::BufferView(text.c_str(), text.c_str() + text.size()));
}

static vrml_proc::parser::ParserResult<vrml_proc::parser::VrmlFile> ParseVrmlFile(const std::filesystem::path& filepath, vrml_proc::parser::VrmlNodeManager& manager) {

    vrml_proc::core::io::MemoryMappedFileReader reader;
    auto readResult = reader.Read(filepath);
    if (!readResult.has_value()) {
        return {};
    }

    vrml_proc::parser::VrmlParser parser(manager);
    return parser.Parse(vrml_proc::parser::BufferView(readResult.value().GetBegin(), readResult.value().GetEnd()));
}

static void HandleRootLevelError(std::shared_ptr<vrml_proc::core::error::Error> error) {
    vrml_proc::core::logger::LogUnformattedText("caught application error", error->GetMessage(), vrml_proc::core::logger::Level::Error, LOGGING_INFO);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------ //

TEST_CASE("Initialization") {

    vrml_proc::core::logger::InitLogging();
}

TEST_CASE("BoxCalculator - Basic tests", "[valid]") {

    using vrml_proc::math::TransformationMatrix;
    using vrml_proc::math::Transformation;
    using vrml_proc::parser::Vec3f;
    using vrml_proc::parser::Vec4f;
    using vrml_proc::math::UpdateTransformationMatrix;

    Vec3f size;
    size.x = 10.0f; size.y = 20.0f; size.z = 30.0f;

    to_stl::calculator::BoxCalculator calculator = to_stl::calculator::BoxCalculator();

    // --- ///

    TransformationMatrix matrix;
    auto result0 = (calculator.Generate3DMesh({ std::cref(size) }, matrix)).value();

    // TODO: compare with file

    // --- //

    Transformation transformationData;
    matrix = UpdateTransformationMatrix(matrix, transformationData);
    auto result1 = (calculator.Generate3DMesh({ std::cref(size) }, matrix)).value();

    // TODO: compare with the first file (should be same)

    // --- //

    transformationData = Transformation();
    matrix = TransformationMatrix();
    transformationData.rotation = Vec4f(0.0f, 0.0f, 1.0f, 0.785398163f);
    matrix = UpdateTransformationMatrix(matrix, transformationData);
    auto result2 = (calculator.Generate3DMesh({ std::cref(size) }, matrix)).value();

    // TODO: compare with file

    // --- //

    transformationData = Transformation();
    matrix = TransformationMatrix();
    transformationData.center = vrml_proc::parser::Vec3f(10.0f, 10.0f, 0.0f);
    transformationData.rotation = Vec4f(0.0f, 0.0f, 1.0f, 0.785398163f);
    matrix = UpdateTransformationMatrix(matrix, transformationData);
    auto result3 = (calculator.Generate3DMesh({ std::cref(size) }, matrix)).value();

    // TODO: compare with file

    // --- //

    transformationData = Transformation();
    matrix = TransformationMatrix();
    transformationData.center = vrml_proc::parser::Vec3f(10.0f, 10.0f, 0.0f);
    matrix = UpdateTransformationMatrix(matrix, transformationData);
    auto result4 = (calculator.Generate3DMesh({ std::cref(size) }, matrix)).value();

    // TODO: compare with the first file (should be same)

    // --- //

    transformationData = Transformation();
    matrix = TransformationMatrix();
    transformationData.rotation = Vec4f(0.0f, 1.0f, 0.0f, 0.785398163f);
    matrix = UpdateTransformationMatrix(matrix, transformationData);
    auto result5 = (calculator.Generate3DMesh({ std::cref(size) }, matrix)).value();

    // TODO: compare with file

    // --- //

    transformationData = Transformation();
    matrix = TransformationMatrix();
    transformationData.center = vrml_proc::parser::Vec3f(10.0f, 10.0f, 10.0f);
    transformationData.rotation = Vec4f(0.0f, 1.0f, 0.0f, 0.785398163f);
    matrix = UpdateTransformationMatrix(matrix, transformationData);
    auto result6 = (calculator.Generate3DMesh({ std::cref(size) }, matrix)).value();

    // TODO: compare with file

    // --- //

    transformationData = Transformation();
    matrix = TransformationMatrix();
    transformationData.translation = vrml_proc::parser::Vec3f(10.0f, 10.0f, 10.0f);
    transformationData.rotation = Vec4f(0.0f, 0.0f, 1.0f, 0.785398163f);
    matrix = UpdateTransformationMatrix(matrix, transformationData);
    auto result7 = (calculator.Generate3DMesh({ std::cref(size) }, matrix)).value();

    // TODO: compare with file

    // --- //

    transformationData = Transformation();
    matrix = TransformationMatrix();
    transformationData.translation = vrml_proc::parser::Vec3f(0.0f, -25.0f, 0.0f);
    transformationData.rotation = vrml_proc::parser::Vec4f(0.0f, 0.0f, 1.0f, 0.785398163f);
    transformationData.scale = vrml_proc::parser::Vec3f(2.0f, 1.0f, 1.0f);
    transformationData.scaleOrientation = vrml_proc::parser::Vec4f(0.0f, 0.0f, 1.0f, 0.785398163f); // without it applies scaling to world coordinates
    matrix = vrml_proc::math::UpdateTransformationMatrix(matrix, transformationData);
    auto result8 = (calculator.Generate3DMesh({ std::cref(size) }, matrix)).value();

    // TODO: compare with file

    // --- //

    transformationData = Transformation();
    matrix = TransformationMatrix();
    transformationData.translation = vrml_proc::parser::Vec3f(0.0f, -25.0f, 0.0f);
    transformationData.rotation = vrml_proc::parser::Vec4f(0.0f, 0.0f, 1.0f, 0.785398163f);
    transformationData.scale = vrml_proc::parser::Vec3f(2.0f, 1.0f, 1.0f);
    matrix = vrml_proc::math::UpdateTransformationMatrix(matrix, transformationData);
    auto result9 = (calculator.Generate3DMesh({ std::cref(size) }, matrix)).value();

    // TODO: compare with file

    // --- //

    transformationData = Transformation();
    transformationData.translation = vrml_proc::parser::Vec3f(15.0f, 0.0f, 0.0f);
    transformationData.rotation = vrml_proc::parser::Vec4f(1.0f, 0.0f, 0.0f, 0.785398163f);
    matrix = vrml_proc::math::UpdateTransformationMatrix(matrix, transformationData);
    auto result10 = (calculator.Generate3DMesh({ std::cref(size) }, matrix)).value();

    // TODO: compare with file

    // --- //

    transformationData = Transformation();
    matrix = TransformationMatrix();
    transformationData.scale = vrml_proc::parser::Vec3f(2.0f, 2.0f, 2.0f);
    matrix = vrml_proc::math::UpdateTransformationMatrix(matrix, transformationData);
    matrix = vrml_proc::math::UpdateTransformationMatrix(matrix, transformationData);
    matrix = vrml_proc::math::UpdateTransformationMatrix(matrix, transformationData);
    matrix = vrml_proc::math::UpdateTransformationMatrix(matrix, transformationData);
    auto result11 = (calculator.Generate3DMesh({ std::cref(size) }, matrix)).value();
    
    // TODO: compare with file

    // --- //

    /*to_stl::core::Mesh mesh;
    mesh.join(*result1);
    mesh.join(*result2);
    mesh.join(*result3);*/
    //mesh.join(*result4);
    //mesh.join(*result5);
    export_to_stl(*result1, R"(C:\Users\marek\Documents\FI_MUNI\sem_05\SBAPR\vrmlxpy\export1.stl)");
    export_to_stl(*result2, R"(C:\Users\marek\Documents\FI_MUNI\sem_05\SBAPR\vrmlxpy\export2.stl)");
    export_to_stl(*result3, R"(C:\Users\marek\Documents\FI_MUNI\sem_05\SBAPR\vrmlxpy\export3.stl)");
    export_to_stl(*result4, R"(C:\Users\marek\Documents\FI_MUNI\sem_05\SBAPR\vrmlxpy\export4.stl)");
    export_to_stl(*result5, R"(C:\Users\marek\Documents\FI_MUNI\sem_05\SBAPR\vrmlxpy\export5.stl)");
    export_to_stl(*result6, R"(C:\Users\marek\Documents\FI_MUNI\sem_05\SBAPR\vrmlxpy\export6.stl)");
    export_to_stl(*result7, R"(C:\Users\marek\Documents\FI_MUNI\sem_05\SBAPR\vrmlxpy\export7.stl)");
    export_to_stl(*result8, R"(C:\Users\marek\Documents\FI_MUNI\sem_05\SBAPR\vrmlxpy\export8.stl)");
    export_to_stl(*result9, R"(C:\Users\marek\Documents\FI_MUNI\sem_05\SBAPR\vrmlxpy\export9.stl)");
    export_to_stl(*result10, R"(C:\Users\marek\Documents\FI_MUNI\sem_05\SBAPR\vrmlxpy\export10.stl)");
    export_to_stl(*result11, R"(C:\Users\marek\Documents\FI_MUNI\sem_05\SBAPR\vrmlxpy\export11.stl)");
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

TEST_CASE("IndexedFaceSetCalculator - valid I.", "[valid]") {

    to_stl::calculator::IndexedFaceSetCalculator calculator = to_stl::calculator::IndexedFaceSetCalculator();

    vrml_proc::parser::Int32Array indices;
    vrml_proc::parser::Vec3fArray points;
    bool isConvex = false;

    vrml_proc::math::TransformationMatrix matrix;

    {
        auto result = calculator.Generate3DMesh(std::cref(indices), std::cref(points), std::cref(isConvex), matrix);
        REQUIRE(result.has_value());
        REQUIRE(result.value()->is_empty());
    }
}

TEST_CASE("IndexedFaceSetCalculator - invalid I.", "[invalid]") {

    to_stl::calculator::IndexedFaceSetCalculator calculator = to_stl::calculator::IndexedFaceSetCalculator();

    vrml_proc::parser::Int32Array indices;
    indices.integers.emplace_back(0); indices.integers.emplace_back(1); indices.integers.emplace_back(2); indices.integers.emplace_back(-1);
    vrml_proc::parser::Vec3fArray points;
    bool isConvex = false;

    vrml_proc::math::TransformationMatrix matrix;

    {
        auto result = calculator.Generate3DMesh(std::cref(indices), std::cref(points), std::cref(isConvex), matrix);
        REQUIRE(result.has_error());
        CHECK(CheckInnermostError<vrml_proc::parser::model::validator::error::EmptyArrayError>(result.error()));
        HandleRootLevelError(result.error());
    }
}

TEST_CASE("IndexedFaceSetCalculator - invalid II.", "[invalid]") {

    using vrml_proc::parser::Vec3f;

    to_stl::calculator::IndexedFaceSetCalculator calculator = to_stl::calculator::IndexedFaceSetCalculator();

    vrml_proc::parser::Int32Array indices;
    indices.integers.emplace_back(-1);

    vrml_proc::parser::Vec3fArray points;
    points.vectors.emplace_back(Vec3f(0.0f, 0.0f, 0.0f)); points.vectors.emplace_back(Vec3f(1.0f, 0.0f, 0.0f)); points.vectors.emplace_back(Vec3f(1.0f, 1.0f, 0.0f));
    points.vectors.emplace_back(Vec3f(0.0f, 1.0f, 0.0f)); points.vectors.emplace_back(Vec3f(0.5f, 0.5f, 1.0f));

    bool isConvex = false;

    vrml_proc::math::TransformationMatrix matrix;

    {
        auto result = calculator.Generate3DMesh(std::cref(indices), std::cref(points), std::cref(isConvex), matrix);
        REQUIRE(result.has_error());
        CHECK(CheckInnermostError<to_stl::calculator::error::InvalidNumberOfCoordinatesForFaceError>(result.error()));
        HandleRootLevelError(result.error());
    }
}

TEST_CASE("IndexedFaceSetCalculator - invalid III.", "[invalid]") {

    using vrml_proc::parser::Vec3f;

    to_stl::calculator::IndexedFaceSetCalculator calculator = to_stl::calculator::IndexedFaceSetCalculator();

    vrml_proc::parser::Int32Array indices;
    indices.integers.emplace_back(0); indices.integers.emplace_back(-1);

    vrml_proc::parser::Vec3fArray points;
    points.vectors.emplace_back(Vec3f(0.0f, 0.0f, 0.0f)); points.vectors.emplace_back(Vec3f(1.0f, 0.0f, 0.0f)); points.vectors.emplace_back(Vec3f(1.0f, 1.0f, 0.0f));
    points.vectors.emplace_back(Vec3f(0.0f, 1.0f, 0.0f)); points.vectors.emplace_back(Vec3f(0.5f, 0.5f, 1.0f));

    bool isConvex = false;

    vrml_proc::math::TransformationMatrix matrix;

    {
        auto result = calculator.Generate3DMesh(std::cref(indices), std::cref(points), std::cref(isConvex), matrix);
        REQUIRE(result.has_error());
        CHECK(CheckInnermostError<to_stl::calculator::error::InvalidNumberOfCoordinatesForFaceError>(result.error()));
        HandleRootLevelError(result.error());
    }
}

TEST_CASE("IndexedFaceSetCalculator - invalid IV.", "[invalid]") {

    using vrml_proc::parser::Vec3f;

    to_stl::calculator::IndexedFaceSetCalculator calculator = to_stl::calculator::IndexedFaceSetCalculator();

    vrml_proc::parser::Int32Array indices;
    indices.integers.emplace_back(0); indices.integers.emplace_back(1); indices.integers.emplace_back(-1);

    vrml_proc::parser::Vec3fArray points;
    points.vectors.emplace_back(Vec3f(0.0f, 0.0f, 0.0f)); points.vectors.emplace_back(Vec3f(1.0f, 0.0f, 0.0f)); points.vectors.emplace_back(Vec3f(1.0f, 1.0f, 0.0f));
    points.vectors.emplace_back(Vec3f(0.0f, 1.0f, 0.0f)); points.vectors.emplace_back(Vec3f(0.5f, 0.5f, 1.0f));

    bool isConvex = false;

    vrml_proc::math::TransformationMatrix matrix;

    {
        auto result = calculator.Generate3DMesh(std::cref(indices), std::cref(points), std::cref(isConvex), matrix);
        REQUIRE(result.has_error());
        CHECK(CheckInnermostError<to_stl::calculator::error::InvalidNumberOfCoordinatesForFaceError>(result.error()));
        HandleRootLevelError(result.error());
    }
}

TEST_CASE("IndexedFaceSetCalculator - valid II.", "[invalid]") {

    using vrml_proc::parser::Vec3f;

    to_stl::calculator::IndexedFaceSetCalculator calculator = to_stl::calculator::IndexedFaceSetCalculator();

    vrml_proc::parser::Int32Array indices;
    indices.integers.emplace_back(0); indices.integers.emplace_back(1); indices.integers.emplace_back(2); indices.integers.emplace_back(-1);
    indices.integers.emplace_back(0); indices.integers.emplace_back(2); indices.integers.emplace_back(3); indices.integers.emplace_back(-1);

    vrml_proc::parser::Vec3fArray points;
    points.vectors.emplace_back(Vec3f(0.0f, 0.0f, 0.0f)); points.vectors.emplace_back(Vec3f(1.0f, 0.0f, 0.0f)); points.vectors.emplace_back(Vec3f(1.0f, 1.0f, 0.0f));
    points.vectors.emplace_back(Vec3f(0.0f, 1.0f, 0.0f)); points.vectors.emplace_back(Vec3f(0.5f, 0.5f, 1.0f));

    bool isConvex = false;

    vrml_proc::math::TransformationMatrix matrix;

    {
        auto result = calculator.Generate3DMesh(std::cref(indices), std::cref(points), std::cref(isConvex), matrix);
        REQUIRE(result.has_value());
        export_to_stl(*(result.value()), R"(C:\Users\marek\Documents\FI_MUNI\sem_05\SBAPR\vrmlxpy\IFS_Quad.stl)");
    }
}

TEST_CASE("IndexedFaceSetCalculator - invalid V.", "[invalid]") {

    using vrml_proc::parser::Vec3f;

    to_stl::calculator::IndexedFaceSetCalculator calculator = to_stl::calculator::IndexedFaceSetCalculator();

    vrml_proc::parser::Int32Array indices;
    indices.integers.emplace_back(-2); indices.integers.emplace_back(1); indices.integers.emplace_back(2); indices.integers.emplace_back(-1);

    vrml_proc::parser::Vec3fArray points;
    points.vectors.emplace_back(Vec3f(0.0f, 0.0f, 0.0f)); points.vectors.emplace_back(Vec3f(1.0f, 0.0f, 0.0f)); points.vectors.emplace_back(Vec3f(1.0f, 1.0f, 0.0f));
    points.vectors.emplace_back(Vec3f(0.0f, 1.0f, 0.0f)); points.vectors.emplace_back(Vec3f(0.5f, 0.5f, 1.0f));

    bool isConvex = false;

    vrml_proc::math::TransformationMatrix matrix;

    {
        auto result = calculator.Generate3DMesh(std::cref(indices), std::cref(points), std::cref(isConvex), matrix);
        REQUIRE(result.has_error());
        CHECK(CheckInnermostError<vrml_proc::parser::model::validator::error::NumberOutOfRangeError<int32_t>>(result.error()));
        HandleRootLevelError(result.error());
    }
}

TEST_CASE("IndexedFaceSetCalculator - invalid VI.", "[invalid]") {

    using vrml_proc::parser::Vec3f;

    to_stl::calculator::IndexedFaceSetCalculator calculator = to_stl::calculator::IndexedFaceSetCalculator();

    vrml_proc::parser::Int32Array indices;
    indices.integers.emplace_back(5); indices.integers.emplace_back(1); indices.integers.emplace_back(2); indices.integers.emplace_back(-1);

    vrml_proc::parser::Vec3fArray points;
    points.vectors.emplace_back(Vec3f(0.0f, 0.0f, 0.0f)); points.vectors.emplace_back(Vec3f(1.0f, 0.0f, 0.0f)); points.vectors.emplace_back(Vec3f(1.0f, 1.0f, 0.0f));
    points.vectors.emplace_back(Vec3f(0.0f, 1.0f, 0.0f)); points.vectors.emplace_back(Vec3f(0.5f, 0.5f, 1.0f));

    bool isConvex = false;

    vrml_proc::math::TransformationMatrix matrix;

    {
        auto result = calculator.Generate3DMesh(std::cref(indices), std::cref(points), std::cref(isConvex), matrix);
        REQUIRE(result.has_error());
        CHECK(CheckInnermostError<vrml_proc::parser::model::validator::error::NumberOutOfRangeError<int32_t>>(result.error()));
        HandleRootLevelError(result.error());
    }
}

TEST_CASE("IndexedFaceSetCalculator - invalid VII.", "[invalid]") {

    using vrml_proc::parser::Vec3f;

    to_stl::calculator::IndexedFaceSetCalculator calculator = to_stl::calculator::IndexedFaceSetCalculator();

    vrml_proc::parser::Int32Array indices;
    indices.integers.emplace_back(0); indices.integers.emplace_back(-2); indices.integers.emplace_back(2); indices.integers.emplace_back(-1);

    vrml_proc::parser::Vec3fArray points;
    points.vectors.emplace_back(Vec3f(0.0f, 0.0f, 0.0f)); points.vectors.emplace_back(Vec3f(1.0f, 0.0f, 0.0f)); points.vectors.emplace_back(Vec3f(1.0f, 1.0f, 0.0f));
    points.vectors.emplace_back(Vec3f(0.0f, 1.0f, 0.0f)); points.vectors.emplace_back(Vec3f(0.5f, 0.5f, 1.0f));

    bool isConvex = false;

    vrml_proc::math::TransformationMatrix matrix;

    {
        auto result = calculator.Generate3DMesh(std::cref(indices), std::cref(points), std::cref(isConvex), matrix);
        REQUIRE(result.has_error());
        CHECK(CheckInnermostError<vrml_proc::parser::model::validator::error::NumberOutOfRangeError<int32_t>>(result.error()));
        HandleRootLevelError(result.error());
    }
}

TEST_CASE("IndexedFaceSetCalculator - invalid VIII.", "[invalid]") {

    using vrml_proc::parser::Vec3f;

    to_stl::calculator::IndexedFaceSetCalculator calculator = to_stl::calculator::IndexedFaceSetCalculator();

    vrml_proc::parser::Int32Array indices;
    indices.integers.emplace_back(0); indices.integers.emplace_back(5); indices.integers.emplace_back(2); indices.integers.emplace_back(-1);

    vrml_proc::parser::Vec3fArray points;
    points.vectors.emplace_back(Vec3f(0.0f, 0.0f, 0.0f)); points.vectors.emplace_back(Vec3f(1.0f, 0.0f, 0.0f)); points.vectors.emplace_back(Vec3f(1.0f, 1.0f, 0.0f));
    points.vectors.emplace_back(Vec3f(0.0f, 1.0f, 0.0f)); points.vectors.emplace_back(Vec3f(0.5f, 0.5f, 1.0f));

    bool isConvex = false;

    vrml_proc::math::TransformationMatrix matrix;

    {
        auto result = calculator.Generate3DMesh(std::cref(indices), std::cref(points), std::cref(isConvex), matrix);
        REQUIRE(result.has_error());
        CHECK(CheckInnermostError<vrml_proc::parser::model::validator::error::NumberOutOfRangeError<int32_t>>(result.error()));
        HandleRootLevelError(result.error());
    }
}

TEST_CASE("IndexedFaceSetCalculator - invalid IX.", "[invalid]") {

    using vrml_proc::parser::Vec3f;

    to_stl::calculator::IndexedFaceSetCalculator calculator = to_stl::calculator::IndexedFaceSetCalculator();

    vrml_proc::parser::Int32Array indices;
    indices.integers.emplace_back(0); indices.integers.emplace_back(1); indices.integers.emplace_back(-2); indices.integers.emplace_back(-1);

    vrml_proc::parser::Vec3fArray points;
    points.vectors.emplace_back(Vec3f(0.0f, 0.0f, 0.0f)); points.vectors.emplace_back(Vec3f(1.0f, 0.0f, 0.0f)); points.vectors.emplace_back(Vec3f(1.0f, 1.0f, 0.0f));
    points.vectors.emplace_back(Vec3f(0.0f, 1.0f, 0.0f)); points.vectors.emplace_back(Vec3f(0.5f, 0.5f, 1.0f));

    bool isConvex = false;

    vrml_proc::math::TransformationMatrix matrix;

    {
        auto result = calculator.Generate3DMesh(std::cref(indices), std::cref(points), std::cref(isConvex), matrix);
        REQUIRE(result.has_error());
        CHECK(CheckInnermostError<vrml_proc::parser::model::validator::error::NumberOutOfRangeError<int32_t>>(result.error()));
        HandleRootLevelError(result.error());
    }
}

TEST_CASE("IndexedFaceSetCalculator - invalid X.", "[invalid]") {

    using vrml_proc::parser::Vec3f;

    to_stl::calculator::IndexedFaceSetCalculator calculator = to_stl::calculator::IndexedFaceSetCalculator();

    vrml_proc::parser::Int32Array indices;
    indices.integers.emplace_back(0); indices.integers.emplace_back(1); indices.integers.emplace_back(5); indices.integers.emplace_back(-1);

    vrml_proc::parser::Vec3fArray points;
    points.vectors.emplace_back(Vec3f(0.0f, 0.0f, 0.0f)); points.vectors.emplace_back(Vec3f(1.0f, 0.0f, 0.0f)); points.vectors.emplace_back(Vec3f(1.0f, 1.0f, 0.0f));
    points.vectors.emplace_back(Vec3f(0.0f, 1.0f, 0.0f)); points.vectors.emplace_back(Vec3f(0.5f, 0.5f, 1.0f));

    bool isConvex = false;

    vrml_proc::math::TransformationMatrix matrix;

    {
        auto result = calculator.Generate3DMesh(std::cref(indices), std::cref(points), std::cref(isConvex), matrix);
        REQUIRE(result.has_error());
        CHECK(CheckInnermostError<vrml_proc::parser::model::validator::error::NumberOutOfRangeError<int32_t>>(result.error()));
        HandleRootLevelError(result.error());
    }
}

TEST_CASE("IndexedFaceSetCalculator - valid III.", "[valid]") {

    using vrml_proc::parser::Vec3f;

    to_stl::calculator::IndexedFaceSetCalculator calculator = to_stl::calculator::IndexedFaceSetCalculator();

    vrml_proc::parser::Int32Array indices;
    indices.integers.emplace_back(0); indices.integers.emplace_back(1); indices.integers.emplace_back(2); indices.integers.emplace_back(-1);
    indices.integers.emplace_back(0); indices.integers.emplace_back(2); indices.integers.emplace_back(3); indices.integers.emplace_back(-1);
    indices.integers.emplace_back(0); indices.integers.emplace_back(1); indices.integers.emplace_back(4); indices.integers.emplace_back(-1);
    indices.integers.emplace_back(1); indices.integers.emplace_back(2); indices.integers.emplace_back(4); indices.integers.emplace_back(-1);
    indices.integers.emplace_back(2); indices.integers.emplace_back(3); indices.integers.emplace_back(4); indices.integers.emplace_back(-1);
    indices.integers.emplace_back(3); indices.integers.emplace_back(0); indices.integers.emplace_back(4); indices.integers.emplace_back(-1);

    vrml_proc::parser::Vec3fArray points;
    points.vectors.emplace_back(Vec3f(0.0f, 0.0f, 0.0f)); points.vectors.emplace_back(Vec3f(1.0f, 0.0f, 0.0f)); points.vectors.emplace_back(Vec3f(1.0f, 1.0f, 0.0f));
    points.vectors.emplace_back(Vec3f(0.0f, 1.0f, 0.0f)); points.vectors.emplace_back(Vec3f(0.5f, 0.5f, 1.0f));

    bool isConvex = false;

    vrml_proc::math::TransformationMatrix matrix;

    {
        auto result = calculator.Generate3DMesh(std::cref(indices), std::cref(points), std::cref(isConvex), matrix);
        REQUIRE(result.has_value());
        export_to_stl(*(result.value()), R"(C:\Users\marek\Documents\FI_MUNI\sem_05\SBAPR\vrmlxpy\IFS_Pyradamid.stl)");
    }
}