#pragma once

#include <cstdint>
#include <memory>

#include <result.hpp>

#include "ConversionContextActionExecutor.hpp"
#include "ConversionContextActionMap.hpp"
#include "Error.hpp"
#include "FormatString.hpp"
#include "FullParsedVrmlNodeContext.hpp"
#include "Logger.hpp"
#include "NodeTraversorError.hpp"
#include "SwitchNodeValidator.hpp"
#include "VrmlNodeTraversor.hpp"

#include "VrmlProcessingExport.hpp"

namespace vrml_proc::traversor::handler::SwitchHandler {

	template<typename ConversionContext>
    VRMLPROCESSING_API inline cpp::result<std::shared_ptr<ConversionContext>, std::shared_ptr<vrml_proc::core::error::Error>> Handle(vrml_proc::traversor::FullParsedVrmlNodeContext context, const vrml_proc::action::ConversionContextActionMap<ConversionContext>& actionMap) {

        vrml_proc::core::logger::LogInfo(vrml_proc::core::utils::FormatString("Handle VRML node <", context.node.header, ">."), LOGGING_INFO);

        vrml_proc::traversor::validator::SwitchNodeValidator validator(context.node, context.manager);
        auto validationResult = validator.Validate();
        if (validationResult.has_error()) {
            std::shared_ptr<vrml_proc::core::error::Error> error = std::make_shared<vrml_proc::traversor::error::NodeTraversorError>(validationResult.error(), context.node);
            return cpp::fail(error);
        }

        static int32_t defaultWhichChoice = -1;
        const int32_t& whichChoice = (validator.GetCachedWhichChoice(defaultWhichChoice)).get();

        std::shared_ptr<ConversionContext> resolvedChild = std::make_shared<ConversionContext>();
        if (whichChoice >= 0 && validator.GetCachedChoice().size() != 0 && whichChoice <= validator.GetCachedChoice().size() - 1) {
            auto recursiveResult = vrml_proc::traversor::VrmlNodeTraversor::Traverse<ConversionContext>({ validator.GetCachedChoice().at(whichChoice), context.manager, context.IsDescendantOfShape, context.transformation}, actionMap);
            if (recursiveResult.has_error()) {
                return cpp::fail(recursiveResult.error());
            }

            resolvedChild = recursiveResult.value();
        }
        else {
            vrml_proc::core::logger::LogWarning(vrml_proc::core::utils::FormatString("Skipping traversing VRML node in 'choice' field, as 'whichChoice' value is <", whichChoice, "> and is out of range!"), LOGGING_INFO);
        }

        return vrml_proc::traversor::utils::ConversionContextActionExecutor::TryToExecute<ConversionContext>(actionMap, context.node.header, { }, { resolvedChild });
	}
}