#pragma once

#include <memory>

#include "GeometryAction.hpp"
#include "MeshTaskConversionContext.hpp"
#include "Vec3f.hpp"

#include "VrmlProcessingExport.hpp"

namespace to_geom {
  namespace action {

    class VRMLPROCESSING_API BoxAction : public to_geom::action::GeometryAction {
     public:
      struct Properties {
        std::reference_wrapper<const vrml_proc::parser::Vec3f> size;
      };

      BoxAction(Properties properties, GeometryAction::Properties geometryProperties);
      std::shared_ptr<to_geom::conversion_context::MeshTaskConversionContext> Execute() override;

     private:
      std::reference_wrapper<const vrml_proc::parser::Vec3f> m_size;
    };
  }  // namespace action
}  // namespace to_geom
