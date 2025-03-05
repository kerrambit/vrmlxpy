#pragma once

#include "MeshTask.hpp"
#include "VectorConversionContext.hpp"

namespace to_geom::conversion_context {
  using MeshTaskConversionContext = vrml_proc::conversion_context::VectorConversionContext<to_geom::core::MeshTask>;
}
