#pragma once

#include <memory>
#include <functional>
#include <memory>

#include <result.hpp>

#include "ConversionContextActionMap.hpp"
#include "CoordConversionContext.hpp"
#include "GeometryAction.hpp"
#include "Logger.hpp"
#include "MeshConversionContext.hpp"
#include "Vec3fArray.hpp"
#include "VrmlFieldExtractor.hpp"
#include "VrmlNodeManager.hpp"
#include "VrmlNodeTraversor.hpp"
#include "IndexedFaceSetHandler.hpp"

#include "VrmlProcessingExport.hpp"

namespace to_stl {
	namespace action {

		class VRMLPROCESSING_API IndexedFaceSetAction : public vrml_proc::action::GeometryAction {

		public:
			IndexedFaceSetAction(const vrml_proc::traversor::handler::IndexedFaceSetHandler::IndexedFaceSetProperties& properties, bool containedByShape);

			std::shared_ptr<vrml_proc::conversion_context::MeshConversionContext> Execute() override;

		private:
			const vrml_proc::traversor::handler::IndexedFaceSetHandler::IndexedFaceSetProperties& m_properties;
		};
	}
}
