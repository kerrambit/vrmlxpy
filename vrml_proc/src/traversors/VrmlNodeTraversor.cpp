#include "VrmlNodeTraversor.hpp"

#include <memory>

#include "BoxHandler.hpp"
#include "GroupHandler.hpp"
#include "ShapeHandler.hpp"
#include "SpotlightHandler.hpp"
#include "VrmlNode.hpp"
#include "WorldInfoHandler.hpp"
#include "MeshConversionContext.hpp"

template<typename ConversionContext>
std::shared_ptr<ConversionContext> vrml_proc::traversor::VrmlNodeTraversor::Traverse(vrml_proc::traversor::FullParsedVrmlNodeContext context, const vrml_proc::action::BaseConversionContextActionMap<ConversionContext>& actionMap) {
	
	if (context.node.header == "WorldInfo") {
		//return vrml_proc::traversor::handler::WorldInfoHandler::Handle(context, actionMap);
	}
	else if (context.node.header == "Group") {
		//return vrml_proc::traversor::handler::GroupHandler::Handle(context, actionMap);
	}
	else if (context.node.header == "Shape") {
		//return vrml_proc::traversor::handler::ShapeHandler::Handle(context, actionMap);
	}
	else if (context.node.header == "Box") {
		return vrml_proc::traversor::handler::BoxHandler::Handle<ConversionContext>(context, actionMap);
	}
	else if (context.node.header == "Spotlight") {
		//return vrml_proc::traversor::handler::SpotlightHandler::Handle(context, actionMap);
	}

	// TODO: empty node is ok, check it here

	std::cout << "Error! Unknown VRML Node." << std::endl;
	return nullptr;
}

namespace vrml_proc {
	namespace traversor {
		namespace VrmlNodeTraversor {
			template VRMLPROCESSING_API std::shared_ptr<vrml_proc::conversion_context::MeshConversionContext> Traverse<vrml_proc::conversion_context::MeshConversionContext>(vrml_proc::traversor::FullParsedVrmlNodeContext context, const vrml_proc::action::BaseConversionContextActionMap<vrml_proc::conversion_context::MeshConversionContext>& actionMap);
		}
	}
}