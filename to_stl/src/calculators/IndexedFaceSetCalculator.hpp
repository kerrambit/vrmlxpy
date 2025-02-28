#pragma once

#include <memory>

#include <result.hpp>

#include "Error.hpp"
#include "IndexedFaceSetAction.hpp"
#include "Mesh.hpp"
#include "TransformationMatrix.hpp"

#include "VrmlToStlExport.hpp"

namespace to_stl::calculator {
  class VRMLTOSTL_API IndexedFaceSetCalculator {
   public:
    cpp::result<std::shared_ptr<core::Mesh>, std::shared_ptr<vrml_proc::core::error::Error>> Generate3DMesh(
        std::reference_wrapper<const vrml_proc::parser::Int32Array> coordinateIndices,
        std::reference_wrapper<const vrml_proc::parser::Vec3fArray> coordinates,
        std::reference_wrapper<const bool> isConvex, const vrml_proc::math::TransformationMatrix& matrix);
  };
}  // namespace to_stl::calculator
