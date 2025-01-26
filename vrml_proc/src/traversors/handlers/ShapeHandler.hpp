#pragma once

#include <memory>

#include <result.hpp>

#include "BaseConversionContext.hpp"
#include "ConversionContextActionMap.hpp"
#include "FullParsedVrmlNodeContext.hpp"
#include "Error.hpp"

#include "VrmlProcessingExport.hpp"

namespace vrml_proc {
	namespace traversor {
		namespace handler {
			namespace ShapeHandler {
				template<typename ConversionContext>
				VRMLPROCESSING_API cpp::result<std::shared_ptr<ConversionContext>, std::shared_ptr<vrml_proc::core::error::Error>> Handle(vrml_proc::traversor::FullParsedVrmlNodeContext context, const vrml_proc::action::ConversionContextActionMap<ConversionContext>& actionMap);
			}
		}
	}
}