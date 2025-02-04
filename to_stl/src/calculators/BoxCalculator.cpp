#include "BoxCalculator.hpp"

#include <memory>

#include <CGAL/Kernel/interface_macros.h>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Surface_mesh/Surface_mesh.h>

#include <result.hpp>

#include "BoxAction.hpp"
#include "CalculatorError.hpp"
#include "Error.hpp"
#include "Mesh.hpp"
#include "ModelValidator.hpp"

typedef CGAL::Simple_cartesian<double> Kernel;
typedef Kernel::Point_3 Point;
typedef CGAL::Surface_mesh<Point> Mesh;

namespace to_stl {
	namespace calculator {
		cpp::result<std::shared_ptr<core::Mesh>, std::shared_ptr<vrml_proc::core::error::Error>> BoxCalculator::Generate3DMesh(const to_stl::action::BoxAction::BoxProperties& properties) {

            auto checkResult = vrml_proc::parser::model::validator::CheckVec3fIsGreaterThanZero(properties.size.get());
            if (checkResult.has_error()) {
                return cpp::fail(std::make_shared<error::BoxCalculatorError>() << (std::make_shared<error::PropertiesError>() << checkResult.error()));
            }

            auto mesh = std::make_shared<core::Mesh>();

            double half_x = properties.size.get().x / 2.0;
            double half_y = properties.size.get().y / 2.0;
            double half_z = properties.size.get().z / 2.0;

            Point vertices[8] = {
                Point(-half_x, -half_y, -half_z),
                Point(half_x, -half_y, -half_z),
                Point(half_x,  half_y, -half_z),
                Point(-half_x,  half_y, -half_z),
                Point(-half_x, -half_y,  half_z),
                Point(half_x, -half_y,  half_z),
                Point(half_x,  half_y,  half_z),
                Point(-half_x,  half_y,  half_z)
            };

            Mesh::Vertex_index v[8];
            for (int i = 0; i < 8; ++i) {
                v[i] = mesh->add_vertex(vertices[i]);
            }

            // Front face.
            mesh->add_face(v[4], v[5], v[6]);
            mesh->add_face(v[4], v[6], v[7]);

            // Back face.
            mesh->add_face(v[1], v[0], v[3]);
            mesh->add_face(v[1], v[3], v[2]);

            // Top face.
            mesh->add_face(v[7], v[6], v[2]);
            mesh->add_face(v[7], v[2], v[3]);

            // Bottom face.
            mesh->add_face(v[0], v[1], v[5]);
            mesh->add_face(v[0], v[5], v[4]);

            // Right face.
            mesh->add_face(v[5], v[1], v[2]);
            mesh->add_face(v[5], v[2], v[6]);

            // Left face.
            mesh->add_face(v[0], v[4], v[7]);
            mesh->add_face(v[0], v[7], v[3]);

            return mesh;
		}
	}
}

