#pragma once

#include <any>
#include <result.hpp>

#include "ConversionContextActionExecutor.hpp"
#include "ConversionContextActionMap.hpp"
#include "Error.hpp"
#include "FormatString.hpp"
#include "FullParsedVrmlNodeContext.hpp"
#include "Logger.hpp"
#include "MeshConversionContext.hpp"
#include "NodeTraversorError.hpp"
#include "TextureCoordinateNodeValidator.hpp"
#include "Vec2fArray.hpp"

#include "VrmlProcessingExport.hpp"

namespace vrml_proc::traversor::handler::TextureCoordinateHandler {

	template<typename ConversionContext>
	VRMLPROCESSING_API inline cpp::result<std::shared_ptr<ConversionContext>, std::shared_ptr<vrml_proc::core::error::Error>> Handle(vrml_proc::traversor::FullParsedVrmlNodeContext context, const vrml_proc::action::ConversionContextActionMap<ConversionContext>& actionMap) {
		
		vrml_proc::core::logger::LogInfo(vrml_proc::core::utils::FormatString("Handle VRML node <", context.node.header, ">."), LOGGING_INFO);

		vrml_proc::traversor::validator::TextureCoordinateNodeValidator validator(context.node);
		auto validationResult = validator.Validate();
		if (validationResult.has_error()) {
			std::shared_ptr<vrml_proc::core::error::Error> error = std::make_shared<vrml_proc::traversor::error::NodeTraversorError>(validationResult.error(), context.node);
			return cpp::fail(error);
		}
		static vrml_proc::parser::Vec2fArray defaultPoint;
		std::any cachedPoint = validator.GetCachedPoint(defaultPoint);

		return vrml_proc::traversor::utils::ConversionContextActionExecutor::TryToExecute<ConversionContext>(actionMap, context.node.header, { std::cref(cachedPoint) }, { });
	}
}