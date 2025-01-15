#pragma once

#include <memory>

#include <result.hpp>

#include "FullParsedVrmlNodeContext.hpp"
#include "BaseConversionContext.hpp"
#include "BaseConversionContextActionMap.hpp"

#include "VrmlProcessingExport.hpp"

namespace vrml_proc {
	namespace traversor {
		namespace VrmlNodeTraversor {
			VRMLPROCESSING_API std::shared_ptr<vrml_proc::conversion_context::BaseConversionContext> Traverse(vrml_proc::traversor::FullParsedVrmlNodeContext context, const vrml_proc::action::BaseConversionContextActionMap& actionMap);
		}
	}
}