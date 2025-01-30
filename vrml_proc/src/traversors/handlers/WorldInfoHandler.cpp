#include "WorldInfoHandler.hpp"

#include <any>
#include <memory>
#include <string>

#include <result.hpp>

#include "ConversionContextActionExecutor.hpp"
#include "ConversionContextActionMap.hpp"
#include "Error.hpp"
#include "Logger.hpp"
#include "MeshConversionContext.hpp"
#include "NodeTraversorError.hpp"
#include "WorldInfoNodeValidator.hpp"

#include "VrmlProcessingExport.hpp"

template<typename ConversionContext>
cpp::result<std::shared_ptr<ConversionContext>, std::shared_ptr<vrml_proc::core::error::Error>> vrml_proc::traversor::handler::WorldInfoHandler::Handle(vrml_proc::traversor::FullParsedVrmlNodeContext context, const vrml_proc::action::ConversionContextActionMap<ConversionContext>& actionMap)
{
	LOG_INFO() << "Handle VRML node <" << context.node.header << ">.";

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

    return vrml_proc::traversor::utils::ConversionContextActionExecutor::TryToExecute<ConversionContext>(actionMap, "WorldInfo", { std::cref(cachedInfo), std::cref(cachedTitle) }, { context.IsDescendantOfShape });
}

namespace vrml_proc {
    namespace traversor {
        namespace handler {
            namespace WorldInfoHandler {

                template VRMLPROCESSING_API cpp::result<std::shared_ptr<vrml_proc::conversion_context::MeshConversionContext>, std::shared_ptr<vrml_proc::core::error::Error>>
                    Handle<vrml_proc::conversion_context::MeshConversionContext>(
                        vrml_proc::traversor::FullParsedVrmlNodeContext,
                        const vrml_proc::action::ConversionContextActionMap<vrml_proc::conversion_context::MeshConversionContext>&);
            }
        }
    }
}