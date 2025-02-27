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
#include "NodeDescriptor.hpp"

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
            auto recursiveResult = vrml_proc::traversor::VrmlNodeTraversor::Traverse<ConversionContext>({ validator.GetCachedChoice().at(whichChoice), context.manager, context.IsDescendantOfShape, context.transformation, context.config }, actionMap);
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

    template<typename ConversionContext>
    VRMLPROCESSING_API inline cpp::result<std::shared_ptr<ConversionContext>, std::shared_ptr<vrml_proc::core::error::Error>> Handle(vrml_proc::traversor::FullParsedVrmlNodeContext context, const vrml_proc::action::ConversionContextActionMap<ConversionContext>& actionMap, const vrml_proc::traversor::node_descriptor::NodeDescriptor& nd) {

        vrml_proc::core::logger::LogInfo(vrml_proc::core::utils::FormatString("Handle VRML node <", context.node.header, ">."), LOGGING_INFO);

        const int32_t& whichChoice = (nd.GetField<std::reference_wrapper<const int32_t>>("whichChoice")).get();

        std::shared_ptr<ConversionContext> resolvedChild = std::make_shared<ConversionContext>();
        if (whichChoice >= 0 && nd.GetField<std::vector<std::reference_wrapper<const vrml_proc::parser::VrmlNode>>>("choice").size() != 0 && whichChoice <= nd.GetField<std::vector<std::reference_wrapper<const vrml_proc::parser::VrmlNode>>>("choice").size() - 1) {
            auto recursiveResult = vrml_proc::traversor::VrmlNodeTraversor::Traverse<ConversionContext>({ nd.GetField<std::vector<std::reference_wrapper<const vrml_proc::parser::VrmlNode>>>("choice").at(whichChoice), context.manager, context.IsDescendantOfShape, context.transformation }, actionMap);
            if (recursiveResult.has_error()) {
                return cpp::fail(recursiveResult.error());
            }

            resolvedChild = recursiveResult.value();
        }
        else {
            vrml_proc::core::logger::LogWarning(vrml_proc::core::utils::FormatString("Skipping traversing VRML node in 'choice' field, as 'whichChoice' value is <", whichChoice, "> and is out of range!"), LOGGING_INFO);
        }

        return vrml_proc::traversor::utils::ConversionContextActionExecutor::TryToExecute<ConversionContext>(actionMap, nd.GetId(), {}, { resolvedChild });
    }
}