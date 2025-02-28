#pragma once

#include "VectorConversionContext.hpp"
#include "StlBaseStructure.hpp"

namespace vrml_proc {
  namespace conversion_context {
    /**
     * @brief ConversionContext type where the internal data structure is a list of `StlBaseStrcture` objects.
     * It is a code representation of STL file.
     */
    using MeshConversionContext = VectorConversionContext<StlBaseStructure>;
  }  // namespace conversion_context
}  // namespace vrml_proc
