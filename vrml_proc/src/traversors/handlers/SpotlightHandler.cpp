#include "SpotlightHandler.hpp"

#include "VrmlFieldExtractor.hpp"
#include "MeshConversionContext.hpp"
#include "BaseConversionContextActionExecutor.hpp"

std::shared_ptr<vrml_proc::conversion_context::BaseConversionContext> vrml_proc::traversor::handler::SpotlightHandler::Handle(vrml_proc::traversor::FullParsedVrmlNodeContext context, const vrml_proc::action::BaseConversionContextActionMap& actionMap)
{
	std::cout << "VRML Node - Spotlight" << std::endl;

	auto ambientIntensity = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByName<float>("ambientIntensity", context.node.fields);
	if (ambientIntensity.has_value()) {
		std::cout << "ambientIntensity" << ambientIntensity.value() << std::endl;
	}
	if (ambientIntensity.error() == vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameError::ValidationError) {
		// Return error type, invalid field value
	}

	return vrml_proc::traversor::utils::BaseConversionContextActionExecutor::TryToExecute<vrml_proc::conversion_context::MeshConversionContext>(actionMap, "Spotlight", { ambientIntensity.value_or(0.0f) });
}
