#include "TransformHandler.hpp"

#include <memory>

#include <any>
#include <vector>

#include <result.hpp>

#include "ConversionContextActionExecutor.hpp"
#include "ConversionContextActionMap.hpp"
#include "Error.hpp"
#include "FormatString.hpp"
#include "FullParsedVrmlNodeContext.hpp"
#include "Logger.hpp"
#include "NodeTraversorError.hpp"
#include "TransformNodeValidator.hpp"
#include "Vec3f.hpp"
#include "VrmlNodeTraversor.hpp"
#include "Transformation.hpp"
#include "TransformationMatrix.hpp"

#include "VrmlProcessingExport.hpp"

template<typename ConversionContext>
cpp::result<std::shared_ptr<ConversionContext>, std::shared_ptr<vrml_proc::core::error::Error>> vrml_proc::traversor::handler::TransformHandler::Handle(vrml_proc::traversor::FullParsedVrmlNodeContext context, const vrml_proc::action::ConversionContextActionMap<ConversionContext>& actionMap)
{
    vrml_proc::core::logger::LogInfo(vrml_proc::core::utils::FormatString("Handle VRML node <", context.node.header, ">."), LOGGING_INFO);

    vrml_proc::traversor::validator::TransformNodeValidator validator(context.node, context.manager);
    auto validationResult = validator.Validate();
    if (validationResult.has_error()) {
        std::shared_ptr<vrml_proc::core::error::Error> error = std::make_shared<vrml_proc::traversor::error::NodeTraversorError>(validationResult.error(), context.node);
        return cpp::fail(error);
    }

    static vrml_proc::parser::Vec3f defaultCenter;
    static vrml_proc::parser::Vec4f defaultRotation = vrml_proc::parser::Vec4f(0.0f, 0.0f, 1.0f, 0.0f);
    static vrml_proc::parser::Vec3f defaultScale = vrml_proc::parser::Vec3f(1.0f, 1.0f, 1.0f);
    static vrml_proc::parser::Vec4f defaultScaleOrientation = vrml_proc::parser::Vec4f(0.0f, 0.0f, 1.0f, 0.0f);
    static vrml_proc::parser::Vec3f defaultTranslation;

    /** Update transformation data via copying. */
    vrml_proc::math::Transformation transformationData;
    transformationData.center = validator.GetCachedCenter(defaultCenter).get();
    transformationData.rotation = validator.GetCachedRotation(defaultRotation).get();
    transformationData.scale = validator.GetCachedScale(defaultScale).get();
    transformationData.scaleOrientation = validator.GetCachedScaleOrientation(defaultScaleOrientation).get();
    transformationData.translation = validator.GetCachedTranslation(defaultTranslation).get();

    vrml_proc::math::UpdateTransformationMatrix(context.transformation, transformationData);

    std::vector<std::shared_ptr<ConversionContext>> resolvedChildren;
    for (const auto& child : validator.GetCachedChildren()) {
        auto recursiveResult = vrml_proc::traversor::VrmlNodeTraversor::Traverse<ConversionContext>({ child, context.manager, context.IsDescendantOfShape, context.transformation }, actionMap);
        if (recursiveResult.has_error()) {
            return cpp::fail(recursiveResult.error());
        }
        resolvedChildren.push_back(recursiveResult.value());
    }

    static vrml_proc::parser::Vec3f defaultBoxCenter = { 0.0f, 0.0f, 0.0f };
    std::any cachedBoxCenter = validator.GetCachedBoxCenter(defaultBoxCenter);

    static vrml_proc::parser::Vec3f defaultBoxSize = { -1.0f, -1.0f, -1.0f };
    std::any cachedBoxSize = validator.GetCachedBoxSize(defaultBoxSize);

    std::any cachedCenter = validator.GetCachedCenter(defaultCenter);
    std::any cachedRotation = validator.GetCachedRotation(defaultRotation);
    std::any cachedScale = validator.GetCachedScale(defaultScale);
    std::any cachedScaleOrientation = validator.GetCachedScaleOrientation(defaultScaleOrientation);
    std::any cachedTranslation = validator.GetCachedTranslation(defaultTranslation);

    return vrml_proc::traversor::utils::ConversionContextActionExecutor::TryToExecute<ConversionContext>(actionMap, context.node.header, {
        std::cref(cachedBoxCenter),
        std::cref(cachedBoxSize),
        std::cref(cachedCenter),
        std::cref(cachedRotation),
        std::cref(cachedScale),
        std::cref(cachedScaleOrientation),
        std::cref(cachedTranslation) }, { resolvedChildren });
}
