#pragma once

#include <memory>

#include "GeometryAction.hpp"
#include "IndexedFaceSetHandler.hpp"
#include "MeshConversionContext.hpp"

#include "VrmlProcessingExport.hpp"

namespace to_stl {
	namespace action {

		class VRMLPROCESSING_API IndexedFaceSetAction : public vrml_proc::action::GeometryAction {

		public:
			IndexedFaceSetAction(vrml_proc::traversor::handler::IndexedFaceSetHandler::IndexedFaceSetProperties properties, bool containedByShape);
			std::shared_ptr<vrml_proc::conversion_context::MeshConversionContext> Execute() override;

		private:
			vrml_proc::traversor::handler::IndexedFaceSetHandler::IndexedFaceSetProperties m_properties;
		};
	}
}
