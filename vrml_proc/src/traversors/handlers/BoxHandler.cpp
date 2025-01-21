#include "BoxHandler.hpp"

#include <memory>

#include "BaseConversionContext.hpp"
#include "ConversionContextActionExecutor.hpp"
#include "BoxNodeValidator.hpp"
#include "MeshConversionContext.hpp"

template<typename ConversionContext>
std::shared_ptr<ConversionContext> vrml_proc::traversor::handler::BoxHandler::Handle(vrml_proc::traversor::FullParsedVrmlNodeContext context, const vrml_proc::action::ConversionContextActionMap<ConversionContext>& actionMap)
{
	std::cout << "VRML Node - Box" << std::endl;
	
	vrml_proc::traversor::validator::BoxNodeValidator validator(context.node);
	if (validator.Validate().has_error()) {
		// propagate the error up
		return nullptr;
	}

	vrml_proc::parser::Vec3f defaultSize = { 2.0f, 2.0f, 2.0f };
	return vrml_proc::traversor::utils::ConversionContextActionExecutor::TryToExecute<ConversionContext>(actionMap, "Box", { validator.GetCachedSize(defaultSize), context.IsDescendantOfShape});
}

namespace vrml_proc {
    namespace traversor {
        namespace handler {
            namespace BoxHandler {

                template VRMLPROCESSING_API std::shared_ptr<vrml_proc::conversion_context::MeshConversionContext>
                    Handle<vrml_proc::conversion_context::MeshConversionContext>(
                        vrml_proc::traversor::FullParsedVrmlNodeContext,
                        const vrml_proc::action::ConversionContextActionMap<vrml_proc::conversion_context::MeshConversionContext>&);
            }
        }
    }
}