#include "BoxAction.hpp"

#include <functional>
#include <memory>

#include "BoxCalculator.hpp"
#include "GeometryAction.hpp"
#include "Logger.hpp"
#include "MeshTaskConversionContext.hpp"
#include "TransformationMatrix.hpp"
#include "Vec3f.hpp"

namespace to_stl {
  namespace action {

    BoxAction::BoxAction(Properties properties, GeometryAction::Properties geometryProperties)
        : to_stl::action::GeometryAction(geometryProperties), m_size(properties.size) {}

    std::shared_ptr<to_stl::conversion_context::MeshTaskConversionContext> BoxAction::Execute() {
      vrml_proc::core::logger::LogInfo("Execute BoxAction.", LOGGING_INFO);

      auto result = std::make_shared<to_stl::conversion_context::MeshTaskConversionContext>();
      if (!m_geometryProperties.containedByShape) {
        vrml_proc::core::logger::LogDebug("Return empty data because Box node is not a child of a Shape node.",
                                          LOGGING_INFO);
        return result;
      }

      std::reference_wrapper<const vrml_proc::parser::Vec3f> size = m_size;
      vrml_proc::math::TransformationMatrix matrix = m_geometryProperties.matrix;

      result->Add([=]() {
        to_stl::calculator::BoxCalculator calculator = to_stl::calculator::BoxCalculator();
        return calculator.Generate3DMesh(size, matrix);
      });

      return result;
    }
  }  // namespace action
}  // namespace to_stl
