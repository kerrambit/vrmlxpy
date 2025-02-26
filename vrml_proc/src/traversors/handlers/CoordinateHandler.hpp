#pragma once

#include <memory>

#include <result.hpp>

#include "ConversionContextActionExecutor.hpp"
#include "ConversionContextActionMap.hpp"
#include "CoordinateNodeValidator.hpp"
#include "Error.hpp"
#include "FormatString.hpp"
#include "FullParsedVrmlNodeContext.hpp"
#include "Logger.hpp"
#include "NodeTraversorError.hpp"
#include "Vec3fArray.hpp"
#include "NodeDescriptor.hpp"

#include "VrmlProcessingExport.hpp"

namespace vrml_proc::traversor::handler::CoordinateHandler {

	template<typename ConversionContext>
	VRMLPROCESSING_API inline cpp::result<std::shared_ptr<ConversionContext>, std::shared_ptr<vrml_proc::core::error::Error>> Handle(vrml_proc::traversor::FullParsedVrmlNodeContext context, const vrml_proc::action::ConversionContextActionMap<ConversionContext>& actionMap) {
		vrml_proc::core::logger::LogInfo(vrml_proc::core::utils::FormatString("Handle VRML node <", context.node.header, ">."), LOGGING_INFO);

		vrml_proc::traversor::validator::CoordinateNodeValidator validator(context.node);
		auto validationResult = validator.Validate();
		if (validationResult.has_error()) {
			std::shared_ptr<vrml_proc::core::error::Error> error = std::make_shared<vrml_proc::traversor::error::NodeTraversorError>(validationResult.error(), context.node);
			return cpp::fail(error);
		}
		static vrml_proc::parser::Vec3fArray defaultPoint;
		std::any cachedPoint = validator.GetCachedPoint(defaultPoint);

		return vrml_proc::traversor::utils::ConversionContextActionExecutor::TryToExecute<ConversionContext>(actionMap, context.node.header, { std::cref(cachedPoint) }, { });
	}

	template<typename ConversionContext>
	VRMLPROCESSING_API inline cpp::result<std::shared_ptr<ConversionContext>, std::shared_ptr<vrml_proc::core::error::Error>> Handle(vrml_proc::traversor::FullParsedVrmlNodeContext context, const vrml_proc::action::ConversionContextActionMap<ConversionContext>& actionMap, const vrml_proc::traversor::node_descriptor::NodeDescriptor& nd) {
		
		vrml_proc::core::logger::LogInfo(vrml_proc::core::utils::FormatString("Handle VRML node <", context.node.header, ">."), LOGGING_INFO);

		std::any cachedPoint = nd.GetField<std::reference_wrapper<vrml_proc::parser::Vec3fArray>>("point");
		return vrml_proc::traversor::utils::ConversionContextActionExecutor::TryToExecute<ConversionContext>(actionMap, nd.GetId(), {std::cref(cachedPoint)}, {});
	}
}