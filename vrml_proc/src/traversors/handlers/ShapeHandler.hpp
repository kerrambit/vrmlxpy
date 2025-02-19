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
#include "ShapeNodeValidator.hpp"
#include "VrmlNode.hpp"

#include "VrmlProcessingExport.hpp"

// Forward declaration.
namespace vrml_proc::traversor::VrmlNodeTraversor {
    template<typename ConversionContext>
    VRMLPROCESSING_API cpp::result<std::shared_ptr<ConversionContext>, std::shared_ptr<vrml_proc::core::error::Error>> Traverse(
        vrml_proc::traversor::FullParsedVrmlNodeContext context,
        const vrml_proc::action::ConversionContextActionMap<ConversionContext>& actionMap);
}

namespace vrml_proc::traversor::handler::ShapeHandler {

	template<typename ConversionContext>
	VRMLPROCESSING_API inline cpp::result<std::shared_ptr<ConversionContext>, std::shared_ptr<vrml_proc::core::error::Error>> Handle(vrml_proc::traversor::FullParsedVrmlNodeContext context, const vrml_proc::action::ConversionContextActionMap<ConversionContext>& actionMap) {
        
        vrml_proc::core::logger::LogInfo(vrml_proc::core::utils::FormatString("Handle VRML node <", context.node.header, ">."), LOGGING_INFO);

        vrml_proc::traversor::validator::ShapeNodeValidator validator(context.node, context.manager);
        auto validationResult = validator.Validate();
        if (validationResult.has_error()) {
            std::shared_ptr<vrml_proc::core::error::Error> error = std::make_shared<vrml_proc::traversor::error::NodeTraversorError>(validationResult.error(), context.node);
            return cpp::fail(error);
        }

        vrml_proc::parser::VrmlNode defaultAppearance;
        vrml_proc::parser::VrmlNode defaultGeometry;

        auto resolvedAppearance = vrml_proc::traversor::VrmlNodeTraversor::Traverse<ConversionContext>({ validator.GetCachedAppearance(defaultAppearance).get(), context.manager, true, context.transformation }, actionMap);
        if (resolvedAppearance.has_error()) {
            return cpp::fail(resolvedAppearance.error());
        }

        auto resolvedGeometry = vrml_proc::traversor::VrmlNodeTraversor::Traverse<ConversionContext>({ validator.GetCachedGeometry(defaultGeometry).get(), context.manager, true, context.transformation }, actionMap);
        if (resolvedGeometry.has_error()) {
            return cpp::fail(resolvedGeometry.error());
        }

        return vrml_proc::traversor::utils::ConversionContextActionExecutor::TryToExecute<ConversionContext>(actionMap, "Shape", {}, { resolvedAppearance.value(), resolvedGeometry.value() });
	}
}