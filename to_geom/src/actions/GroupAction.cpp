#include "GroupAction.hpp"

#include <memory>
#include <vector>

#include "Logger.hpp"
#include "MeshTaskConversionContext.hpp"
#include "Vec3f.hpp"

namespace to_geom {
  namespace action {

    GroupAction::GroupAction(
        std::vector<std::shared_ptr<to_geom::conversion_context::MeshTaskConversionContext>> children,
        std::reference_wrapper<const vrml_proc::parser::Vec3f> bboxCenter,
        std::reference_wrapper<const vrml_proc::parser::Vec3f> bboxSize)
        : m_children(children), m_bboxCenter(bboxCenter), m_bboxSize(bboxSize) {}

    std::shared_ptr<to_geom::conversion_context::MeshTaskConversionContext> GroupAction::Execute() {
      vrml_proc::core::logger::LogInfo("Execute GroupAction.", LOGGING_INFO);

      auto result = std::make_shared<to_geom::conversion_context::MeshTaskConversionContext>();
      for (const auto& child : m_children) {
        if (child != nullptr) {
          std::shared_ptr<to_geom::conversion_context::MeshTaskConversionContext> meshContextPtr =
              std::dynamic_pointer_cast<to_geom::conversion_context::MeshTaskConversionContext>(child);
          if (meshContextPtr != nullptr) {
            result->Merge(meshContextPtr.get());
            meshContextPtr.reset();
          }
        }
      }

      return result;
    }
  }  // namespace action
}  // namespace to_geom
