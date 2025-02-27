#pragma once

#include <memory>

#include <result.hpp>

#include "ConversionContextActionExecutor.hpp"
#include "ConversionContextActionMap.hpp"
#include "Error.hpp"
#include "FormatString.hpp"
#include "FullParsedVrmlNodeContext.hpp"
#include "Logger.hpp"
#include "NodeTraversorError.hpp"
#include "Transformation.hpp"
#include "TransformationMatrix.hpp"
#include "TransformNodeValidator.hpp"
#include "Vec3f.hpp"

#include "VrmlProcessingExport.hpp"

// Forward declaration.
namespace vrml_proc::traversor::VrmlNodeTraversor {
    template<typename ConversionContext>
    VRMLPROCESSING_API cpp::result<std::shared_ptr<ConversionContext>, std::shared_ptr<vrml_proc::core::error::Error>> Traverse(
        vrml_proc::traversor::FullParsedVrmlNodeContext context,
        const vrml_proc::action::ConversionContextActionMap<ConversionContext>& actionMap);
}

namespace vrml_proc::traversor::handler::TransformHandler {

	template<typename ConversionContext>
	VRMLPROCESSING_API inline cpp::result<std::shared_ptr<ConversionContext>, std::shared_ptr<vrml_proc::core::error::Error>> Handle(vrml_proc::traversor::FullParsedVrmlNodeContext context, const vrml_proc::action::ConversionContextActionMap<ConversionContext>& actionMap) {
        
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

        vrml_proc::math::TransformationMatrix transformation = vrml_proc::math::UpdateTransformationMatrix(context.transformation, transformationData);

        std::vector<std::shared_ptr<ConversionContext>> resolvedChildren;
        for (const auto& child : validator.GetCachedChildren()) {
            auto recursiveResult = vrml_proc::traversor::VrmlNodeTraversor::Traverse<ConversionContext>({ child, context.manager, context.IsDescendantOfShape, transformation, context.config }, actionMap);
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
            std::cref(cachedCenter),
            std::cref(cachedRotation),
            std::cref(cachedScale),
            std::cref(cachedScaleOrientation),
            std::cref(cachedTranslation),
            std::cref(cachedBoxCenter),
            std::cref(cachedBoxSize) }, { resolvedChildren });
	}

    template<typename ConversionContext>
    VRMLPROCESSING_API inline cpp::result<std::shared_ptr<ConversionContext>, std::shared_ptr<vrml_proc::core::error::Error>> Handle(vrml_proc::traversor::FullParsedVrmlNodeContext context, const vrml_proc::action::ConversionContextActionMap<ConversionContext>& actionMap, const vrml_proc::traversor::node_descriptor::NodeDescriptor& nd) {

        vrml_proc::core::logger::LogInfo(vrml_proc::core::utils::FormatString("Handle VRML node <", context.node.header, ">."), LOGGING_INFO);

        /** Update transformation data via copying. */
        vrml_proc::math::Transformation transformationData;
        transformationData.center = nd.GetField<std::reference_wrapper<const vrml_proc::parser::Vec3f>>("center").get();
        transformationData.rotation = nd.GetField<std::reference_wrapper<const vrml_proc::parser::Vec4f>>("rotation").get();
        transformationData.scale = nd.GetField<std::reference_wrapper<const vrml_proc::parser::Vec3f>>("scale").get();
        transformationData.scaleOrientation = nd.GetField<std::reference_wrapper<const vrml_proc::parser::Vec4f>>("scaleOrientation").get();
        transformationData.translation = nd.GetField<std::reference_wrapper<const vrml_proc::parser::Vec3f>>("translation").get();

        vrml_proc::math::TransformationMatrix transformation = vrml_proc::math::UpdateTransformationMatrix(context.transformation, transformationData);

        std::vector<std::shared_ptr<ConversionContext>> resolvedChildren;
        for (const auto& child : nd.GetField<std::vector<std::reference_wrapper<const vrml_proc::parser::VrmlNode>>>("children")) {
            auto recursiveResult = vrml_proc::traversor::VrmlNodeTraversor::Traverse<ConversionContext>({ child, context.manager, context.IsDescendantOfShape, transformation }, actionMap);
            if (recursiveResult.has_error()) {
                return cpp::fail(recursiveResult.error());
            }
            resolvedChildren.push_back(recursiveResult.value());
        }

        std::any cachedBoxCenter = nd.GetField<std::reference_wrapper<const vrml_proc::parser::Vec3f>>("bboxCenter");
        std::any cachedBoxSize = nd.GetField<std::reference_wrapper<const vrml_proc::parser::Vec3f>>("bboxSize");
        std::any cachedCenter = nd.GetField<std::reference_wrapper<const vrml_proc::parser::Vec3f>>("center");
        std::any cachedRotation = nd.GetField<std::reference_wrapper<const vrml_proc::parser::Vec4f>>("rotation");
        std::any cachedScale = nd.GetField<std::reference_wrapper<const vrml_proc::parser::Vec3f>>("scale");
        std::any cachedScaleOrientation = nd.GetField<std::reference_wrapper<const vrml_proc::parser::Vec4f>>("scaleOrientation");
        std::any cachedTranslation = nd.GetField<std::reference_wrapper<const vrml_proc::parser::Vec3f>>("translation");

        return vrml_proc::traversor::utils::ConversionContextActionExecutor::TryToExecute<ConversionContext>(actionMap, nd.GetId(), {
            std::cref(cachedCenter),
            std::cref(cachedRotation),
            std::cref(cachedScale),
            std::cref(cachedScaleOrientation),
            std::cref(cachedTranslation),
            std::cref(cachedBoxCenter),
            std::cref(cachedBoxSize) }, { resolvedChildren });
    }
}