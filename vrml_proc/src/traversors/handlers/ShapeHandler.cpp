#include "ShapeHandler.hpp"

#include "BaseConversionContextActionExecutor.hpp"
#include "MeshConversionContext.hpp"
#include "ShapeNodeValidator.hpp"
#include "VrmlNode.hpp"
#include "VrmlNodeTraversor.hpp"

template<typename ConversionContext>
std::shared_ptr<ConversionContext> vrml_proc::traversor::handler::ShapeHandler::Handle(vrml_proc::traversor::FullParsedVrmlNodeContext context, const vrml_proc::action::BaseConversionContextActionMap<ConversionContext>& actionMap) {
	std::cout << "VRML Node - Shape" << std::endl;
	return nullptr;
	//vrml_proc::traversor::validator::ShapeNodeValidator validator(context.node, context.manager);
	//if (validator.Validate().has_error()) {
	//	// here we gonna extend the error type and return it
	//	return nullptr;
	//}

	//auto defaultAppearance = vrml_proc::parser::VrmlNode(); auto defaultGeometry = vrml_proc::parser::VrmlNode();
	//auto apperanceTraversorResult = vrml_proc::traversor::VrmlNodeTraversor::Traverse({ validator.GetCachedAppearance(defaultAppearance), context.manager, true }, actionMap);
	//auto geometryTraversorResult = vrml_proc::traversor::VrmlNodeTraversor::Traverse({ validator.GetCachedGeometry(defaultGeometry), context.manager, true }, actionMap);

	//return vrml_proc::traversor::utils::BaseConversionContextActionExecutor::TryToExecute<vrml_proc::conversion_context::MeshConversionContext>(actionMap, "Shape", { apperanceTraversorResult, geometryTraversorResult });
}

namespace vrml_proc {
	namespace traversor {
		namespace handler {
			namespace ShapeHandler {

				template VRMLPROCESSING_API std::shared_ptr<vrml_proc::conversion_context::MeshConversionContext>
					Handle<vrml_proc::conversion_context::MeshConversionContext>(
						vrml_proc::traversor::FullParsedVrmlNodeContext,
						const vrml_proc::action::BaseConversionContextActionMap<vrml_proc::conversion_context::MeshConversionContext>&);
			}
		}
	}
}