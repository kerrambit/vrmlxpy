#pragma once

#include <memory>

#include <result.hpp>

#include "BaseConversionContext.hpp"
#include "ConversionContextActionMap.hpp"
#include "BoxHandler.hpp"
#include "FullParsedVrmlNodeContext.hpp"

#include "VrmlProcessingExport.hpp"

namespace vrml_proc {
	namespace traversor {
		namespace VrmlNodeTraversor {
			template<typename ConversionContext>
			VRMLPROCESSING_API std::shared_ptr<ConversionContext> Traverse(vrml_proc::traversor::FullParsedVrmlNodeContext context, const vrml_proc::action::ConversionContextActionMap<ConversionContext>& actionMap);
		}
	}
}