#include "BoxHandler.hpp"

#include <memory>

#include <any>
#include <result.hpp>

#include "BoxNodeValidator.hpp"
#include "ConversionContextActionExecutor.hpp"
#include "ConversionContextActionMap.hpp"
#include "Error.hpp"
#include "Logger.hpp"
#include "MeshConversionContext.hpp"
#include "NodeTraversorError.hpp"
#include "Vec3f.hpp"

#include "VrmlProcessingExport.hpp"

template<typename ConversionContext>
cpp::result<std::shared_ptr<ConversionContext>, std::shared_ptr<vrml_proc::core::error::Error>> vrml_proc::traversor::handler::BoxHandler::Handle(vrml_proc::traversor::FullParsedVrmlNodeContext context, const vrml_proc::action::ConversionContextActionMap<ConversionContext>& actionMap)
{
    LOG_INFO() << "Handle VRML node <" << context.node.header << ">.";
	
	vrml_proc::traversor::validator::BoxNodeValidator validator(context.node);
    auto validationResult = validator.Validate();
	if (validationResult.has_error()) {
        std::shared_ptr<vrml_proc::core::error::Error> error = std::make_shared<vrml_proc::traversor::error::NodeTraversorError>(validationResult.error(), context.node);
        return cpp::fail(error);
	}
	static vrml_proc::parser::Vec3f defaultSize = { 2.0f, 2.0f, 2.0f };

    std::cout << "Handler (1): " << (validator.GetCachedSize(defaultSize)).get() << std::endl;

    std::any cachedSize = validator.GetCachedSize(defaultSize);

    std::cout << "Handler (2): "
        << (std::any_cast<std::reference_wrapper<const vrml_proc::parser::Vec3f>>(cachedSize)).get()
        << std::endl;

    return vrml_proc::traversor::utils::ConversionContextActionExecutor::TryToExecute<ConversionContext>(actionMap, "Box", { std::cref(cachedSize) }, {context.IsDescendantOfShape});
}

namespace vrml_proc {
    namespace traversor {
        namespace handler {
            namespace BoxHandler {

                template VRMLPROCESSING_API cpp::result<std::shared_ptr<vrml_proc::conversion_context::MeshConversionContext>, std::shared_ptr<vrml_proc::core::error::Error>>
                    Handle<vrml_proc::conversion_context::MeshConversionContext>(
                        vrml_proc::traversor::FullParsedVrmlNodeContext,
                        const vrml_proc::action::ConversionContextActionMap<vrml_proc::conversion_context::MeshConversionContext>&);
            }
        }
    }
}