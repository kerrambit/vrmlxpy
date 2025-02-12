#pragma once

#include <memory>
#include <vector>

#include "ConversionContextAction.hpp"
#include "MeshConversionContext.hpp"
#include "Vec3f.hpp"
#include "Vec4f.hpp"

#include "VrmlProcessingExport.hpp"

namespace to_stl {
	namespace action {
		/**
		 * @brief Represents a concrete type of Action.
		 *
		 * @implements ConversionContextAction
		 */
		class VRMLPROCESSING_API TransformAction : public vrml_proc::action::ConversionContextAction<vrml_proc::conversion_context::MeshConversionContext> {

		public:

			struct TransformProperties {
				std::reference_wrapper<const vrml_proc::parser::Vec3f> center;
				std::reference_wrapper<const vrml_proc::parser::Vec4f> rotation;
				std::reference_wrapper<const vrml_proc::parser::Vec3f> scale;
				std::reference_wrapper<const vrml_proc::parser::Vec4f> scaleOrientation;
				std::reference_wrapper<const vrml_proc::parser::Vec3f> translation;
				std::reference_wrapper<const vrml_proc::parser::Vec3f> bboxCenter;
				std::reference_wrapper<const vrml_proc::parser::Vec3f> bboxSize;
				std::vector<std::shared_ptr<vrml_proc::conversion_context::MeshConversionContext>> children;
			};

			TransformAction(TransformProperties properties);

			std::shared_ptr<vrml_proc::conversion_context::MeshConversionContext> Execute() override;

		private:
			TransformProperties m_properties;
		};
	}
}
