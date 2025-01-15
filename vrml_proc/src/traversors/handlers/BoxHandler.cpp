#include "BoxHandler.hpp"

#include "NodeValidator.hpp"
#include "VrmlFieldExtractor.hpp"
#include "BaseConversionContextActionExecutor.hpp"
#include "MeshConversionContext.hpp"

std::shared_ptr<vrml_proc::conversion_context::BaseConversionContext> vrml_proc::traversor::handler::BoxHandler::Handle(vrml_proc::traversor::FullParsedVrmlNodeContext context, const vrml_proc::action::BaseConversionContextActionMap& actionMap)
{
	std::cout << "VRML Node - Box" << std::endl;

	if (context.node.fields.empty()) {
		// propagate error up
		// return
		return nullptr;
	}

	if (vrml_proc::traversor::validator::NodeValidator::CheckForOnlyUniqueAllowedFieldNames({ "size" }, context.node.fields).has_error()) {
		// propagate error up
		// return
		return nullptr;
	}

	vrml_proc::parser::Vec3f sizeValue = { 2.0f, 2.0f, 2.0f };
	auto size = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByName<vrml_proc::parser::Vec3f>("size", context.node.fields);
	if (size.has_value()) {
		std::cout << "size" << size.value() << std::endl;
		sizeValue = size.value();
	}
	if (size.error() == vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameError::ValidationError) {
		// Return error type, invalid field value
	}

	return vrml_proc::traversor::utils::BaseConversionContextActionExecutor::TryToExecute<vrml_proc::conversion_context::MeshConversionContext>(actionMap, "Box", { sizeValue, context.IsDescendantOfShape });
}
