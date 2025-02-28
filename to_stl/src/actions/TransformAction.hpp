#pragma once

#include <memory>
#include <vector>

#include "ConversionContextAction.hpp"
#include "MeshTaskConversionContext.hpp"
#include "Vec3f.hpp"
#include "Vec4f.hpp"

#include "VrmlProcessingExport.hpp"

namespace to_stl {
  namespace action {

    class VRMLPROCESSING_API TransformAction
        : public vrml_proc::action::ConversionContextAction<to_stl::conversion_context::MeshTaskConversionContext> {
     public:
      struct TransformProperties {
        std::reference_wrapper<const vrml_proc::parser::Vec3f> center;
        std::reference_wrapper<const vrml_proc::parser::Vec4f> rotation;
        std::reference_wrapper<const vrml_proc::parser::Vec3f> scale;
        std::reference_wrapper<const vrml_proc::parser::Vec4f> scaleOrientation;
        std::reference_wrapper<const vrml_proc::parser::Vec3f> translation;
        std::reference_wrapper<const vrml_proc::parser::Vec3f> bboxCenter;
        std::reference_wrapper<const vrml_proc::parser::Vec3f> bboxSize;
        std::vector<std::shared_ptr<to_stl::conversion_context::MeshTaskConversionContext>> children;
      };

      TransformAction(TransformProperties properties);

      std::shared_ptr<to_stl::conversion_context::MeshTaskConversionContext> Execute() override;

     private:
      TransformProperties m_properties;
    };
  }  // namespace action
}  // namespace to_stl
