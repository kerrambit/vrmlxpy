#pragma once

#include <memory>

#include <result.hpp>

#include "BoxHandler.hpp"
#include "ColorHandler.hpp"
#include "ConversionContextActionMap.hpp"
#include "CoordinateHandler.hpp"
#include "Error.hpp"
#include "FormatString.hpp"
#include "FullParsedVrmlNodeContext.hpp"
#include "GroupHandler.hpp"
#include "IndexedFaceSetHandler.hpp"
#include "Logger.hpp"
#include "NodeTraversorError.hpp"
#include "NormalHandler.hpp"
#include "ShapeHandler.hpp"
#include "SpotlightHandler.hpp"
#include "SwitchHandler.hpp"
#include "TextureCoordinateHandler.hpp"
#include "TransformHandler.hpp"
#include "VrmlNode.hpp"
#include "WorldInfoHandler.hpp"

#include "VrmlProcessingExport.hpp"

namespace vrml_proc::traversor::VrmlNodeTraversor {

	template<typename ConversionContext>
	VRMLPROCESSING_API inline cpp::result<std::shared_ptr<ConversionContext>, std::shared_ptr<vrml_proc::core::error::Error>> Traverse(vrml_proc::traversor::FullParsedVrmlNodeContext context, const vrml_proc::action::ConversionContextActionMap<ConversionContext>& actionMap) {

		using namespace vrml_proc::core::logger;
		using namespace vrml_proc::core::utils;
		using namespace vrml_proc::traversor::handler;
		using namespace vrml_proc::traversor::error;

		LogInfo(FormatString("Find handler for VRML node with name <", context.node.header, ">."), LOGGING_INFO);

		if (context.node.header == "") {
			LogInfo("Handle empty VRML node.", LOGGING_INFO);
			return std::make_shared<ConversionContext>();
		}
		else if (context.node.header == "WorldInfo" || context.node.header == "VRMLWorldInfo") {
			return WorldInfoHandler::Handle(context, actionMap);
		}
		else if (context.node.header == "Group" || context.node.header == "VRMLGroup") {
			return GroupHandler::Handle(context, actionMap);
		}
		else if (context.node.header == "Transform" || context.node.header == "VRMLTransform") {
			return TransformHandler::Handle(context, actionMap);
		}
		else if (context.node.header == "Shape" || context.node.header == "VRMLShape") {
			return ShapeHandler::Handle(context, actionMap);
		}
		else if (context.node.header == "Appearance" || context.node.header == "VRMLAppearance") {
			// TODO
			return std::make_shared<ConversionContext>();
		}
		else if (context.node.header == "IndexedFaceSet" || context.node.header == "VRMLIndexedFaceSet") {
			return IndexedFaceSetHandler::Handle(context, actionMap);
		}
		else if (context.node.header == "Coordinate" || context.node.header == "VRMLCoordinate") {
			return CoordinateHandler::Handle(context, actionMap);
		}
		else if (context.node.header == "Normal" || context.node.header == "VRMLNormal") {
			return NormalHandler::Handle(context, actionMap);
		}
		else if (context.node.header == "TextureCoordinate" || context.node.header == "VRMLTextureCoordinate") {
			return TextureCoordinateHandler::Handle(context, actionMap);
		}
		else if (context.node.header == "Color" || context.node.header == "VRMLColor") {
			return ColorHandler::Handle(context, actionMap);
		}
		else if (context.node.header == "Box" || context.node.header == "VRMLBox") {
			return BoxHandler::Handle(context, actionMap);
		}
		else if (context.node.header == "Switch" || context.node.header == "VRMLSwitch") {
			return SwitchHandler::Handle(context, actionMap);
		}
		else if (context.node.header == "Spotlight") {
			//return SpotlightHandler::Handle(context, actionMap);
		}

		LogError(FormatString("No handler for VRML node with name <", context.node.header, "> was found! It is unknown VRML node."), LOGGING_INFO);
		std::shared_ptr<UnknownVrmlNode> innerError = std::make_shared<UnknownVrmlNode>(context.node.header);
		
		return cpp::fail(std::make_shared<NodeTraversorError>(innerError, context.node));
	}
}