#include "BoxHandler.hpp"

#include <memory>

#include "BaseConversionContext.hpp"
#include "BaseConversionContextActionExecutor.hpp"
#include "BoxValidator.hpp"
#include "MeshConversionContext.hpp"

std::shared_ptr<vrml_proc::conversion_context::BaseConversionContext> vrml_proc::traversor::handler::BoxHandler::Handle(vrml_proc::traversor::FullParsedVrmlNodeContext context, const vrml_proc::action::BaseConversionContextActionMap& actionMap)
{
	std::cout << "VRML Node - Box" << std::endl;
	
	vrml_proc::traversor::validator::BoxNodeValidator validator(context.node);
	if (validator.Validate().has_error()) {
		// propagate the error up
		return nullptr;
	}

	vrml_proc::parser::Vec3f defaultSize = { 2.0f, 2.0f, 2.0f };
	return vrml_proc::traversor::utils::BaseConversionContextActionExecutor::TryToExecute<vrml_proc::conversion_context::MeshConversionContext>(actionMap, "Box", { validator.GetCachedSize(defaultSize), context.IsDescendantOfShape});
}
