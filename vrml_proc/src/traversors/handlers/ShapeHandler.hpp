#pragma once

#include <memory>

#include "BaseConversionContext.hpp"
#include "BaseConversionContextActionMap.hpp"
#include "FullParsedVrmlNodeContext.hpp"

namespace vrml_proc {
	namespace traversor {
		namespace handler {
			namespace ShapeHandler {
				std::shared_ptr<vrml_proc::conversion_context::BaseConversionContext> Handle(vrml_proc::traversor::FullParsedVrmlNodeContext context, const vrml_proc::action::BaseConversionContextActionMap& actionMap);
			}
		}
	}
}