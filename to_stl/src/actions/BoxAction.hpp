#pragma once

#include <memory>

#include "GeometryAction.hpp"
#include "MeshTaskConversionContext.hpp"
#include "Vec3f.hpp"

#include "VrmlProcessingExport.hpp"

namespace to_stl {
	namespace action {

		class VRMLPROCESSING_API BoxAction : public to_stl::action::GeometryAction {

		public:

			struct Properties {
				std::reference_wrapper<const vrml_proc::parser::Vec3f> size;
			};

			BoxAction(std::reference_wrapper<const vrml_proc::parser::Vec3f> size, bool containedByShape);
			std::shared_ptr<to_stl::conversion_context::MeshTaskConversionContext> Execute() override;

		private:
			std::reference_wrapper<const vrml_proc::parser::Vec3f> m_size;
		};
	}
}
