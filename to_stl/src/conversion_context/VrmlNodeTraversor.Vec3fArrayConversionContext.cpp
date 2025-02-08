#if __INCLUDE_LEVEL__
#error "Don't include this file"
#endif

#include "VrmlNodeTraversor.cpp"

#include <memory>

#include <result.hpp>

#include "ConversionContextActionMap.hpp"
#include "Error.hpp"
#include "FullParsedVrmlNodeContext.hpp"
#include "Vec3fArrayConversionContext.hpp"

namespace vrml_proc {
	namespace traversor {
		namespace VrmlNodeTraversor {
			template cpp::result<std::shared_ptr<to_stl::conversion_context::Vec3fArrayConversionContext>, std::shared_ptr<vrml_proc::core::error::Error>> Traverse<to_stl::conversion_context::Vec3fArrayConversionContext>(vrml_proc::traversor::FullParsedVrmlNodeContext context, const vrml_proc::action::ConversionContextActionMap<to_stl::conversion_context::Vec3fArrayConversionContext>& actionMap);
		}
	}
}