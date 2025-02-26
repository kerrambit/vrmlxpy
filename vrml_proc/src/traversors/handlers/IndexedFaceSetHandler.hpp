#pragma once

#include <any>
#include <memory>

#include <result.hpp>

#include "ConversionContextActionExecutor.hpp"
#include "ConversionContextActionMap.hpp"
#include "Error.hpp"
#include "FormatString.hpp"
#include "FullParsedVrmlNodeContext.hpp"
#include "IndexedFaceSetNodeValidator.hpp"
#include "Int32Array.hpp"
#include "Logger.hpp"
#include "NodeTraversorError.hpp"
#include "VrmlNode.hpp"
#include "VrmlUnits.hpp"

#include "VrmlProcessingExport.hpp"

namespace vrml_proc::traversor::handler::IndexedFaceSetHandler {

    template<typename ConversionContext>
    VRMLPROCESSING_API cpp::result<std::shared_ptr<ConversionContext>, std::shared_ptr<vrml_proc::core::error::Error>> Handle(vrml_proc::traversor::FullParsedVrmlNodeContext context, const vrml_proc::action::ConversionContextActionMap<ConversionContext>& actionMap) {
        vrml_proc::core::logger::LogInfo(vrml_proc::core::utils::FormatString("Handle VRML node <", context.node.header, ">."), LOGGING_INFO);

        vrml_proc::traversor::validator::IndexedFaceSetNodeValidator validator(context.node, context.manager);
        auto validationResult = validator.Validate();
        if (validationResult.has_error()) {
            std::shared_ptr<vrml_proc::core::error::Error> error = std::make_shared<vrml_proc::traversor::error::NodeTraversorError>(validationResult.error(), context.node);
            return cpp::fail(error);
        }

        // There are geometry primitive nodes. They are send as VrmlNodes into the given action where they can be traversed if needed.
        // TODO: we should run validators for coord, color, normal and texCoord

        static vrml_proc::parser::VrmlNode defaultColor;
        static vrml_proc::parser::VrmlNode defaultCoord;
        static vrml_proc::parser::VrmlNode defaultNormal;
        static vrml_proc::parser::VrmlNode defaultTexCoord;

        static bool defaultCcw = true;
        static vrml_proc::parser::Int32Array defaultColorIndex;
        static bool defaultColorPerVertex = true;
        static bool defaultConvex = true;
        static vrml_proc::parser::Int32Array defaultCoordIndex;
        static vrml_proc::parser::float32_t defaultCreaseangle = 0.0f;
        static vrml_proc::parser::Int32Array defaultNormalIndex;
        static bool defaultNormalPerVertex = true;
        static bool defaultSolid = true;
        static vrml_proc::parser::Int32Array defaultTexCoordIndex;

        std::any color = validator.GetCachedColor(defaultColor);
        std::any coord = validator.GetCachedCoord(defaultCoord);
        std::any normal = validator.GetCachedNormal(defaultNormal);
        std::any texCoord = validator.GetCachedTexCoord(defaultTexCoord);
        std::any ccw = validator.GetCachedCcw(defaultCcw);
        std::any colorIndex = validator.GetCachedColorIndex(defaultColorIndex);
        std::any colorPerVertex = validator.GetCachedColorPerVertex(defaultColorPerVertex);
        std::any convex = validator.GetCachedConvex(defaultConvex);
        std::any coordIndex = validator.GetCachedCoordIndex(defaultCoordIndex);
        std::any creaseAngle = validator.GetCachedCreaseAngle(defaultCreaseangle);
        std::any normalIndex = validator.GetCachedNormalIndex(defaultNormalIndex);
        std::any normalPerVertex = validator.GetCachedNormalPerVertex(defaultNormalPerVertex);
        std::any solid = validator.GetCachedSolid(defaultSolid);
        std::any texCoordIndex = validator.GetCachedTexCoordIndex(defaultTexCoordIndex);

        return vrml_proc::traversor::utils::ConversionContextActionExecutor::TryToExecute<ConversionContext>(
            actionMap, context.node.header,
            {
                std::cref(color),
                std::cref(coord),
                std::cref(normal),
                std::cref(texCoord),
                std::cref(ccw),
                std::cref(colorIndex),
                std::cref(colorPerVertex),
                std::cref(convex),
                std::cref(coordIndex),
                std::cref(creaseAngle),
                std::cref(normalIndex),
                std::cref(normalPerVertex),
                std::cref(solid),
                std::cref(texCoordIndex),
            }, { context.IsDescendantOfShape, context.transformation });
    }

    template<typename ConversionContext>
    VRMLPROCESSING_API cpp::result<std::shared_ptr<ConversionContext>, std::shared_ptr<vrml_proc::core::error::Error>> Handle(vrml_proc::traversor::FullParsedVrmlNodeContext context, const vrml_proc::action::ConversionContextActionMap<ConversionContext>& actionMap, const vrml_proc::traversor::node_descriptor::NodeDescriptor& nd) {
        
        vrml_proc::core::logger::LogInfo(vrml_proc::core::utils::FormatString("Handle VRML node <", context.node.header, ">."), LOGGING_INFO);

        // There are geometry primitive nodes. They are send as VrmlNodes into the given action where they can be traversed if needed.
        // TODO: we should run validators for coord, color, normal and texCoord

        std::any color = nd.GetField<std::reference_wrapper<const vrml_proc::parser::VrmlNode>>("color");
        std::any coord = nd.GetField<std::reference_wrapper<const vrml_proc::parser::VrmlNode>>("coord");
        std::any normal = nd.GetField<std::reference_wrapper<const vrml_proc::parser::VrmlNode>>("normal");
        std::any texCoord = nd.GetField<std::reference_wrapper<const vrml_proc::parser::VrmlNode>>("texCoord");
        std::any ccw = nd.GetField<std::reference_wrapper<const bool>>("ccw");
        std::any colorIndex = nd.GetField<std::reference_wrapper<const vrml_proc::parser::Int32Array>>("colorIndex");
        std::any colorPerVertex = nd.GetField<std::reference_wrapper<const bool>>("colorPerVertex");
        std::any convex = nd.GetField<std::reference_wrapper<const bool>>("convex");
        std::any coordIndex = nd.GetField<std::reference_wrapper<const vrml_proc::parser::Int32Array>>("coordIndex");
        std::any creaseAngle = nd.GetField<std::reference_wrapper<const vrml_proc::parser::float32_t>>("creaseAngle");
        std::any normalIndex = nd.GetField<std::reference_wrapper<const vrml_proc::parser::Int32Array>>("normalIndex");
        std::any normalPerVertex = nd.GetField<std::reference_wrapper<const bool>>("normalPerVertex");
        std::any solid = nd.GetField<std::reference_wrapper<const bool>>("solid");
        std::any texCoordIndex = nd.GetField<std::reference_wrapper<const vrml_proc::parser::Int32Array>>("texCoordIndex");

        return vrml_proc::traversor::utils::ConversionContextActionExecutor::TryToExecute<ConversionContext>(
            actionMap, nd.GetId(),
            {
                std::cref(color),
                std::cref(coord),
                std::cref(normal),
                std::cref(texCoord),
                std::cref(ccw),
                std::cref(colorIndex),
                std::cref(colorPerVertex),
                std::cref(convex),
                std::cref(coordIndex),
                std::cref(creaseAngle),
                std::cref(normalIndex),
                std::cref(normalPerVertex),
                std::cref(solid),
                std::cref(texCoordIndex),
            }, { context.IsDescendantOfShape, context.transformation });
    }
}