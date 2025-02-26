#include <catch2/catch_test_macros.hpp>

#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include <BoxCalculator.hpp>
#include <CalculatorError.hpp>
#include <IndexedFaceSetCalculator.hpp>
#include <Int32Array.hpp>
#include <Logger.hpp>
#include <ModelValidationError.hpp>
#include <StlFileWriter.hpp>
#include <Transformation.hpp>
#include <TransformationMatrix.hpp>
#include <Vec3f.hpp>
#include <VrmlUnits.hpp>

#include "../../test_common/TestCommon.hpp"

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

    /*to_stl::core::io::StlFileWriter writer;
    writer.Write(std::filesystem::path(R"(C:\Users\marek\Documents\FI_MUNI\sem_05\SBAPR\vrmlxpy\export1.stl)"), *result1);
    writer.Write(std::filesystem::path(R"(C:\Users\marek\Documents\FI_MUNI\sem_05\SBAPR\vrmlxpy\export2.stl)"), *result2);
    writer.Write(std::filesystem::path(R"(C:\Users\marek\Documents\FI_MUNI\sem_05\SBAPR\vrmlxpy\export3.stl)"), *result3);
    writer.Write(std::filesystem::path(R"(C:\Users\marek\Documents\FI_MUNI\sem_05\SBAPR\vrmlxpy\export4.stl)"), *result4);
    writer.Write(std::filesystem::path(R"(C:\Users\marek\Documents\FI_MUNI\sem_05\SBAPR\vrmlxpy\export5.stl)"), *result5);
    writer.Write(std::filesystem::path(R"(C:\Users\marek\Documents\FI_MUNI\sem_05\SBAPR\vrmlxpy\export6.stl)"), *result6);
    writer.Write(std::filesystem::path(R"(C:\Users\marek\Documents\FI_MUNI\sem_05\SBAPR\vrmlxpy\export7.stl)"), *result7);
    writer.Write(std::filesystem::path(R"(C:\Users\marek\Documents\FI_MUNI\sem_05\SBAPR\vrmlxpy\export8.stl)"), *result8);
    writer.Write(std::filesystem::path(R"(C:\Users\marek\Documents\FI_MUNI\sem_05\SBAPR\vrmlxpy\export9.stl)"), *result9);
    writer.Write(std::filesystem::path(R"(C:\Users\marek\Documents\FI_MUNI\sem_05\SBAPR\vrmlxpy\export10.stl)"), *result10);
    writer.Write(std::filesystem::path(R"(C:\Users\marek\Documents\FI_MUNI\sem_05\SBAPR\vrmlxpy\export11.stl)"), *result11);*/

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
        LogError(result.error());
    }

    size.x = 50.5f; size.y = 20.0f; size.z = -0.0001f;
    {
        auto result = calculator.Generate3DMesh({ std::cref(size) }, matrix);
        REQUIRE(result.has_error());
        CHECK(CheckInnermostError<vrml_proc::parser::model::validator::error::NumberOutOfRangeError<vrml_proc::parser::float32_t>>(result.error()));
        LogError(result.error());
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
        LogError(result.error());
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
        LogError(result.error());
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
        LogError(result.error());
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
        LogError(result.error());
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

        /*to_stl::core::io::StlFileWriter writer;
        writer.Write(std::filesystem::path(R"(C:\Users\marek\Documents\FI_MUNI\sem_05\SBAPR\vrmlxpy\IFS_Quad.stl)"), *(result.value()));*/
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
        LogError(result.error());
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
        LogError(result.error());
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
        LogError(result.error());
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
        LogError(result.error());
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
        LogError(result.error());
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
        LogError(result.error());
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
        /*to_stl::core::io::StlFileWriter writer;
        writer.Write(std::filesystem::path(R"(C:\Users\marek\Documents\FI_MUNI\sem_05\SBAPR\vrmlxpy\IFS_Pyramid.stl)"), *(result.value()));*/
    }
}