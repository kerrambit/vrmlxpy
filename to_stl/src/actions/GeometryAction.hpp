#pragma once

#include <memory>

#include "ConversionContextAction.hpp"
#include "MeshTaskConversionContext.hpp"
#include "TransformationMatrix.hpp"

namespace to_stl {
	namespace action {

		class GeometryAction : public vrml_proc::action::ConversionContextAction<to_stl::conversion_context::MeshTaskConversionContext> {

		public:

			struct Properties {
				bool containedByShape;
				vrml_proc::math::TransformationMatrix matrix;
			};

			explicit GeometryAction(Properties properties) :
				m_geometryProperties(properties) {}

			virtual std::shared_ptr<to_stl::conversion_context::MeshTaskConversionContext> Execute() = 0;
		protected:
			Properties m_geometryProperties;
		};
	}
}
