#pragma once

#include "VrmlNode.hpp"
#include "VrmlNodeManager.hpp"

#include "VrmlProcessingExport.hpp"

namespace vrml_proc {
	namespace traversor {

		struct VRMLPROCESSING_API FullParsedVrmlNodeContext {
			FullParsedVrmlNodeContext(const vrml_proc::parser::VrmlNode& node, const vrml_proc::parser::VrmlNodeManager& manager)
				: node(node), manager(manager), IsDescendantOfShape(false) {}

			FullParsedVrmlNodeContext(const vrml_proc::parser::VrmlNode& node, const vrml_proc::parser::VrmlNodeManager& manager, bool isDescendantOfShape)
				: node(node), manager(manager), IsDescendantOfShape(isDescendantOfShape) {}

			const vrml_proc::parser::VrmlNode& node;
			const vrml_proc::parser::VrmlNodeManager& manager;
			bool IsDescendantOfShape;
		};
	}
}
