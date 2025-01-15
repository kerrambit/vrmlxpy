#pragma once

#include <memory>

#include "BaseConversionContextAction.hpp"

namespace vrml_proc {
	namespace action {

		class GeometryAction : public BaseConversionContextAction {

		public:
			explicit GeometryAction(bool containedByShape) :
				m_containedByShape(containedByShape) {}

			virtual std::shared_ptr<vrml_proc::conversion_context::BaseConversionContext> Execute() = 0;
		protected:
			bool m_containedByShape;
		};
	}
}
