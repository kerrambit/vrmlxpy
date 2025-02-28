#pragma once

#include "MeshTask.hpp"
#include "VectorConversionContext.hpp"

namespace to_stl::conversion_context {
  using MeshTaskConversionContext = vrml_proc::conversion_context::VectorConversionContext<to_stl::core::MeshTask>;
}
