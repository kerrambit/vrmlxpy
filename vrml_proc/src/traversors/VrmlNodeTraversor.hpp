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

  template <typename ConversionContext>
  VRMLPROCESSING_API inline cpp::result<std::shared_ptr<ConversionContext>,
                                        std::shared_ptr<vrml_proc::core::error::Error>>
  Traverse(vrml_proc::traversor::FullParsedVrmlNodeContext context,
           const vrml_proc::action::ConversionContextActionMap<ConversionContext>& actionMap) {
    using namespace vrml_proc::core::logger;
    using namespace vrml_proc::core::utils;
    using namespace vrml_proc::traversor::handler;
    using namespace vrml_proc::traversor::error;
    using namespace vrml_proc::traversor::node_descriptor;

    bool ignoreUnknownNodeFlag = context.config.ignoreUnknownNode;

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

    auto descriptorMap = CreateNodeDescriptorMap();
    auto it = descriptorMap.find(canonicalHeader);
    if (it != descriptorMap.end()) {
      auto nd = it->second();
      auto validationResult = nd.Validate(context.node, context.manager);
      if (validationResult.has_error()) {
        LogError(FormatString("Validation for node <", context.node.header, "> failed!"), LOGGING_INFO);
        return cpp::fail(std::make_shared<NodeTraversorError>(validationResult.error(), context.node));
      }
    } else {
      if (ignoreUnknownNodeFlag) {
        LogInfo(FormatString("No handler for VRML node with name <", context.node.header,
                             "> was found! The unknown node will be ignored."),
                LOGGING_INFO);
        return std::make_shared<ConversionContext>();
      }

      LogError(FormatString("No handler for VRML node with name <", context.node.header,
                            "> was found! It is unknown VRML node."),
               LOGGING_INFO);
      std::shared_ptr<UnknownVrmlNode> innerError = std::make_shared<UnknownVrmlNode>(context.node.header);
      return cpp::fail(std::make_shared<NodeTraversorError>(innerError, context.node));
    }

    cpp::result<std::shared_ptr<ConversionContext>, std::shared_ptr<vrml_proc::core::error::Error>> handlerResult;

    if (canonicalHeader == "WorldInfo") {
      handlerResult = WorldInfoHandler::Handle(context, actionMap);
    } else if (canonicalHeader == "Group") {
      handlerResult = GroupHandler::Handle(context, actionMap);
    } else if (canonicalHeader == "Transform") {
      handlerResult = TransformHandler::Handle(context, actionMap);
    } else if (canonicalHeader == "Shape") {
      handlerResult = ShapeHandler::Handle(context, actionMap);
    } else if (canonicalHeader == "IndexedFaceSet") {
      handlerResult = IndexedFaceSetHandler::Handle(context, actionMap);
    } else if (canonicalHeader == "Coordinate") {
      handlerResult = CoordinateHandler::Handle(context, actionMap);
    } else if (canonicalHeader == "Normal") {
      handlerResult = NormalHandler::Handle(context, actionMap);
    } else if (canonicalHeader == "TextureCoordinate") {
      handlerResult = TextureCoordinateHandler::Handle(context, actionMap);
    } else if (canonicalHeader == "Color") {
      handlerResult = ColorHandler::Handle(context, actionMap);
    } else if (canonicalHeader == "Box") {
      handlerResult = BoxHandler::Handle(context, actionMap);
    } else if (canonicalHeader == "Switch") {
      handlerResult = SwitchHandler::Handle(context, actionMap);
    }

    if (handlerResult.has_error()) {
      return cpp::fail(std::make_shared<NodeTraversorError>(handlerResult.error(), context.node));
    }

    return handlerResult;
  }
}  // namespace vrml_proc::traversor::VrmlNodeTraversor