#pragma once

#include "VectorConversionContext.hpp"
#include "StlBaseStructure.hpp"

namespace vrml_proc {
	namespace conversion_context {

		using MeshConversionContext = VectorConversionContext<StlBaseStructure>;
	}
}
