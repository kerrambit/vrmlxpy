#include "GroupHandler.hpp"

#include <vector>

#include <boost/variant.hpp>
#include <boost/variant/recursive_wrapper.hpp>

#include "BaseConversionContextActionExecutor.hpp"
#include "MeshConversionContext.hpp"
#include "NodeValidator.hpp"
#include "VrmlFieldExtractor.hpp"
#include "VrmlNodeTraversor.hpp"

std::shared_ptr<vrml_proc::conversion_context::BaseConversionContext> vrml_proc::traversor::handler::GroupHandler::Handle(vrml_proc::traversor::FullParsedVrmlNodeContext context, const vrml_proc::action::BaseConversionContextActionMap& actionMap)
{
	std::cout << "VRML Node - Group" << std::endl;

	auto result = std::make_shared<vrml_proc::conversion_context::MeshConversionContext>();

	if (context.node.fields.empty()) {
		return result;
	}

	if (vrml_proc::traversor::validator::NodeValidator::CheckForOnlyUniqueAllowedFieldNames({ "children", "bboxSize", "bboxCenter" }, context.node.fields).has_error()) {
		// propagate error up
		// return
		return nullptr;
	}

	std::vector<std::shared_ptr<vrml_proc::conversion_context::BaseConversionContext>> resolvedChildren;
	auto children = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByName<std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>, boost::recursive_wrapper<vrml_proc::parser::UseNode>>>>("children", context.node.fields);
	if (children.has_value()) {

		std::cout << "children " << std::endl;

		resolvedChildren.reserve(children.value().size());

		for (const auto& child : children.value()) {

			auto vrmlNode = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractFromVariant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>>(child);
			if (vrmlNode.has_value()) {
				std::cout << "VRML Node child " << vrmlNode.value().get() << std::endl;
				auto vrmlNodeTraversorResult = vrml_proc::traversor::VrmlNodeTraversor::Traverse({ vrmlNode.value().get(), context.manager }, actionMap);
				resolvedChildren.push_back(vrmlNodeTraversorResult);
			}

			auto useNode = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractFromVariant<boost::recursive_wrapper<vrml_proc::parser::UseNode>>(child);
			if (useNode.has_value()) {
				std::cout << "USE Node child " << useNode.value().get() << std::endl;
				auto managerFound = context.manager.GetDefinitionNode(useNode.value().get_pointer()->identifier);
				if (managerFound != nullptr) {
					// Manual dereferencing of managerFound shared pointer.
					auto useNodeTraversorResult = vrml_proc::traversor::VrmlNodeTraversor::Traverse({ *managerFound, context.manager }, actionMap); // TODO: manager must be thread-safe!
					resolvedChildren.push_back(useNodeTraversorResult);
				}
				else {
					// Return error type, invalid VRML file.
				}
			}
		}
	}
	if (children.error() == vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameError::ValidationError) {
		// Return error type, invalid field value
	}

	vrml_proc::parser::Vec3f bboxCentreValue = { 0.0f, 0.0f, 0.0f };
	auto bboxCentre = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByName<vrml_proc::parser::Vec3f>("bboxCenter", context.node.fields);
	if (bboxCentre.has_value()) {
		std::cout << "bboxCenter " << bboxCentre.value() << std::endl;
		vrml_proc::parser::Vec3f bboxCentreValue = bboxCentre.value();
	}
	if (bboxCentre.error() == vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameError::ValidationError) {
		// Return error type, invalid field value
	}

	vrml_proc::parser::Vec3f bboxSizeValue = { -1.0f, -1.0f, -1.0f };
	auto bboxSize = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByName<vrml_proc::parser::Vec3f>("bboxSize", context.node.fields);
	if (bboxSize.has_value()) {
		std::cout << "bboxSize " << bboxSize.value() << std::endl;
		vrml_proc::parser::Vec3f bboxSizeValue = bboxSize.value();
	}
	if (bboxSize.error() == vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameError::ValidationError) {
		// Return error type, invalid field value
	}

	return vrml_proc::traversor::utils::BaseConversionContextActionExecutor::TryToExecute<vrml_proc::conversion_context::MeshConversionContext>(
		actionMap, "Group", { resolvedChildren, bboxCentreValue, bboxSizeValue });
}
