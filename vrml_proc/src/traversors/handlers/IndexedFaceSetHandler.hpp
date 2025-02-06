#pragma once

#include <any>
#include <memory>

#include <result.hpp>

#include "ConversionContextActionExecutor.hpp"
#include "ConversionContextActionMap.hpp"
#include "Error.hpp"
#include "IndexedFaceSetNodeValidator.hpp"
#include "Int32Array.hpp"
#include "Logger.hpp"
#include "MeshConversionContext.hpp"
#include "NodeTraversorError.hpp"
#include "VrmlNode.hpp"
#include "VrmlUnits.hpp"
#include "FullParsedVrmlNodeContext.hpp"

#include "VrmlProcessingExport.hpp"

namespace vrml_proc {
	namespace traversor {
		namespace handler {
			namespace IndexedFaceSetHandler {

				struct IndexedFaceSetProperties {
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

				template<typename ConversionContext>
                VRMLPROCESSING_API cpp::result<std::shared_ptr<ConversionContext>, std::shared_ptr<vrml_proc::core::error::Error>> Handle(vrml_proc::traversor::FullParsedVrmlNodeContext context, const vrml_proc::action::ConversionContextActionMap<ConversionContext>& actionMap);
			}
		}
	}
}