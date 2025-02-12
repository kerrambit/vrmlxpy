#pragma once

#include "TransformationMatrix.hpp"
#include "VrmlNode.hpp"
#include "VrmlNodeManager.hpp"

#include "VrmlProcessingExport.hpp"

namespace vrml_proc {
	namespace traversor {

		struct VRMLPROCESSING_API FullParsedVrmlNodeContext {

			FullParsedVrmlNodeContext(const vrml_proc::parser::VrmlNode& node, const vrml_proc::parser::VrmlNodeManager& manager, bool isDescendantOfShape, vrml_proc::math::TransformationMatrix transformation)
				: node(node), manager(manager), IsDescendantOfShape(isDescendantOfShape), transformation(transformation) {}

			const vrml_proc::parser::VrmlNode& node;
			const vrml_proc::parser::VrmlNodeManager& manager;
			bool IsDescendantOfShape;
			vrml_proc::math::TransformationMatrix transformation;
		};
	}
}
