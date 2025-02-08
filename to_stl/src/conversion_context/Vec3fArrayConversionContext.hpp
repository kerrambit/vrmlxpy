#pragma once

#include <functional>

#include "Vec3fArray.hpp"
#include "VectorConversionContext.hpp"

namespace to_stl::conversion_context {

	using Vec3fArrayConversionContext = vrml_proc::conversion_context::VectorConversionContext<std::reference_wrapper<const vrml_proc::parser::Vec3fArray>>;
}
