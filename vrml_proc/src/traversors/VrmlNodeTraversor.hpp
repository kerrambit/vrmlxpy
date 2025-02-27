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
#include <NodeDescriptorMap.hpp>
#include <VrmlCanonicalHeaders.hpp>

#include "VrmlProcessingExport.hpp"

namespace vrml_proc::traversor::VrmlNodeTraversor {

	template<typename ConversionContext>
	VRMLPROCESSING_API inline cpp::result<std::shared_ptr<ConversionContext>, std::shared_ptr<vrml_proc::core::error::Error>> Traverse(vrml_proc::traversor::FullParsedVrmlNodeContext context, const vrml_proc::action::ConversionContextActionMap<ConversionContext>& actionMap) {

		using namespace vrml_proc::core::logger;
		using namespace vrml_proc::core::utils;
		using namespace vrml_proc::traversor::handler;
		using namespace vrml_proc::traversor::error;
		using namespace vrml_proc::traversor::node_descriptor;

		bool ignoreUnknownNodeFlag = false;

		LogInfo(FormatString("Find handler for VRML node with name <", context.node.header, ">."), LOGGING_INFO);

		if (context.node.header.empty()) {
			LogInfo("Handle empty VRML node.", LOGGING_INFO);
			return std::make_shared<ConversionContext>();
		}

		std::string canonicalHeader;
		{
			auto it = HeaderToCanonicalName.find(context.node.header);
			if (it != HeaderToCanonicalName.end()) {
				canonicalHeader = it->second;
			}
		}

		// TMP, first config file must be read!
		if (canonicalHeader == "Appearance" || canonicalHeader == "VRMLAppearance") {
			return std::make_shared<ConversionContext>();
		}

		auto descriptorMap = CreateNodeDescriptorMap();
		auto it = descriptorMap.find(canonicalHeader);
		if (it != descriptorMap.end()) {
			auto nd = it->second();
			auto validationResult = nd.Validate(context.node, context.manager);
			if (validationResult.has_error()) {
				LogError(FormatString("Validation for node <", context.node.header, "> failed!"), LOGGING_INFO);
				return cpp::fail(std::make_shared<NodeTraversorError>(validationResult.error(), context.node));
			}
		}
		else {
			if (ignoreUnknownNodeFlag) {
				LogInfo(FormatString("No handler for VRML node with name <", context.node.header, "> was found! The unknown node will be ignored."), LOGGING_INFO);
				return std::make_shared<ConversionContext>();
			}

			LogError(FormatString("No handler for VRML node with name <", context.node.header, "> was found! It is unknown VRML node."), LOGGING_INFO);
			std::shared_ptr<UnknownVrmlNode> innerError = std::make_shared<UnknownVrmlNode>(context.node.header);
			return cpp::fail(std::make_shared<NodeTraversorError>(innerError, context.node));
		}

		if (canonicalHeader == "WorldInfo") {
			return WorldInfoHandler::Handle(context, actionMap);
		}
		else if (canonicalHeader == "Group") {
			return GroupHandler::Handle(context, actionMap);
		}
		else if (canonicalHeader == "Transform") {
			return TransformHandler::Handle(context, actionMap);
		}
		else if (canonicalHeader == "Shape") {
			return ShapeHandler::Handle(context, actionMap);
		}
		else if (canonicalHeader == "IndexedFaceSet") {
			return IndexedFaceSetHandler::Handle(context, actionMap);
		}
		else if (canonicalHeader == "Coordinate") {
			return CoordinateHandler::Handle(context, actionMap);
		}
		else if (canonicalHeader == "Normal") {
			return NormalHandler::Handle(context, actionMap);
		}
		else if (canonicalHeader == "TextureCoordinate") {
			return TextureCoordinateHandler::Handle(context, actionMap);
		}
		else if (canonicalHeader == "Color") {
			return ColorHandler::Handle(context, actionMap);
		}
		else if (canonicalHeader == "Box") {
			return BoxHandler::Handle(context, actionMap);
		}
		else if (canonicalHeader == "Switch") {
			return SwitchHandler::Handle(context, actionMap);
		}
	}
}