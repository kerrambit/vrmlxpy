#pragma once

#include <memory>
#include <vector>

#include <boost/variant.hpp>
#include <boost/variant/recursive_wrapper.hpp>

#include <result.hpp>

#include "VrmlNode.hpp"
#include "UseNode.hpp"
#include "VrmlNodeManager.hpp"
#include "VrmlFieldExtractor.hpp"
#include "BaseConversionContext.hpp"
#include "StlBaseStructure.hpp"
#include "BaseConversionContextActionMap.hpp"
#include "BaseConversionContextActionExecutor.hpp"
#include "VrmlFieldChecker.hpp"

// Forward declaration.
namespace vrml_proc {
	namespace traversor {
		struct FullParsedVrmlNodeContext;
	}
}

// Static helper functions.
static std::shared_ptr<vrml_proc::conversion_context::BaseConversionContext> HandleSpotlight(
	vrml_proc::traversor::FullParsedVrmlNodeContext context,
	const vrml_proc::action::BaseConversionContextActionMap& actionMap);

static std::shared_ptr<vrml_proc::conversion_context::BaseConversionContext> HandleWorldInfo(
	vrml_proc::traversor::FullParsedVrmlNodeContext context,
	const vrml_proc::action::BaseConversionContextActionMap& actionMap);

static std::shared_ptr<vrml_proc::conversion_context::BaseConversionContext> HandleGroup(
	vrml_proc::traversor::FullParsedVrmlNodeContext context,
	const vrml_proc::action::BaseConversionContextActionMap& actionMap);

static std::shared_ptr<vrml_proc::conversion_context::BaseConversionContext> HandleShape(
	vrml_proc::traversor::FullParsedVrmlNodeContext context,
	const vrml_proc::action::BaseConversionContextActionMap& actionMap);

static std::shared_ptr<vrml_proc::conversion_context::BaseConversionContext> HandleBox(
	vrml_proc::traversor::FullParsedVrmlNodeContext context,
	const vrml_proc::action::BaseConversionContextActionMap& actionMap);

namespace vrml_proc {
	namespace traversor {

		struct FullParsedVrmlNodeContext {

			FullParsedVrmlNodeContext(const vrml_proc::parser::VrmlNode& node, const vrml_proc::parser::VrmlNodeManager& manager)
				: node(node), manager(manager) {}

			const vrml_proc::parser::VrmlNode& node;
			const vrml_proc::parser::VrmlNodeManager& manager;
		};

		// tmp
		struct FieldNotFoundError {
			std::string field_name;
		};

		struct ValidationError {
			int error_code;
		};

		struct UnknownVrmlNodeError {
			std::string unknownVrmlNodeName;
			std::string node;
		};

		using ExtractError = boost::variant<FieldNotFoundError, ValidationError, UnknownVrmlNodeError>;

		using VrmlNodeTraverorResult = cpp::result<std::shared_ptr<vrml_proc::conversion_context::BaseConversionContext>, ExtractError>;
		// tmp

		class VrmlNodeTraversor {
		public:


			static std::shared_ptr<vrml_proc::conversion_context::BaseConversionContext> Traverse(FullParsedVrmlNodeContext context, const vrml_proc::action::BaseConversionContextActionMap& actionMap) {

				if (context.node.header == "WorldInfo") {
					return HandleWorldInfo(context, actionMap);
				}
				else if (context.node.header == "Group") {
					return HandleGroup(context, actionMap);
				}
				else if (context.node.header == "Shape") {
					return HandleShape(context, actionMap);
				}
				else if (context.node.header == "Box") {
					return HandleBox(context, actionMap);
				}
				else if (context.node.header == "Spotlight") {
					return HandleSpotlight(context, actionMap);
				}

				std::cout << "Error! Unknown VRML Node." << std::endl;
				return nullptr;
			}
		};
	}
}

