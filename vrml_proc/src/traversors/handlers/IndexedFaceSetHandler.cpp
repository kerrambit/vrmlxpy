#include "IndexedFaceSetHandler.hpp"

#include <any>
#include <memory>

#include <result.hpp>

#include "ConversionContextActionExecutor.hpp"
#include "ConversionContextActionMap.hpp"
#include "Error.hpp"
#include "IndexedFaceSetNodeValidator.hpp"
#include "Int32Array.hpp"
#include "FormatString.hpp"
#include "Logger.hpp"
#include "MeshConversionContext.hpp"
#include "NodeTraversorError.hpp"
#include "VrmlNode.hpp"
#include "VrmlUnits.hpp"

#include "VrmlProcessingExport.hpp"

template<typename ConversionContext>
cpp::result<std::shared_ptr<ConversionContext>, std::shared_ptr<vrml_proc::core::error::Error>> vrml_proc::traversor::handler::IndexedFaceSetHandler::Handle(vrml_proc::traversor::FullParsedVrmlNodeContext context, const vrml_proc::action::ConversionContextActionMap<ConversionContext>& actionMap)
{
    vrml_proc::core::logger::LogInfo(vrml_proc::core::utils::FormatString("Handle VRML node <", context.node.header, ">."), LOGGING_INFO);

    vrml_proc::traversor::validator::IndexedFaceSetNodeValidator validator(context.node, context.manager);
    auto validationResult = validator.Validate();
    if (validationResult.has_error()) {
        std::shared_ptr<vrml_proc::core::error::Error> error = std::make_shared<vrml_proc::traversor::error::NodeTraversorError>(validationResult.error(), context.node);
        return cpp::fail(error);
    }

    // There are geometry primitive nodes. They are send as VrmlNodes into the given action where they can be traversed if needed.
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
    std::any colorIndex = validator.GetCachedColorIndex(defaultColorIndex);
    std::any coordIndex = validator.GetCachedCoordIndex(defaultCoordIndex);
    std::any normalIndex = validator.GetCachedNormalIndex(defaultNormalIndex);
    std::any texCoordIndex = validator.GetCachedTexCoordIndex(defaultTexCoordIndex);
    std::any creaseAngle = validator.GetCachedCreaseAngle(defaultCreaseangle);
    std::any ccw = validator.GetCachedCcw(defaultCcw);
    std::any colorPerVertex = validator.GetCachedColorPerVertex(defaultColorPerVertex);
    std::any convex = validator.GetCachedConvex(defaultConvex);
    std::any normalPerVertex = validator.GetCachedNormalPerVertex(defaultNormalPerVertex);
    std::any solid = validator.GetCachedSolid(defaultSolid);

    return vrml_proc::traversor::utils::ConversionContextActionExecutor::TryToExecute<ConversionContext>(
        actionMap, context.node.header,
        {
            std::cref(color),
            std::cref(coord),
            std::cref(normal),
            std::cref(texCoord),
            std::cref(colorIndex),
            std::cref(coordIndex),
            std::cref(normalIndex),
            std::cref(texCoordIndex),
            std::cref(creaseAngle),
            std::cref(ccw),
            std::cref(colorPerVertex),
            std::cref(convex),
            std::cref(normalPerVertex),
            std::cref(solid)
        }, { context.IsDescendantOfShape });
}

namespace vrml_proc {
    namespace traversor {
        namespace handler {
            namespace IndexedFaceSetHandler {

                template VRMLPROCESSING_API cpp::result<std::shared_ptr<vrml_proc::conversion_context::MeshConversionContext>, std::shared_ptr<vrml_proc::core::error::Error>>
                    Handle<vrml_proc::conversion_context::MeshConversionContext>(
                        vrml_proc::traversor::FullParsedVrmlNodeContext,
                        const vrml_proc::action::ConversionContextActionMap<vrml_proc::conversion_context::MeshConversionContext>&);
            }
        }
    }
}