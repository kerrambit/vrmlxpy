#include "WorldInfoHandler.hpp"

#include "VrmlFieldExtractor.hpp"
#include "BaseConversionContextActionExecutor.hpp"
#include "MeshConversionContext.hpp"

std::shared_ptr<vrml_proc::conversion_context::BaseConversionContext> vrml_proc::traversor::handler::WorldInfoHandler::Handle(vrml_proc::traversor::FullParsedVrmlNodeContext context, const vrml_proc::action::BaseConversionContextActionMap& actionMap)
{
	std::cout << "VRML Node - WorldInfo" << std::endl;

	auto info = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByName<std::string>("info", context.node.fields);
	if (info.has_value()) {
		std::cout << "info" << info.value() << std::endl;
	}
	if (info.error() == vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameError::ValidationError) {
		// Return error type, invalid field value
	}

	auto title = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByName<std::string>("title", context.node.fields);
	if (title.has_value()) {
		std::cout << "title" << title.value() << std::endl;
	}
	if (title.error() == vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameError::ValidationError) {
		// Return error type, invalid field value
	}

	return vrml_proc::traversor::utils::BaseConversionContextActionExecutor::TryToExecute<vrml_proc::conversion_context::MeshConversionContext>(actionMap, "WorldInfo", { info.value_or(""), title.value_or("") });
}
