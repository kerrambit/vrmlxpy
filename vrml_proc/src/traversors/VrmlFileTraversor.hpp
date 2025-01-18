#pragma once

#include <memory>

#include "BaseConversionContext.hpp"
#include "BaseConversionContextActionMap.hpp"
#include "FullParsedVrmlFileContext.hpp"
#include "MeshConversionContext.hpp"
#include "VrmlFile.hpp"
#include "VrmlNodeManager.hpp"
#include "VrmlNodeTraversor.hpp"

#include "VrmlProcessingExport.hpp"

namespace vrml_proc {
	namespace traversor {
		namespace  VrmlFileTraversor {
			template<typename DefaultConversionContext>
			VRMLPROCESSING_API std::shared_ptr<vrml_proc::conversion_context::BaseConversionContext> Traverse(FullParsedVrmlFileContext context, const vrml_proc::action::BaseConversionContextActionMap& actionMap);
		}
	}
}
