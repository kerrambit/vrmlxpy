#pragma once

#include <memory>

#include <result.hpp>

#include "ConversionContextActionExecutor.hpp"
#include "ConversionContextActionMap.hpp"
#include "Error.hpp"
#include "FormatString.hpp"
#include "FullParsedVrmlNodeContext.hpp"
#include "Logger.hpp"
#include "NodeTraversorError.hpp"
#include "WorldInfoNodeValidator.hpp"

#include "VrmlProcessingExport.hpp"

namespace vrml_proc::traversor::handler::WorldInfoHandler {

	template<typename ConversionContext>
	VRMLPROCESSING_API inline cpp::result<std::shared_ptr<ConversionContext>, std::shared_ptr<vrml_proc::core::error::Error>> Handle(vrml_proc::traversor::FullParsedVrmlNodeContext context, const vrml_proc::action::ConversionContextActionMap<ConversionContext>& actionMap) {
		
		vrml_proc::core::logger::LogInfo(vrml_proc::core::utils::FormatString("Handle VRML node <", context.node.header, ">."), LOGGING_INFO);

		vrml_proc::traversor::validator::WorldInfoNodeValidator validator(context.node);
		auto validationResult = validator.Validate();
		if (validationResult.has_error()) {
			std::shared_ptr<vrml_proc::core::error::Error> error = std::make_shared<vrml_proc::traversor::error::NodeTraversorError>(validationResult.error(), context.node);
			return cpp::fail(error);
		}

		static std::string defaultInfo = "";
		std::any cachedInfo = validator.GetCachedInfo(defaultInfo);

		static std::string defaultTitle = "";
		std::any cachedTitle = validator.GetCachedInfo(defaultTitle);

		return vrml_proc::traversor::utils::ConversionContextActionExecutor::TryToExecute<ConversionContext>(actionMap, context.node.header, { std::cref(cachedInfo), std::cref(cachedTitle) }, { context.IsDescendantOfShape });
	}

	template<typename ConversionContext>
	VRMLPROCESSING_API inline cpp::result<std::shared_ptr<ConversionContext>, std::shared_ptr<vrml_proc::core::error::Error>> Handle(vrml_proc::traversor::FullParsedVrmlNodeContext context, const vrml_proc::action::ConversionContextActionMap<ConversionContext>& actionMap, const vrml_proc::traversor::node_descriptor::NodeDescriptor& nd) {

		vrml_proc::core::logger::LogInfo(vrml_proc::core::utils::FormatString("Handle VRML node <", context.node.header, ">."), LOGGING_INFO);

		std::any cachedInfo = nd.GetField<std::reference_wrapper<const std::string>>("info");
		std::any cachedTitle = nd.GetField<std::reference_wrapper<const std::string>>("title");

		return vrml_proc::traversor::utils::ConversionContextActionExecutor::TryToExecute<ConversionContext>(actionMap, nd.GetId(), {std::cref(cachedInfo), std::cref(cachedTitle)}, {context.IsDescendantOfShape});
	}
}