#include "VrmlNodeTraversor.hpp"

#include <memory>

#include "BoxHandler.hpp"
#include "GroupHandler.hpp"
#include "ShapeHandler.hpp"
#include "SpotlightHandler.hpp"
#include "VrmlNode.hpp"
#include "WorldInfoHandler.hpp"
#include "MeshConversionContext.hpp"
#include <result.hpp>
#include <Error.hpp>
#include "Logger.hpp"

template<typename ConversionContext>
cpp::result<std::shared_ptr<ConversionContext>, std::shared_ptr<vrml_proc::core::error::Error>> vrml_proc::traversor::VrmlNodeTraversor::Traverse(vrml_proc::traversor::FullParsedVrmlNodeContext context, const vrml_proc::action::ConversionContextActionMap<ConversionContext>& actionMap) {
	
	LOG_INFO() << "Find handler for VRML node with name <" << context.node.header << ">.";

	if (context.node.header == "WorldInfo") {
		return vrml_proc::traversor::handler::WorldInfoHandler::Handle(context, actionMap);
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
			template VRMLPROCESSING_API cpp::result<std::shared_ptr<vrml_proc::conversion_context::MeshConversionContext>, std::shared_ptr<vrml_proc::core::error::Error>> Traverse<vrml_proc::conversion_context::MeshConversionContext>(vrml_proc::traversor::FullParsedVrmlNodeContext context, const vrml_proc::action::ConversionContextActionMap<vrml_proc::conversion_context::MeshConversionContext>& actionMap);
		}
	}
}