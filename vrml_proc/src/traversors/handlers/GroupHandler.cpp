#include "GroupHandler.hpp"

#include <memory>

#include <result.hpp>

#include "ConversionContextActionExecutor.hpp"
#include "ConversionContextActionMap.hpp"
#include "Error.hpp"
#include "FullParsedVrmlNodeContext.hpp"
#include "GroupNodeValidator.hpp"
#include "Logger.hpp"
#include "MeshConversionContext.hpp"
#include "NodeTraversorError.hpp"
#include "Vec3f.hpp"
#include "VrmlNodeTraversor.hpp"

#include "VrmlProcessingExport.hpp"

template<typename ConversionContext>
cpp::result<std::shared_ptr<ConversionContext>, std::shared_ptr<vrml_proc::core::error::Error>> vrml_proc::traversor::handler::GroupHandler::Handle(vrml_proc::traversor::FullParsedVrmlNodeContext context, const vrml_proc::action::ConversionContextActionMap<ConversionContext>& actionMap)
{
    LOG_INFO() << "Handle VRML node <" << context.node.header << ">.";

    vrml_proc::traversor::validator::GroupNodeValidator validator(context.node, context.manager);
    auto validationResult = validator.Validate();
    if (validationResult.has_error()) {
        std::shared_ptr<vrml_proc::core::error::Error> error = std::make_shared<vrml_proc::traversor::error::NodeTraversorError>(validationResult.error(), context.node);
        return cpp::fail(error);
    }

    std::vector<std::shared_ptr<ConversionContext>> resolvedChildren;
    for (const auto& child : validator.GetCachedChildren()) {
        auto recursiveResult = vrml_proc::traversor::VrmlNodeTraversor::Traverse<ConversionContext>({ child, context.manager, context.IsDescendantOfShape }, actionMap);
        if (recursiveResult.has_error()) {
            return cpp::fail(recursiveResult.error());
        }
        resolvedChildren.push_back(recursiveResult.value());
    }

    vrml_proc::parser::Vec3f defaultBoxCenter = { 0.0f, 0.0f, 0.0f };
    vrml_proc::parser::Vec3f defaultBoxSize = { -1.0f, -1.0f, -1.0f };
    return vrml_proc::traversor::utils::ConversionContextActionExecutor::TryToExecute<ConversionContext>(actionMap, "Group", { resolvedChildren, validator.GetCachedBoxCenter(defaultBoxCenter), validator.GetCachedBoxSize(defaultBoxSize) });
}

namespace vrml_proc {
    namespace traversor {
        namespace handler {
            namespace GroupHandler {

                template VRMLPROCESSING_API cpp::result<std::shared_ptr<vrml_proc::conversion_context::MeshConversionContext>, std::shared_ptr<vrml_proc::core::error::Error>>
                    Handle<vrml_proc::conversion_context::MeshConversionContext>(
                        vrml_proc::traversor::FullParsedVrmlNodeContext,
                        const vrml_proc::action::ConversionContextActionMap<vrml_proc::conversion_context::MeshConversionContext>&);
            }
        }
    }
}