std::shared_ptr<vrml_proc::conversion_context::BaseConversionContext> HandleGroup(vrml_proc::traversor::FullParsedVrmlNodeContext context, const vrml_proc::action::BaseConversionContextActionMap& actionMap) {
	
	std::cout << "VRML Node - Group" << std::endl;

	auto result = std::make_shared<vrml_proc::conversion_context::MeshConversionContext>();

	if (context.node.fields.empty()) {
		return result;
	}

	if (!vrml_proc::traversor::utils::VrmlFieldChecker::CheckFields({ "children", "bboxSize", "bboxCenter" }, context.node.fields)) {
		// error propagation
		return result;
	}

	std::vector<std::shared_ptr<vrml_proc::conversion_context::BaseConversionContext>> resolvedChildren;
	auto children = vrml_proc::traversor::utils::VrmlFieldExtractor::ExtractByName<std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>, boost::recursive_wrapper<vrml_proc::parser::UseNode>>>>("children", context.node.fields);
	if (children.has_value()) {

		std::cout << "children " << std::endl;

		resolvedChildren.reserve(children.value().size());

		for (const auto& child : children.value()) {

			auto vrmlNode = vrml_proc::traversor::utils::VrmlFieldExtractor::ExtractFromVariant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>>(child);
			if (vrmlNode.has_value()) {
				std::cout << "VRML Node child " << vrmlNode.value().get() << std::endl;
				auto vrmlNodeTraversorResult = vrml_proc::traversor::VrmlNodeTraversor::Traverse({ vrmlNode.value().get(), context.manager }, actionMap);
				resolvedChildren.push_back(vrmlNodeTraversorResult);
			}

			auto useNode = vrml_proc::traversor::utils::VrmlFieldExtractor::ExtractFromVariant<boost::recursive_wrapper<vrml_proc::parser::UseNode>>(child);
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
	if (children.error() == vrml_proc::traversor::utils::VrmlFieldExtractor::ExtractByNameError::ValidationError) {
		// Return error type, invalid field value
	}

	vrml_proc::parser::Vec3f bboxCentreValue = { 0.0f, 0.0f, 0.0f };
	auto bboxCentre = vrml_proc::traversor::utils::VrmlFieldExtractor::ExtractByName<vrml_proc::parser::Vec3f>("bboxCenter", context.node.fields);
	if (bboxCentre.has_value()) {
		std::cout << "bboxCenter " << bboxCentre.value() << std::endl;
		vrml_proc::parser::Vec3f bboxCentreValue = bboxCentre.value();
	}
	if (bboxCentre.error() == vrml_proc::traversor::utils::VrmlFieldExtractor::ExtractByNameError::ValidationError) {
		// Return error type, invalid field value
	}

	vrml_proc::parser::Vec3f bboxSizeValue = { -1.0f, -1.0f, -1.0f };
	auto bboxSize = vrml_proc::traversor::utils::VrmlFieldExtractor::ExtractByName<vrml_proc::parser::Vec3f>("bboxSize", context.node.fields);
	if (bboxSize.has_value()) {
		std::cout << "bboxSize " << bboxSize.value() << std::endl;
		vrml_proc::parser::Vec3f bboxSizeValue = bboxSize.value();
	}
	if (bboxSize.error() == vrml_proc::traversor::utils::VrmlFieldExtractor::ExtractByNameError::ValidationError) {
		// Return error type, invalid field value
	}

	return vrml_proc::traversor::utils::BaseConversionContextActionExecutor::TryToExecute<vrml_proc::conversion_context::MeshConversionContext>(
		actionMap, "Group", { resolvedChildren, bboxCentreValue, bboxSizeValue });
}

std::shared_ptr<vrml_proc::conversion_context::BaseConversionContext> HandleShape(vrml_proc::traversor::FullParsedVrmlNodeContext context, const vrml_proc::action::BaseConversionContextActionMap& actionMap) {
	std::cout << "VRML Node - Shape" << std::endl;

	auto result = std::make_shared<vrml_proc::conversion_context::MeshConversionContext>();

	if (context.node.fields.empty()) {
		return result;
	}

	if (!vrml_proc::traversor::utils::VrmlFieldChecker::CheckFields({ "appearance", "geometry" }, context.node.fields)) {
		// error propagation
		return result;
	}

	vrml_proc::parser::VrmlNode appearance;
	auto appearanceResult = vrml_proc::traversor::utils::VrmlFieldExtractor::ExtractVrmlNode("appearance", context.node.fields, context.manager);
	if (appearanceResult.has_value()) {
		// <check valid node names>
		// 
		// </check valid node names>
	}
	if (appearanceResult.error() == vrml_proc::traversor::utils::VrmlFieldExtractor::ExtractVrmlNodeError::ValidationError) {
		// Error propagation and end.
	}

	auto apperanceTraversorResult = vrml_proc::traversor::VrmlNodeTraversor::Traverse({ appearance, context.manager }, actionMap);

	vrml_proc::parser::VrmlNode geometry;
	auto geometryResult = vrml_proc::traversor::utils::VrmlFieldExtractor::ExtractVrmlNode("geometry", context.node.fields, context.manager);
	if (geometryResult.has_value()) {
		// <check valid node names>
		// 
		// </check valid node names>
	}
	if (geometryResult.error() == vrml_proc::traversor::utils::VrmlFieldExtractor::ExtractVrmlNodeError::ValidationError) {
		// Error propagation and end.
	}

	auto geometryTraversorResult = vrml_proc::traversor::VrmlNodeTraversor::Traverse({ geometry, context.manager }, actionMap);

	return vrml_proc::traversor::utils::BaseConversionContextActionExecutor::TryToExecute<vrml_proc::conversion_context::MeshConversionContext>(actionMap, "Shape", { apperanceTraversorResult, geometryTraversorResult });
}

std::shared_ptr<vrml_proc::conversion_context::BaseConversionContext> HandleBox(vrml_proc::traversor::FullParsedVrmlNodeContext context, const vrml_proc::action::BaseConversionContextActionMap& actionMap) {
	std::cout << "VRML Node - Box" << std::endl;

	auto result = std::make_shared<vrml_proc::conversion_context::MeshConversionContext>();

	if (context.node.fields.empty()) {
		return result;
	}

	if (!vrml_proc::traversor::utils::VrmlFieldChecker::CheckFields({ "size" }, context.node.fields)) {
		// error propagation
	}

	vrml_proc::parser::Vec3f sizeValue = { 2.0f, 2.0f, 2.0f };
	auto size = vrml_proc::traversor::utils::VrmlFieldExtractor::ExtractByName<vrml_proc::parser::Vec3f>("size", context.node.fields);
	if (size.has_value()) {
		std::cout << "size" << size.value() << std::endl;
		sizeValue = size.value();
	}
	if (size.error() == vrml_proc::traversor::utils::VrmlFieldExtractor::ExtractByNameError::ValidationError) {
		// Return error type, invalid field value
	}

	return vrml_proc::traversor::utils::BaseConversionContextActionExecutor::TryToExecute<vrml_proc::conversion_context::MeshConversionContext>(actionMap, "Box", { sizeValue });
}

std::shared_ptr<vrml_proc::conversion_context::BaseConversionContext> HandleSpotlight(vrml_proc::traversor::FullParsedVrmlNodeContext context, const vrml_proc::action::BaseConversionContextActionMap& actionMap) {
	std::cout << "VRML Node - Spotlight" << std::endl;

	auto ambientIntensity = vrml_proc::traversor::utils::VrmlFieldExtractor::ExtractByName<float>("ambientIntensity", context.node.fields);
	if (ambientIntensity.has_value()) {
		std::cout << "ambientIntensity" << ambientIntensity.value() << std::endl;
	}
	if (ambientIntensity.error() == vrml_proc::traversor::utils::VrmlFieldExtractor::ExtractByNameError::ValidationError) {
		// Return error type, invalid field value
	}

	return vrml_proc::traversor::utils::BaseConversionContextActionExecutor::TryToExecute<vrml_proc::conversion_context::MeshConversionContext>(actionMap, "Spotlight", { ambientIntensity.value_or(0.0f) });
}

std::shared_ptr<vrml_proc::conversion_context::BaseConversionContext> HandleWorldInfo(vrml_proc::traversor::FullParsedVrmlNodeContext context, const vrml_proc::action::BaseConversionContextActionMap& actionMap) {
	std::cout << "VRML Node - WorldInfo" << std::endl;

	auto info = vrml_proc::traversor::utils::VrmlFieldExtractor::ExtractByName<std::string>("info", context.node.fields);
	if (info.has_value()) {
		std::cout << "info" << info.value() << std::endl;
	}
	if (info.error() == vrml_proc::traversor::utils::VrmlFieldExtractor::ExtractByNameError::ValidationError) {
		// Return error type, invalid field value
	}

	auto title = vrml_proc::traversor::utils::VrmlFieldExtractor::ExtractByName<std::string>("title", context.node.fields);
	if (title.has_value()) {
		std::cout << "title" << title.value() << std::endl;
	}
	if (title.error() == vrml_proc::traversor::utils::VrmlFieldExtractor::ExtractByNameError::ValidationError) {
		// Return error type, invalid field value
	}

	return vrml_proc::traversor::utils::BaseConversionContextActionExecutor::TryToExecute<vrml_proc::conversion_context::MeshConversionContext>(actionMap, "WorldInfo", { info.value_or(""), title.value_or("") });
}
