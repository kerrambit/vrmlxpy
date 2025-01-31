#include "VrmlNodeTraversor.hpp"

#include <memory>

#include "BoxHandler.hpp"
#include "ColorHandler.hpp"
#include "CoordinateHandler.hpp"
#include "GroupHandler.hpp"
#include "IndexedFaceSetHandler.hpp"
#include "Logger.hpp"
#include "MeshConversionContext.hpp"
#include "NodeTraversorError.hpp"
#include "NormalHandler.hpp"
#include "ShapeHandler.hpp"
#include "SpotlightHandler.hpp"
#include "TextureCoordinateHandler.hpp"
#include "VrmlNode.hpp"
#include "WorldInfoHandler.hpp"
#include <Error.hpp>
#include <result.hpp>

template<typename ConversionContext>
cpp::result<std::shared_ptr<ConversionContext>, std::shared_ptr<vrml_proc::core::error::Error>> vrml_proc::traversor::VrmlNodeTraversor::Traverse(vrml_proc::traversor::FullParsedVrmlNodeContext context, const vrml_proc::action::ConversionContextActionMap<ConversionContext>& actionMap) {
	
	LOG_INFO() << "Find handler for VRML node with name <" << context.node.header << ">.";

	if (context.node.header == "") {
		LOG_INFO() << "Handle empty VRML node.";
		return std::make_shared<ConversionContext>();
	}
	else if (context.node.header == "WorldInfo") {
		return vrml_proc::traversor::handler::WorldInfoHandler::Handle(context, actionMap);
	}
	else if (context.node.header == "Group") {
		return vrml_proc::traversor::handler::GroupHandler::Handle<ConversionContext>(context, actionMap);
	}
	else if (context.node.header == "Shape") {
		return vrml_proc::traversor::handler::ShapeHandler::Handle<ConversionContext>(context, actionMap);
	}
	else if (context.node.header == "IndexedFaceSet") {
		// return vrml_proc::traversor::handler::IndexedFaceSetHandler::Handle<ConversionContext>(context, actionMap);
	}
	else if (context.node.header == "Coordinate") {
		// return vrml_proc::traversor::handler::CoordinateHandler::Handle<ConversionContext>(context, actionMap);
	}
	else if (context.node.header == "Normal") {
		// return vrml_proc::traversor::handler::NormalHandler::Handle<ConversionContext>(context, actionMap);
	}
	else if (context.node.header == "TextureCoordinate") {
		// return vrml_proc::traversor::handler::TextureCoordinateHandler::Handle<ConversionContext>(context, actionMap);
	}
	else if (context.node.header == "Color") {
		// return vrml_proc::traversor::handler::ColorHandler::Handle<ConversionContext>(context, actionMap);
	}
	else if (context.node.header == "Box") {
		return vrml_proc::traversor::handler::BoxHandler::Handle<ConversionContext>(context, actionMap);
	}
	else if (context.node.header == "Spotlight") {
		//return vrml_proc::traversor::handler::SpotlightHandler::Handle(context, actionMap);
	}

	LOG_ERROR() << "No handler for VRML node with name <" << context.node.header << "> was found! It is unknown VRML node.";
	std::shared_ptr<vrml_proc::traversor::error::UnknownVrmlNode> innerError = std::make_shared<vrml_proc::traversor::error::UnknownVrmlNode>(context.node.header);
	return cpp::fail(std::make_shared<vrml_proc::traversor::error::NodeTraversorError>(innerError, context.node));
}

namespace vrml_proc {
	namespace traversor {
		namespace VrmlNodeTraversor {
			template VRMLPROCESSING_API cpp::result<std::shared_ptr<vrml_proc::conversion_context::MeshConversionContext>, std::shared_ptr<vrml_proc::core::error::Error>> Traverse<vrml_proc::conversion_context::MeshConversionContext>(vrml_proc::traversor::FullParsedVrmlNodeContext context, const vrml_proc::action::ConversionContextActionMap<vrml_proc::conversion_context::MeshConversionContext>& actionMap);
		}
	}
}