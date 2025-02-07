#include "ColorHandler.hpp"

#include <memory>

#include <any>
#include <result.hpp>

#include "ConversionContextActionExecutor.hpp"
#include "ConversionContextActionMap.hpp"
#include "ColorNodeValidator.hpp"
#include "Error.hpp"
#include "Logger.hpp"
#include "FormatString.hpp"
#include "MeshConversionContext.hpp"
#include "NodeTraversorError.hpp"
#include "Vec3fArray.hpp"

#include "VrmlProcessingExport.hpp"

template<typename ConversionContext>
cpp::result<std::shared_ptr<ConversionContext>, std::shared_ptr<vrml_proc::core::error::Error>> vrml_proc::traversor::handler::ColorHandler::Handle(vrml_proc::traversor::FullParsedVrmlNodeContext context, const vrml_proc::action::ConversionContextActionMap<ConversionContext>& actionMap)
{
    vrml_proc::core::logger::LogInfo(vrml_proc::core::utils::FormatString("Handle VRML node <", context.node.header, ">."), LOGGING_INFO);

    vrml_proc::traversor::validator::ColorNodeValidator validator(context.node);
    auto validationResult = validator.Validate();
    if (validationResult.has_error()) {
        std::shared_ptr<vrml_proc::core::error::Error> error = std::make_shared<vrml_proc::traversor::error::NodeTraversorError>(validationResult.error(), context.node);
        return cpp::fail(error);
    }
    static vrml_proc::parser::Vec3fArray defaultColor;
    std::any cachedColor = validator.GetCachedColor(defaultColor);

    return vrml_proc::traversor::utils::ConversionContextActionExecutor::TryToExecute<ConversionContext>(actionMap, context.node.header, { std::cref(cachedColor) }, { });
}

namespace vrml_proc {
    namespace traversor {
        namespace handler {
            namespace ColorHandler {

                template VRMLPROCESSING_API cpp::result<std::shared_ptr<vrml_proc::conversion_context::MeshConversionContext>, std::shared_ptr<vrml_proc::core::error::Error>>
                    Handle<vrml_proc::conversion_context::MeshConversionContext>(
                        vrml_proc::traversor::FullParsedVrmlNodeContext,
                        const vrml_proc::action::ConversionContextActionMap<vrml_proc::conversion_context::MeshConversionContext>&);
            }
        }
    }
}