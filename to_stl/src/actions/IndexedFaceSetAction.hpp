#pragma once

#include <functional>
#include <memory>

#include "GeometryAction.hpp"
#include "Int32Array.hpp"
#include "MeshConversionContext.hpp"
#include "VrmlNode.hpp"
#include "Int32Array.hpp"
#include "VrmlUnits.hpp"

#include "VrmlProcessingExport.hpp"

namespace to_stl {
	namespace action {

		class VRMLPROCESSING_API IndexedFaceSetAction : public vrml_proc::action::GeometryAction {

		public:

            struct Properties {
                std::reference_wrapper<const vrml_proc::parser::VrmlNode> color;
                std::reference_wrapper<const vrml_proc::parser::VrmlNode> coord;
                std::reference_wrapper<const vrml_proc::parser::VrmlNode> normal;
                std::reference_wrapper<const vrml_proc::parser::VrmlNode> texCoord;
                std::reference_wrapper<const bool> ccw;
                std::reference_wrapper<const vrml_proc::parser::Int32Array> colorIndex;
                std::reference_wrapper<const bool> colorPerVertex;
                std::reference_wrapper<const bool> convex;
                std::reference_wrapper<const vrml_proc::parser::Int32Array> coordIndex;
                std::reference_wrapper<const vrml_proc::parser::float32_t> creaseangle;
                std::reference_wrapper<const vrml_proc::parser::Int32Array> normalIndex;
                std::reference_wrapper<const bool> normalPerVertex;
                std::reference_wrapper<const bool> solid;
                std::reference_wrapper<const vrml_proc::parser::Int32Array> texCoordIndex;
            };

			IndexedFaceSetAction(Properties properties, bool containedByShape);
			std::shared_ptr<vrml_proc::conversion_context::MeshConversionContext> Execute() override;

		private:
			Properties m_properties;
		};
	}
}
