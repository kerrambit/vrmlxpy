#pragma once

#include <memory>

#include <result.hpp>

#include "ConversionContextActionExecutor.hpp"
#include "ConversionContextActionMap.hpp"
#include "Error.hpp"
#include "FormatString.hpp"
#include "FullParsedVrmlNodeContext.hpp"
#include "GroupNodeValidator.hpp"
#include "Logger.hpp"
#include "NodeTraversorError.hpp"
#include "Vec3f.hpp"

#include "VrmlProcessingExport.hpp"

namespace vrml_proc {
    namespace traversor {
        namespace VrmlNodeTraversor {
            template<typename ConversionContext>
            VRMLPROCESSING_API cpp::result<std::shared_ptr<ConversionContext>, std::shared_ptr<vrml_proc::core::error::Error>> Traverse(
                vrml_proc::traversor::FullParsedVrmlNodeContext context,
                const vrml_proc::action::ConversionContextActionMap<ConversionContext>& actionMap);
        }
    }
}


namespace vrml_proc::traversor::handler::GroupHandler {

	template<typename ConversionContext>
    VRMLPROCESSING_API inline cpp::result<std::shared_ptr<ConversionContext>, std::shared_ptr<vrml_proc::core::error::Error>> Handle(vrml_proc::traversor::FullParsedVrmlNodeContext context, const vrml_proc::action::ConversionContextActionMap<ConversionContext>& actionMap) {
        vrml_proc::core::logger::LogInfo(vrml_proc::core::utils::FormatString("Handle VRML node <", context.node.header, ">."), LOGGING_INFO);

        vrml_proc::traversor::validator::GroupNodeValidator validator(context.node, context.manager);
        auto validationResult = validator.Validate();
        if (validationResult.has_error()) {
            std::shared_ptr<vrml_proc::core::error::Error> error = std::make_shared<vrml_proc::traversor::error::NodeTraversorError>(validationResult.error(), context.node);
            return cpp::fail(error);
        }

        std::vector<std::shared_ptr<ConversionContext>> resolvedChildren;
        for (const auto& child : validator.GetCachedChildren()) {
            auto recursiveResult = vrml_proc::traversor::VrmlNodeTraversor::Traverse<ConversionContext>(
                { child, context.manager, context.IsDescendantOfShape, context.transformation }, actionMap);
            if (recursiveResult.has_error()) {
                return cpp::fail(recursiveResult.error());
            }
            resolvedChildren.push_back(recursiveResult.value());
        }

        static vrml_proc::parser::Vec3f defaultBoxCenter = { 0.0f, 0.0f, 0.0f };
        std::any cachedBoxCenter = validator.GetCachedBoxCenter(defaultBoxCenter);

        static vrml_proc::parser::Vec3f defaultBoxSize = { -1.0f, -1.0f, -1.0f };
        std::any cachedBoxSize = validator.GetCachedBoxSize(defaultBoxSize);

        return vrml_proc::traversor::utils::ConversionContextActionExecutor::TryToExecute<ConversionContext>(actionMap, "Group", { std::cref(cachedBoxCenter), std::cref(cachedBoxSize) }, { resolvedChildren });
	}
}