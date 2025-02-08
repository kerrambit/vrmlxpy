#pragma once

#include <memory>

#include <any>
#include <result.hpp>

#include "ConversionContextActionExecutor.hpp"
#include "ConversionContextActionMap.hpp"
#include "Error.hpp"
#include "Logger.hpp"
#include "MeshConversionContext.hpp"
#include "NodeTraversorError.hpp"
#include "NormalNodeValidator.hpp"
#include "Vec3fArray.hpp"
#include "FullParsedVrmlNodeContext.hpp"

#include "VrmlProcessingExport.hpp"

namespace vrml_proc {
	namespace traversor {
		namespace handler {
			namespace NormalHandler {
				template<typename ConversionContext>
				VRMLPROCESSING_API cpp::result<std::shared_ptr<ConversionContext>, std::shared_ptr<vrml_proc::core::error::Error>> Handle(vrml_proc::traversor::FullParsedVrmlNodeContext context, const vrml_proc::action::ConversionContextActionMap<ConversionContext>& actionMap);
			}
		}
	}
}