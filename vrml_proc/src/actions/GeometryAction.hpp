#pragma once

#include <memory>

#include "BaseConversionContextAction.hpp"
#include "MeshConversionContext.hpp"

namespace vrml_proc {
	namespace action {

		class GeometryAction : public BaseConversionContextAction<vrml_proc::conversion_context::MeshConversionContext> {

		public:
			explicit GeometryAction(bool containedByShape) :
				m_containedByShape(containedByShape) {}

			virtual std::shared_ptr<vrml_proc::conversion_context::MeshConversionContext> Execute() = 0;
		protected:
			bool m_containedByShape;
		};
	}
}
