#pragma once

#include <memory>

#include "GeometryAction.hpp"
#include "MeshConversionContext.hpp"
#include "Vec3f.hpp"

#include "VrmlProcessingExport.hpp"

namespace to_stl {
	namespace action {

		class VRMLPROCESSING_API BoxAction : public vrml_proc::action::GeometryAction {

		public:

			struct BoxProperties {
				std::reference_wrapper<const vrml_proc::parser::Vec3f> size;
			};

			BoxAction(std::reference_wrapper<const vrml_proc::parser::Vec3f> size, bool containedByShape);
			std::shared_ptr<vrml_proc::conversion_context::MeshConversionContext> Execute() override;

		private:
			std::reference_wrapper<const vrml_proc::parser::Vec3f> m_size;
		};
	}
}
