#pragma once

#include <memory>

#include "ConversionContextAction.hpp"
#include "MeshTaskConversionContext.hpp"

namespace to_stl {
	namespace action {

		class GeometryAction : public vrml_proc::action::ConversionContextAction<to_stl::conversion_context::MeshTaskConversionContext> {

		public:
			explicit GeometryAction(bool containedByShape) :
				m_containedByShape(containedByShape) {}

			virtual std::shared_ptr<to_stl::conversion_context::MeshTaskConversionContext> Execute() = 0;
		protected:
			bool m_containedByShape;
		};
	}
}
