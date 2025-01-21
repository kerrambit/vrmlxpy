#pragma once

#include <memory>

#include "BaseConversionContext.hpp"
#include "BaseConversionContextActionMap.hpp"
#include "FullParsedVrmlNodeContext.hpp"

namespace vrml_proc {
	namespace traversor {
		namespace handler {
			namespace ShapeHandler {
				template<typename ConversionContext>
				std::shared_ptr<ConversionContext> Handle(vrml_proc::traversor::FullParsedVrmlNodeContext context, const vrml_proc::action::BaseConversionContextActionMap<ConversionContext>& actionMap);
			}
		}
	}
}