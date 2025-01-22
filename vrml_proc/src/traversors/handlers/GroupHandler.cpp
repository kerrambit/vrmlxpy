#include "GroupHandler.hpp"

#include <memory>
#include <vector>

#include <boost/variant.hpp>
#include <boost/variant/recursive_wrapper.hpp>

#include "ConversionContextActionExecutor.hpp"
#include "GroupNodeValidator.hpp"
#include "MeshConversionContext.hpp"
#include "NodeValidator.hpp"
#include "VrmlFieldExtractor.hpp"
#include "VrmlNode.hpp"
#include "VrmlNodeTraversor.hpp"

template<typename ConversionContext>
std::shared_ptr<ConversionContext> vrml_proc::traversor::handler::GroupHandler::Handle(vrml_proc::traversor::FullParsedVrmlNodeContext context, const vrml_proc::action::ConversionContextActionMap<ConversionContext>& actionMap)
{
	std::cout << "VRML Node - Group" << std::endl;
	return nullptr;

	//vrml_proc::traversor::validator::GroupNodeValidator validator(context.node, context.manager);
	//if (validator.Validate().has_error()) {
	//	// here we gonna extend the error type and return it
	//	return nullptr;
	//}

	//std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>, boost::recursive_wrapper<vrml_proc::parser::UseNode>>> defaultChildren = {};
	//auto& children = validator.GetCachedChildren(defaultChildren);
	//
	//std::vector<std::shared_ptr<vrml_proc::conversion_context::BaseConversionContext>> resolvedChildren;
	//resolvedChildren.reserve(children.size());

	//for (const auto& child : children) {

	//	auto vrmlNode = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractFromVariant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>>(child);
	//	if (vrmlNode.has_value()) {
	//		auto vrmlNodeTraversorResult = vrml_proc::traversor::VrmlNodeTraversor::Traverse({ vrmlNode.value().get(), context.manager }, actionMap);
	//		resolvedChildren.push_back(vrmlNodeTraversorResult);
	//	}

	//	auto useNode = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractFromVariant<boost::recursive_wrapper<vrml_proc::parser::UseNode>>(child);
	//	if (useNode.has_value()) {
	//		auto managerFound = context.manager.GetDefinitionNode(useNode.value().get_pointer()->identifier);
	//		auto useNodeTraversorResult = vrml_proc::traversor::VrmlNodeTraversor::Traverse({ *managerFound, context.manager }, actionMap); // TODO: manager must be thread-safe!
	//		resolvedChildren.push_back(useNodeTraversorResult);
	//	}
	//}

	//vrml_proc::parser::Vec3f defaultBoxCentre = { 0.0f, 0.0f, 0.0f };
	//vrml_proc::parser::Vec3f defaultBoxSize = { -1.0f, -1.0f, -1.0f };

	//return vrml_proc::traversor::utils::ConversionContextActionExecutor::TryToExecute<vrml_proc::conversion_context::MeshConversionContext>(
	//	actionMap, "Group", { resolvedChildren, validator.GetCachedBoxCenter(defaultBoxCentre), validator.GetCachedBoxSize(defaultBoxSize)});
}

namespace vrml_proc {
	namespace traversor {
		namespace handler {
			namespace GroupHandler {

				template VRMLPROCESSING_API std::shared_ptr<vrml_proc::conversion_context::MeshConversionContext>
					Handle<vrml_proc::conversion_context::MeshConversionContext>(
						vrml_proc::traversor::FullParsedVrmlNodeContext,
						const vrml_proc::action::ConversionContextActionMap<vrml_proc::conversion_context::MeshConversionContext>&);
			}
		}
	}
}