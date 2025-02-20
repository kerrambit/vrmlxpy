#include "IndexedFaceSetCalculator.hpp"

#include <cstdint>
#include <memory>
#include <vector>

#include <CGAL/Kernel/interface_macros.h>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Surface_mesh/Surface_mesh.h>

#include <result.hpp>

#include "CalculatorError.hpp"
#include "CGALBaseTypesForVrml.hpp"
#include "Error.hpp"
#include "FormatString.hpp"
#include "Int32Array.hpp"
#include "Logger.hpp"
#include "ManualTimer.hpp"
#include "Mesh.hpp"
#include "ModelValidationError.hpp"
#include "Range.hpp"
#include "UnsupportedOperationError.hpp"

namespace to_stl::calculator {
    cpp::result<std::shared_ptr<core::Mesh>, std::shared_ptr<vrml_proc::core::error::Error>>
        IndexedFaceSetCalculator::Generate3DMesh(std::reference_wrapper<const vrml_proc::parser::Int32Array> coordinateIndices, std::reference_wrapper<const vrml_proc::parser::Vec3fArray> coordinates, std::reference_wrapper<const bool> isConvex, const vrml_proc::math::TransformationMatrix& matrix) {

        vrml_proc::core::logger::LogInfo("Generate 3D mesh using IndexedFaceSetCalculator.", LOGGING_INFO);

        auto mesh = std::make_shared<core::Mesh>();
        auto error = std::make_shared<error::IndexedFaceSetCalculatorError>();

        const std::vector<int32_t>& indicides = coordinateIndices.get().integers;
        const std::vector<vrml_proc::parser::Vec3f>& points = coordinates.get().vectors;

        if (indicides.empty()) {
            vrml_proc::core::logger::LogWarning("No coordinate indices were found! There is nothing to calculate and empty mesh will be returned.", LOGGING_INFO);
            return mesh;
        }
        if (points.empty()) {
            return cpp::fail(error << (std::make_shared<error::PropertiesError>() << std::make_shared<vrml_proc::parser::model::validator::error::EmptyArrayError>("coordinateIndices")));
        }

        auto timer = vrmlproc::core::utils::ManualTimer();
        timer.Start();

        size_t start = 0; size_t end = 0;
        vrml_proc::core::utils::Range<int32_t> range(0, points.size() - 1);
        for (size_t i = 0; i < indicides.size(); ++i) {
            
            if (indicides[i] == -1) {

                if (end - start <= 2) {
                    return cpp::fail(error << (std::make_shared<error::PropertiesError>() <<
                        std::make_shared<error::InvalidNumberOfCoordinatesForFaceError>(end - start)));
                }

                if (end - start == 3) {

                    using NumberOutOfRangeError = vrml_proc::parser::model::validator::error::NumberOutOfRangeError<int32_t>;

                    if (!range.CheckValueInRangeInclusive(indicides[start])) {
                        return cpp::fail(std::make_shared<error::IndexedFaceSetCalculatorError>() <<
                            (std::make_shared<error::PropertiesError>() <<
                                (std::make_shared<error::VertexIndexOutOfRangeError>() <<
                                    std::make_shared<NumberOutOfRangeError>(range, indicides[start]))));
                    }
                    if (!range.CheckValueInRangeInclusive(indicides[start + 1])) {
                        return cpp::fail(std::make_shared<error::IndexedFaceSetCalculatorError>() <<
                            (std::make_shared<error::PropertiesError>() <<
                                (std::make_shared<error::VertexIndexOutOfRangeError>() <<
                                    std::make_shared<NumberOutOfRangeError>(range, indicides[start + 1]))));
                    }
                    if (!range.CheckValueInRangeInclusive(indicides[start + 2])) {
                        return cpp::fail(std::make_shared<error::IndexedFaceSetCalculatorError>() <<
                            (std::make_shared<error::PropertiesError>() <<
                                (std::make_shared<error::VertexIndexOutOfRangeError>() <<
                                    std::make_shared<NumberOutOfRangeError>(range, indicides[start + 2]))));
                    }

                    auto vertex1 = vrml_proc::math::cgal::CGALPoint(points[indicides[start]].x,
                        points[indicides[start]].y,
                        points[indicides[start]].z);

                    auto vertex2 = vrml_proc::math::cgal::CGALPoint(points[indicides[start + 1]].x,
                        points[indicides[start + 1]].y,
                        points[indicides[start + 1]].z);

                    auto vertex3 = vrml_proc::math::cgal::CGALPoint(points[indicides[start + 2]].x,
                        points[indicides[start + 2]].y,
                        points[indicides[start + 2]].z);

                    mesh->add_face(mesh->add_vertex(matrix.transform(vertex1)), mesh->add_vertex(matrix.transform(vertex2)), mesh->add_vertex(matrix.transform(vertex3)));
                }
                else {
                    return cpp::fail(std::make_shared<error::IndexedFaceSetCalculatorError>() <<
                        std::make_shared<vrml_proc::core::error::UnsupportedOperationError>("Face is constructed from more than 3 coordinates. IndexedFaceSetCalculator does not support fan triangulation or any other algorithms for such a task yet. Thus, mesh cannot be generated fully."));
                }

                start = end + 1;
            }

            end++;
        }

        double time = timer.End();
        vrml_proc::core::logger::LogInfo(vrml_proc::core::utils::FormatString("Mesh was generated successfully. The generation took ", time, " seconds."), LOGGING_INFO);

        return mesh;
	}
}

